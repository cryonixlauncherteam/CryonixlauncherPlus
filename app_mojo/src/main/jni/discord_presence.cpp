#include <jni.h>
#include <android/log.h>
#include <ctime>
#include <memory>
#include <mutex>

#define LOG_TAG "CryonixDiscordPresence"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

#ifdef CRYONIX_HAS_DISCORD_SDK
#include "discordpp.h"
namespace {
std::mutex g_mutex;
std::shared_ptr<discordpp::Client> g_client;
}

extern "C" JNIEXPORT void JNICALL
Java_git_artdeell_mojo_DiscordPresenceManager_nativeStart(
        JNIEnv*, jclass, jlong applicationId) {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (g_client) return;
    try {
        g_client = std::make_shared<discordpp::Client>();
        // Android RPC presence works without OAuth; Discord must be installed and signed in.
        g_client->SetApplicationId(static_cast<uint64_t>(applicationId));
        discordpp::Activity activity;
        activity.SetType(discordpp::ActivityTypes::Playing);
        activity.SetName("CryonixlauncherPlus");
        activity.SetDetails("Launcher is open");
        activity.SetState("Getting Minecraft ready");
        discordpp::ActivityTimestamps timestamps;
        timestamps.SetStart(static_cast<uint64_t>(std::time(nullptr)));
        activity.SetTimestamps(timestamps);
        g_client->UpdateRichPresence(std::move(activity), [](discordpp::ClientResult result) {
            if (result.Successful()) {
                LOGI("Discord Rich Presence updated");
            } else {
                LOGW("Discord Rich Presence update failed");
            }
        });
    } catch (...) {
        LOGW("Exception while initializing Discord Rich Presence");
        g_client.reset();
    }
}
#else
extern "C" JNIEXPORT void JNICALL
Java_git_artdeell_mojo_DiscordPresenceManager_nativeStart(JNIEnv*, jclass, jlong) {
    LOGW("Discord SDK not bundled for this ABI; presence is unavailable");
}
#endif
