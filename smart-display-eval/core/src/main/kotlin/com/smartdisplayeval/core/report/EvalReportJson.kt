package com.smartdisplayeval.core.report

/** Renders an [EvalReport] to JSON using the dependency-free [JsonWriter]. */
object EvalReportJson {
    fun render(report: EvalReport): String {
        return JsonWriter().obj {
            str("scenario", report.scenarioName)
            str("target", report.target)
            num("startedAtMs", report.startedAtMs)
            num("durationMs", report.durationMs)
            bool("passed", report.passed)
            num("score", report.score())

            obj("fluidity") {
                num("frameCount", report.fluidity.frameCount)
                num("measuredFps", report.fluidity.measuredFps)
                num("captureJitterMs", report.fluidity.captureJitterMs)
                num("motionFrames", report.fluidity.motionFrames)
                num("stallCount", report.fluidity.stallCount)
                num("longestStallMs", report.fluidity.longestStallMs)
                num("smoothness", report.fluidity.smoothness)
            }

            val av = report.avSync
            if (av == null) {
                nul("avSync")
            } else {
                obj("avSync") {
                    num("offsetMs", av.offsetMs)
                    num("correlation", av.correlation)
                    num("confidence", av.confidence)
                    num("bins", av.bins)
                }
            }

            arr("steps", report.stepResults) { step ->
                str("name", step.name)
                str("action", step.actionDescription)
                num("startMs", step.startMs)
                num("endMs", step.endMs)
                bool("passed", step.passed)
                arr("expectations", step.expectationResults) { e ->
                    str("description", e.description)
                    bool("passed", e.passed)
                    str("detail", e.detail)
                }
                arr("artifacts", step.artifacts) { a ->
                    str("type", a.type.name)
                    num("timestampMs", a.timestampMs)
                    num("frameIndex", a.frameIndex)
                    num("severity", a.severity)
                    num("confidence", a.confidence)
                    str("detail", a.detail)
                }
            }

            arr("artifacts", report.artifacts) { a ->
                str("type", a.type.name)
                num("timestampMs", a.timestampMs)
                num("frameIndex", a.frameIndex)
                num("severity", a.severity)
                num("confidence", a.confidence)
                str("detail", a.detail)
            }
        }
    }
}
