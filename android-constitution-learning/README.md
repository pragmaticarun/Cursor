# Samvidhaan — Learn the Indian Constitution (Android) 🇮🇳

A native **Android** app, built with **Jetpack Compose** and **Material Design 3**, to
learn the **Constitution of India**. It is the Android counterpart of the
[`indian-constitution-learning`](../indian-constitution-learning) web app and shares the
same learning content.

The app explains the structure of the Constitution, provides plain-language
explanations of its most important articles, and includes a knowledge check with
**flashcards** and a **quiz** — all fully offline.

## ✨ Features

- **Material Design 3** UI with a tricolour-inspired colour scheme, light/dark theme, and **dynamic colour** (Material You) on Android 12+.
- **Bottom navigation** across five sections:
  - **Overview** — quick facts, the Preamble, and key notes about it.
  - **Structure** — all Parts (I–XXII, including IVA, IXA, IXB, XIVA) and all 12 Schedules.
  - **Articles** — expandable cards with "what it says" and "in simple terms" explanations, **search** and **filter-by-Part** chips.
  - **Cards** — animated flip **flashcards** with previous/next navigation and shuffle.
  - **Quiz** — multiple-choice questions with instant feedback, per-question explanations, live scoring, a progress bar, and a results screen.
- **Offline first** — all content is bundled in the app; no network required.
- **Edge-to-edge** layout and adaptive theming.

## 🧱 Tech stack

- Kotlin + Jetpack Compose (BOM `2024.06.00`)
- Material 3 (`androidx.compose.material3`) + extended Material icons
- Android Gradle Plugin 8.5.2, Kotlin 1.9.24
- `compileSdk` / `targetSdk` 34, `minSdk` 24

## 📂 Project structure

```
android-constitution-learning/
├── app/
│   └── src/
│       ├── main/
│       │   ├── AndroidManifest.xml
│       │   ├── java/com/example/constitution/
│       │   │   ├── MainActivity.kt
│       │   │   ├── ConstitutionData.kt        # All learning content (data classes + data)
│       │   │   └── ui/
│       │   │       ├── ConstitutionApp.kt     # Scaffold + bottom navigation
│       │   │       ├── OverviewScreen.kt
│       │   │       ├── StructureScreen.kt
│       │   │       ├── ArticlesScreen.kt
│       │   │       ├── FlashcardsScreen.kt
│       │   │       ├── QuizScreen.kt
│       │   │       └── theme/                  # Material 3 theme + colours
│       │   └── res/
│       └── test/java/com/example/constitution/
│           └── ConstitutionDataTest.kt        # Data-integrity unit tests
├── build.gradle.kts
├── settings.gradle.kts
└── gradle/ + gradlew                          # Gradle wrapper (8.9)
```

## 🚀 Building & running

### Android Studio
1. Open the `android-constitution-learning/` folder in Android Studio.
2. Let Gradle sync (it will download dependencies).
3. Run the `app` configuration on an emulator or device (API 24+).

### Command line
From inside `android-constitution-learning/`:

```bash
# Run unit tests
./gradlew test

# Build a debug APK
./gradlew assembleDebug
# Output: app/build/outputs/apk/debug/app-debug.apk
```

> Building requires an Android SDK and internet access to download Gradle and the
> AndroidX/Compose dependencies the first time.

## 🧩 Editing the content

All study content lives in [`app/.../ConstitutionData.kt`](app/src/main/java/com/example/constitution/ConstitutionData.kt)
as simple Kotlin lists: `facts`, `preamble`, `preambleNotes`, `parts`, `schedules`,
`articles`, `flashcards` and `quiz`. Add an item to any list and it appears in the UI
automatically. The unit tests in `ConstitutionDataTest.kt` help catch mistakes such as
out-of-range quiz answers or duplicate article numbers.

## 📝 A note on accuracy

This project is intended for **education and study**. The explanations are simplified
for learning and may omit nuance. For authoritative legal text, always refer to the
official Constitution of India as published by the Government of India.
