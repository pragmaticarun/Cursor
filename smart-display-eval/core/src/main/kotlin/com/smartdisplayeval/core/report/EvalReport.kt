package com.smartdisplayeval.core.report

import com.smartdisplayeval.core.analysis.ArtifactEvent
import com.smartdisplayeval.core.analysis.AvSyncReport
import com.smartdisplayeval.core.analysis.FluidityReport

/** Outcome of checking a single [com.smartdisplayeval.core.scenario.Expectation]. */
data class ExpectationResult(
    val description: String,
    val passed: Boolean,
    val detail: String,
)

/** Result of one scenario step: what was done and whether it met expectations. */
data class StepResult(
    val name: String,
    val actionDescription: String,
    val startMs: Long,
    val endMs: Long,
    val expectationResults: List<ExpectationResult>,
    val artifacts: List<ArtifactEvent>,
) {
    val passed: Boolean get() = expectationResults.all { it.passed }
}

/**
 * The full result of running a scenario — the artifact an agentic workflow
 * grades against. It is intentionally structured and timestamped so a grader
 * can both compute a score and point at exact failure moments.
 */
data class EvalReport(
    val scenarioName: String,
    val target: String,
    val startedAtMs: Long,
    val durationMs: Long,
    val stepResults: List<StepResult>,
    val artifacts: List<ArtifactEvent>,
    val fluidity: FluidityReport,
    val avSync: AvSyncReport?,
) {
    /** True if every step met all its expectations. */
    val passed: Boolean get() = stepResults.all { it.passed }

    /**
     * A 0..100 quality score blending step pass-rate, fluidity and an artifact
     * penalty. Provides a single scalar for ranking / regression tracking while
     * the detailed events remain available for inspection.
     */
    fun score(): Double {
        if (stepResults.isEmpty()) return 0.0
        val passRate = stepResults.count { it.passed }.toDouble() / stepResults.size
        val artifactPenalty = artifacts.sumOf { it.severity * it.confidence }
            .let { (it * 6.0).coerceAtMost(45.0) }
        val fluidityComponent = fluidity.smoothness / 100.0
        val raw = 100.0 * (0.55 * passRate + 0.30 * fluidityComponent + 0.15) - artifactPenalty
        return raw.coerceIn(0.0, 100.0)
    }

    /** Serialise to JSON for machine grading / storage. */
    fun toJson(): String = EvalReportJson.render(this)
}
