package com.smartdisplayeval.core.control

import com.smartdisplayeval.core.frame.Frame
import com.smartdisplayeval.core.scenario.RemoteKey

/**
 * Sends control input to the device under test. Implementations live outside
 * the core (e.g. an ADB-over-TCP controller for a Fire TV Stick), keeping the
 * analysis engine free of transport concerns.
 */
interface RemoteController {
    /** Press [key], repeated [repeat] times with [intervalMs] between presses. */
    fun press(key: RemoteKey, repeat: Int = 1, intervalMs: Long = 120)

    /** Launch an application (Fire OS: `am start`). */
    fun launchApp(packageName: String, activity: String? = null)

    /** Inject text input (Fire OS: `input text`). */
    fun inputText(text: String)

    /** Release any transport resources. */
    fun close() {}
}

/**
 * Produces [Frame]s from the observed display. A camera pointed at the TV, an
 * HDMI capture dongle, or a recorded sequence all implement this. `next()`
 * blocks until a frame is available and returns null when the source is
 * exhausted (recorded sources) — live sources simply keep returning frames.
 */
interface FrameSource {
    fun next(): Frame?

    /** Optional cleanup. */
    fun close() {}
}
