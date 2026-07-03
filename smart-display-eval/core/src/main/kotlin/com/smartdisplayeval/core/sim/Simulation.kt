package com.smartdisplayeval.core.sim

import com.smartdisplayeval.core.control.FrameSource
import com.smartdisplayeval.core.control.RemoteController
import com.smartdisplayeval.core.frame.Frame
import com.smartdisplayeval.core.scenario.RemoteKey

/**
 * A [FrameSource] backed by a precomputed list of frames (in increasing
 * timestamp order). Used by tests and the demo to replay a deterministic
 * "capture" through the same runner the device uses.
 */
class ScriptedFrameSource(private val frames: List<Frame>) : FrameSource {
    private var i = 0
    override fun next(): Frame? = if (i < frames.size) frames[i++] else null
}

/** A [RemoteController] that records the commands it was asked to send. */
class RecordingRemoteController : RemoteController {
    data class Command(val kind: String, val detail: String)

    val commands = ArrayList<Command>()

    override fun press(key: RemoteKey, repeat: Int, intervalMs: Long) {
        commands.add(Command("press", "${key.label} x$repeat"))
    }

    override fun launchApp(packageName: String, activity: String?) {
        commands.add(Command("launch", packageName + (activity?.let { "/$it" } ?: "")))
    }

    override fun inputText(text: String) {
        commands.add(Command("text", text))
    }
}
