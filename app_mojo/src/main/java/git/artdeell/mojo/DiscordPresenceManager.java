package git.artdeell.mojo;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

/** Discord Social SDK Rich Presence for CryonixlauncherPlus. */
public final class DiscordPresenceManager {
    public static final String ACTION_UPDATE = "git.artdeell.mojo.action.DISCORD_PRESENCE";
    public static final String EXTRA_DETAILS = "presence_details";
    public static final String EXTRA_STATE = "presence_state";
    public static final String EXTRA_START_MS = "presence_start_ms";
    private static final String TAG = "CryonixDiscordPresence";
    private static final long APPLICATION_ID = 1557412253131743232L;
    private static volatile boolean started;
    private static volatile long sessionStartMs;

    static {
        try {
            System.loadLibrary("cryonix_discord_presence");
        } catch (Throwable error) {
            Log.e(TAG, "Discord Rich Presence native bridge could not be loaded", error);
        }
    }

    private DiscordPresenceManager() {}

    public static synchronized void start(Context context) {
        if (started) return;
        started = true;
        sessionStartMs = System.currentTimeMillis();
        try {
            nativeStart(APPLICATION_ID, "Minecraft Java Edition on Android", "Launcher is ready", sessionStartMs);
        } catch (Throwable error) {
            Log.e(TAG, "Could not start Discord Rich Presence", error);
        }
    }

    public static void showGame(Context context, String version) {
        String safeVersion = (version == null || version.trim().isEmpty()) ? "Minecraft" : version.trim();
        sendUpdate(context, "Minecraft Java Edition on Android", "Playing Minecraft " + safeVersion,
                System.currentTimeMillis());
    }

    public static void showLauncher(Context context) {
        sendUpdate(context, "Minecraft Java Edition on Android", "Launcher is ready", System.currentTimeMillis());
    }

    private static void sendUpdate(Context context, String details, String state, long startMs) {
        Intent intent = new Intent(context, DiscordPresenceReceiver.class);
        intent.setAction(ACTION_UPDATE);
        intent.putExtra(EXTRA_DETAILS, details);
        intent.putExtra(EXTRA_STATE, state);
        intent.putExtra(EXTRA_START_MS, startMs);
        context.sendBroadcast(intent);
    }

    static void applyBroadcast(Context context, Intent intent) {
        if (intent == null || !ACTION_UPDATE.equals(intent.getAction())) return;
        start(context);
        String details = intent.getStringExtra(EXTRA_DETAILS);
        String state = intent.getStringExtra(EXTRA_STATE);
        long startMs = intent.getLongExtra(EXTRA_START_MS, System.currentTimeMillis());
        try {
            nativeUpdate(APPLICATION_ID,
                    details == null ? "Minecraft Java Edition on Android" : details,
                    state == null ? "Launcher is ready" : state,
                    startMs);
        } catch (Throwable error) {
            Log.e(TAG, "Could not update Discord Rich Presence", error);
        }
    }

    private static native void nativeStart(long applicationId, String details, String state, long startTimestampMs);
    private static native void nativeUpdate(long applicationId, String details, String state, long startTimestampMs);
}
