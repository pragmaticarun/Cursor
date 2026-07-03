package com.smartdisplayeval.app.control

import com.smartdisplayeval.core.control.RemoteController
import com.smartdisplayeval.core.scenario.RemoteKey
import dadb.Dadb

/**
 * Drives a Fire TV Stick (or any Fire OS / Android TV device) over ADB-on-TCP.
 *
 * Fire OS is Android, so remote input maps directly to `input keyevent`, app
 * launches to `am start`, and text entry to `input text`. We use the pure-JVM
 * [dadb] client so the phone talks ADB to the TV directly over Wi‑Fi, with no
 * host `adb` server involved.
 *
 * Prerequisites on the Fire TV: Settings → My Fire TV → Developer Options →
 * "ADB debugging" ON. The first connection pops an authorization dialog on the
 * TV that must be accepted.
 */
class AdbRemoteController private constructor(private val dadb: Dadb) : RemoteController {

    override fun press(key: RemoteKey, repeat: Int, intervalMs: Long) {
        repeat(repeat.coerceAtLeast(1)) { i ->
            dadb.shell("input keyevent ${key.androidKeyCode}")
            if (i < repeat - 1 && intervalMs > 0) Thread.sleep(intervalMs)
        }
    }

    override fun launchApp(packageName: String, activity: String?) {
        if (activity != null) {
            dadb.shell("am start -n $packageName/$activity")
        } else {
            // Launch the app's default launcher activity.
            dadb.shell("monkey -p $packageName -c android.intent.category.LAUNCHER 1")
        }
    }

    override fun inputText(text: String) {
        // `input text` treats spaces specially; %s is the accepted escape.
        dadb.shell("input text ${text.replace(" ", "%s")}")
    }

    override fun close() {
        runCatching { dadb.close() }
    }

    companion object {
        /** Connect to a device's ADB TCP endpoint (Fire TV default port 5555). */
        fun connect(host: String, port: Int = 5555): AdbRemoteController =
            AdbRemoteController(Dadb.create(host, port))
    }
}
