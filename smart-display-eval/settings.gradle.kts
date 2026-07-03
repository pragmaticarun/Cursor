pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}

// Lets Gradle auto-provision the JDK 17 toolchain the modules target, so the
// build is reproducible regardless of the JDK installed on the machine.
plugins {
    id("org.gradle.toolchains.foojay-resolver-convention") version "0.8.0"
}

@Suppress("UnstableApiUsage")
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
        // dadb (ADB-over-TCP client) is published on Maven Central; JitPack kept
        // as a fallback for any transitive artifacts.
        maven { url = uri("https://jitpack.io") }
    }
}

rootProject.name = "SmartDisplayEval"
include(":core")
include(":app")
