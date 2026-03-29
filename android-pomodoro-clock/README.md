# Android Pomodoro Clock

A standalone Android app implementing the Pomodoro technique with Jetpack Compose.

## Features

- Start/pause/reset timer controls
- Skip to next phase control
- Pomodoro cycle flow:
  - Focus session
  - Short break
  - After 4 focus sessions, long break
- Configurable durations:
  - Focus minutes
  - Short-break minutes
  - Long-break minutes
- Completed focus session counter
- Auto-start toggle for the next phase
- Progress display (percentage + mm:ss countdown)

## Tech stack

- Kotlin
- Jetpack Compose (Material 3)
- ViewModel + StateFlow
- Pure timer engine unit tests

## Project structure

- `app/src/main/java/com/example/pomodoro/`
  - `PomodoroState.kt`: UI state model
  - `PomodoroTimerEngine.kt`: pure state-transition logic
  - `PomodoroViewModel.kt`: timer ticking + state orchestration
  - `MainActivity.kt`: app entry point
  - `ui/PomodoroApp.kt`: Compose UI
- `app/src/test/java/com/example/pomodoro/PomodoroTimerEngineTest.kt`

## Build and run

1. Open `android-pomodoro-clock` in Android Studio.
2. Let Android Studio install missing SDK/platform dependencies.
3. Run the `app` configuration on an emulator or device.

## Notes

- This implementation keeps timing in-app through ViewModel coroutines.
- For background execution, notifications, and persistence across process death, add a foreground service and local storage in a future iteration.
