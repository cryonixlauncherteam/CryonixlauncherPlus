package git.artdeell.mojo;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

/** Receives in-game status changes in the launcher process that owns Discord RPC. */
public final class DiscordPresenceReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        DiscordPresenceManager.applyBroadcast(context, intent);
    }
}
