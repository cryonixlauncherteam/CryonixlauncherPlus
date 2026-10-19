#include <jni.h>
#include <android/log.h>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

#define LOG_TAG "CryonixDiscordPresence"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

#ifdef CRYONIX_HAS_DISCORD_SDK
#include "cdiscord.h"
namespace {
std::mutex g_mutex;
Discord_Client g_client{};
bool g_clientInitialized = false;
std::atomic<bool> g_callbackPumpStarted{false};
std::atomic<bool> g_stopCallbackPump{false};

Discord_String stringRef(const std::string& value) {
    return Discord_String{reinterpret_cast<uint8_t*>(const_cast<char*>(value.data())), value.size()};
}

void freeCallbackData(void*) {}
void onPresenceUpdated(Discord_ClientResult*, void*) {
    LOGI("Discord Rich Presence update callback received");
}

void startCallbackPump() {
    bool expected = false;
    if (!g_callbackPumpStarted.compare_exchange_strong(expected, true)) return;
    g_stopCallbackPump.store(false);
    std::thread([] {
        while (!g_stopCallbackPump.load()) {
            Discord_RunCallbacks();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        LOGI("Discord callback pump stopped");
    }).detach();
}

bool ensureClient(uint64_t applicationId) {
    if (!g_clientInitialized) {
        Discord_Client_Init(&g_client);
        Discord_Client_SetApplicationId(&g_client, applicationId);
        g_clientInitialized = true;
        LOGI("Discord Social SDK C client initialized for app %llu",
             static_cast<unsigned long long>(applicationId));
    }
    startCallbackPump();
    return true;
}

void publishPresence(uint64_t applicationId, const std::string& details,
                     const std::string& state, uint64_t startTimestampMs) {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (!ensureClient(applicationId)) return;

    Discord_Activity activity{};
    Discord_ActivityTimestamps timestamps{};
    Discord_Activity_Init(&activity);
    Discord_ActivityTimestamps_Init(&timestamps);

    const std::string appName = "CryonixlauncherPlus";
    Discord_String nameValue = stringRef(appName);
    Discord_String detailsValue = stringRef(details);
    Discord_String stateValue = stringRef(state);

    Discord_Activity_SetType(&activity, Discord_ActivityTypes_Playing);
    Discord_Activity_SetName(&activity, nameValue);
    Discord_Activity_SetDetails(&activity, &detailsValue);
    Discord_Activity_SetState(&activity, &stateValue);
    Discord_ActivityTimestamps_SetStart(&timestamps, startTimestampMs);
    Discord_Activity_SetTimestamps(&activity, &timestamps);
    Discord_Activity_SetSupportedPlatforms(&activity, Discord_ActivityGamePlatforms_Android);

    Discord_Client_UpdateRichPresence(&g_client, &activity, onPresenceUpdated, freeCallbackData, nullptr);

    Discord_ActivityTimestamps_Drop(&timestamps);
    Discord_Activity_Drop(&activity);
}

std::string fromJava(JNIEnv* env, jstring value, const char* fallback) {
    if (!value) return fallback;
    const char* chars = env->GetStringUTFChars(value, nullptr);
    if (!chars) return fallback;
    std::string result(chars);
    env->ReleaseStringUTFChars(value, chars);
    return result;
}
}
#else
namespace {
std::string fromJava(JNIEnv* env, jstring value, const char* fallback) {
    if (!value) return fallback;
    const char* chars = env->GetStringUTFChars(value, nullptr);
    if (!chars) return fallback;
    std::string result(chars);
    env->ReleaseStringUTFChars(value, chars);
    return result;
}
}
#endif

extern "C" JNIEXPORT void JNICALL
Java_git_artdeell_mojo_DiscordPresenceManager_nativeStart(
        JNIEnv* env, jclass, jlong applicationId, jstring details, jstring state, jlong startTimestampMs) {
#ifdef CRYONIX_HAS_DISCORD_SDK
    publishPresence(static_cast<uint64_t>(applicationId), fromJava(env, details, "Minecraft Java Edition on Android"),
                    fromJava(env, state, "Launcher is ready"), static_cast<uint64_t>(startTimestampMs));
#else
    (void) applicationId; (void) startTimestampMs;
    LOGW("Discord Rich Presence unavailable: SDK library is not bundled for this ABI");
#endif
}

extern "C" JNIEXPORT void JNICALL
Java_git_artdeell_mojo_DiscordPresenceManager_nativeUpdate(
        JNIEnv* env, jclass, jlong applicationId, jstring details, jstring state, jlong startTimestampMs) {
#ifdef CRYONIX_HAS_DISCORD_SDK
    publishPresence(static_cast<uint64_t>(applicationId), fromJava(env, details, "Minecraft Java Edition on Android"),
                    fromJava(env, state, "Launcher is ready"), static_cast<uint64_t>(startTimestampMs));
#else
    (void) env; (void) applicationId; (void) details; (void) state; (void) startTimestampMs;
    LOGW("Discord Rich Presence update skipped: SDK library is not bundled for this ABI");
#endif
}

extern "C" JNIEXPORT void JNICALL
Java_git_artdeell_mojo_DiscordPresenceManager_nativeClear(
        JNIEnv*, jclass, jlong applicationId) {
#ifdef CRYONIX_HAS_DISCORD_SDK
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (g_clientInitialized) {
            Discord_Client_ClearRichPresence(&g_client);
            LOGI("Discord Rich Presence cleared for app %llu",
                 static_cast<unsigned long long>(applicationId));
        }
    }
    // Allow the asynchronous clear RPC to be processed before stopping callbacks.
    std::thread([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(350));
        g_stopCallbackPump.store(true);
    }).detach();
#else
    (void) applicationId;
    LOGW("Discord Rich Presence clear skipped: SDK library is not bundled for this ABI");
#endif
}
