package com.smartdisplayeval.core.scenario

import com.smartdisplayeval.core.analysis.ArtifactType
import com.smartdisplayeval.core.analysis.SceneType

/** Target device family for a scenario (affects defaults / key maps). */
enum class TargetDevice { FIRE_TV_STICK, ANDROID_TV, GENERIC }

/** An action the automated tester performs on the device. */
sealed interface StepAction {
    /** Press a remote key, optionally repeated (e.g. scroll a list). */
    data class PressKey(val key: RemoteKey, val repeat: Int = 1, val intervalMs: Long = 120) : StepAction

    /** Launch an app by package (and optional activity), e.g. Prime Video. */
    data class LaunchApp(val packageName: String, val activity: String? = null) : StepAction

    /** Type text (into a search box etc.). */
    data class InputText(val text: String) : StepAction

    /** Do nothing but keep observing for the given duration. */
    data object Idle : StepAction
}

/**
 * A condition the eval verifies while/after a step runs. Each expectation is
 * checked against the analysis collected during the step's observation window.
 */
sealed interface Expectation {
    /** The scene should be classified as [type] for most of the window. */
    data class SceneShouldBe(val type: SceneType, val minFraction: Double = 0.6) : Expectation

    /** Motion smoothness (0..100) should be at least [min]. */
    data class MinFluidity(val min: Double) : Expectation

    /** None of [types] should be detected above [maxSeverity]. */
    data class NoArtifacts(
        val types: Set<ArtifactType>,
        val maxSeverity: Double = 0.0,
    ) : Expectation

    /** Absolute AV-sync offset should not exceed [maxAbsMs]. */
    data class MaxAvSyncMs(val maxAbsMs: Long) : Expectation
}

/**
 * One step of a scenario: perform [action], observe the display for
 * [observeMs], then check [expectations].
 */
data class Step(
    val name: String,
    val action: StepAction,
    val observeMs: Long = 2_000,
    val expectations: List<Expectation> = emptyList(),
)

/** A full evaluation scenario: an ordered list of steps against a device. */
data class Scenario(
    val name: String,
    val description: String,
    val steps: List<Step>,
    val target: TargetDevice = TargetDevice.FIRE_TV_STICK,
)
