package com.smartdisplayeval.app.scenario

import com.smartdisplayeval.core.analysis.ArtifactType
import com.smartdisplayeval.core.analysis.SceneType
import com.smartdisplayeval.core.scenario.Expectation
import com.smartdisplayeval.core.scenario.RemoteKey
import com.smartdisplayeval.core.scenario.Scenario
import com.smartdisplayeval.core.scenario.Step
import com.smartdisplayeval.core.scenario.StepAction
import com.smartdisplayeval.core.scenario.TargetDevice

/** Ready-made Fire TV Stick evaluation scenarios the app can run out of the box. */
object FireTvScenarios {

    /** Common Fire TV app package names. */
    const val PKG_PRIME_VIDEO = "com.amazon.avod"

    val all: List<Scenario> = listOf(
        homeNavigationFluidity(),
        browseAndPlay(),
    )

    /**
     * Exercises UI fluidity: go home, then scroll the home carousel and rows.
     * Verifies the scene is UI motion and the scrolling stays smooth.
     */
    fun homeNavigationFluidity(): Scenario = Scenario(
        name = "Home navigation fluidity",
        description = "Scroll the Fire TV home carousel and rows; check scrolling is smooth UI motion.",
        target = TargetDevice.FIRE_TV_STICK,
        steps = listOf(
            Step(
                name = "Go Home",
                action = StepAction.PressKey(RemoteKey.HOME),
                observeMs = 1_500,
                expectations = listOf(Expectation.SceneShouldBe(SceneType.STATIC, minFraction = 0.4)),
            ),
            Step(
                name = "Scroll carousel right",
                action = StepAction.PressKey(RemoteKey.DPAD_RIGHT, repeat = 8, intervalMs = 220),
                observeMs = 2_600,
                expectations = listOf(
                    Expectation.SceneShouldBe(SceneType.UI_MOTION, minFraction = 0.4),
                    Expectation.MinFluidity(min = 60.0),
                ),
            ),
            Step(
                name = "Move down through rows",
                action = StepAction.PressKey(RemoteKey.DPAD_DOWN, repeat = 5, intervalMs = 260),
                observeMs = 2_000,
                expectations = listOf(Expectation.MinFluidity(min = 55.0)),
            ),
        ),
    )

    /**
     * Opens Prime Video, starts playback, and watches for video-quality defects
     * (macroblocking / low resolution / audio loss / freezes) and AV sync.
     */
    fun browseAndPlay(pkg: String = PKG_PRIME_VIDEO): Scenario = Scenario(
        name = "Browse and play a title",
        description = "Launch a streaming app, start playback, and verify picture/sound quality.",
        target = TargetDevice.FIRE_TV_STICK,
        steps = listOf(
            Step(
                name = "Launch app",
                action = StepAction.LaunchApp(pkg),
                observeMs = 4_000,
            ),
            Step(
                name = "Open featured title",
                action = StepAction.PressKey(RemoteKey.DPAD_CENTER),
                observeMs = 2_000,
            ),
            Step(
                name = "Start playback",
                action = StepAction.PressKey(RemoteKey.MEDIA_PLAY_PAUSE),
                observeMs = 8_000,
                expectations = listOf(
                    Expectation.SceneShouldBe(SceneType.VIDEO, minFraction = 0.5),
                    Expectation.MinFluidity(min = 60.0),
                    Expectation.NoArtifacts(
                        types = setOf(
                            ArtifactType.MACROBLOCKING,
                            ArtifactType.LOW_RESOLUTION,
                            ArtifactType.AUDIO_LOSS,
                            ArtifactType.FRAME_STALL,
                            ArtifactType.BLACK_FRAME,
                        ),
                        maxSeverity = 0.2,
                    ),
                    Expectation.MaxAvSyncMs(maxAbsMs = 120),
                ),
            ),
        ),
    )
}
