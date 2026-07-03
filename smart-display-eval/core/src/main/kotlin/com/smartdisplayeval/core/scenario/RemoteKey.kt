package com.smartdisplayeval.core.scenario

/**
 * Remote-control buttons, mapped to Android/Fire OS `KEYCODE_*` values.
 *
 * A Fire TV Stick runs Fire OS (Android) and accepts key events over ADB, so
 * these codes can be dispatched directly with `input keyevent <code>`. The enum
 * keeps the eval scenarios device-agnostic; a different target can supply its
 * own key map.
 */
enum class RemoteKey(val androidKeyCode: Int, val label: String) {
    HOME(3, "Home"),
    BACK(4, "Back"),
    DPAD_UP(19, "Up"),
    DPAD_DOWN(20, "Down"),
    DPAD_LEFT(21, "Left"),
    DPAD_RIGHT(22, "Right"),
    DPAD_CENTER(23, "Select"),
    VOLUME_UP(24, "Volume Up"),
    VOLUME_DOWN(25, "Volume Down"),
    MENU(82, "Menu"),
    MEDIA_PLAY_PAUSE(85, "Play/Pause"),
    MEDIA_STOP(86, "Stop"),
    MEDIA_NEXT(87, "Next"),
    MEDIA_PREVIOUS(88, "Previous"),
    MEDIA_REWIND(89, "Rewind"),
    MEDIA_FAST_FORWARD(90, "Fast Forward"),
    MEDIA_PLAY(126, "Play"),
    MEDIA_PAUSE(127, "Pause"),
    MUTE(164, "Mute"),
    WAKEUP(224, "Wake"),
    SEARCH(84, "Search"),
}
