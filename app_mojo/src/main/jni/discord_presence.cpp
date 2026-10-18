#include <jni.h>
#include <android/log.h>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

#define LOG_TAG "CryonixDiscordPresence"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

#ifdef CRYONIX_HAS_DISCORD_SDK
#include "discordpp.h"
namespace {
std::mutex g_mutex;
std::shared_ptr<discordpp::Client> g_client;
std::atomic<bool> g_callbackPumpStarted{false};
std::atomic<bool> g_callbackPumpRunning{true};
std::thread g_callbackPump;

void startCallbackPump() {
    bool expected = false;
    if (!g_callbackPumpStarted.compare_exchange_strong(expected, true)) return;
    g_callbackPump = std::thread([] {
        while (g_callbackPumpRunning.load()) {
            try { discordpp::RunCallbacks(); }
            catch (...) { LOGW("Discord callback pump caught an exception"); }
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    });
    g_callbackPump.detach();
}

bool ensureClient(uint64_t applicationId) {
    if (!g_client) {
        try {
            g_client = std::make_shared<discordpp::Client>();
            g_client->SetApplicationId(applicationId);
            LOGI("Discord Social SDK client initialized for app %llu", static_cast<unsigned long long>(applicationId));
        } catch (const std::exception& e) {
            LOGW("Discord Social SDK initialization failed: %s", e.what());
            g_client.reset();
            return false;
        } catch (...) {
            LOGW("Discord Social SDK initialization failed");
            g_client.reset();
            return false;
        }
    }
    startCallbackPump();
    return true;
}

void publishPresence(uint64_t applicationId, const std::string& details,
                     const std::string& state, uint64_t startTimestampMs) {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (!ensureClient(applicationId)) return;
    try {
        discordpp::Activity activity;
        activity.SetType(discordpp::ActivityTypes::Playing);
        activity.SetName("CryonixlauncherPlus");
        activity.SetDetails(details);
        activity.SetState(state);
        activity.SetSupportedPlatforms(discordpp::ActivityGamePlatforms::Android);
        discordpp::ActivityTimestamps timestamps;
        timestamps.SetStart(startTimestampMs);
        activity.SetTimestamps(timestamps);
        g_client->UpdateRichPresence(std::move(activity), [](discordpp::ClientResult result) {
            if (result.Successful()) LOGI("Discord Rich Presence update succeeded");
            else LOGW("Discord Rich Presence update failed: %s", result.Error().c_str());
        });
    } catch (const std::exception& e) {
        LOGW("Could not publish Discord Rich Presence: %s", e.what());
    } catch (...) {
        LOGW("Could not publish Discord Rich Presence");
    }
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
    LOGW("Discord Rich Presence is unavailable for this ABI; SDK library is not bundled");
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
