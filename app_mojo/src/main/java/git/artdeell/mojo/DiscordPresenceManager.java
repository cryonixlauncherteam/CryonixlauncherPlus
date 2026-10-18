package git.artdeell.mojo;

import android.util.Log;

/**
 * Optional Discord Social SDK Rich Presence for Cryonix Launcher Plus.
 * The native bridge uses the SDK's Android RPC transport (no OAuth token or client secret).
 */
public final class DiscordPresenceManager {
    private static final String TAG = "CryonixDiscordPresence";
    private static final long APPLICATION_ID = 1557412253131743232L;
    private static boolean started;

    static {
        try {
            System.loadLibrary("cryonix_discord_presence");
        } catch (Throwable error) {
            Log.w(TAG, "Discord Rich Presence native bridge unavailable", error);
        }
    }

    private DiscordPresenceManager() {}

    public static synchronized void start() {
        if (started) return;
        started = true;
        try {
            nativeStart(APPLICATION_ID);
        } catch (Throwable error) {
            Log.w(TAG, "Could not start Discord Rich Presence", error);
        }
    }

    private static native void nativeStart(long applicationId);
}
