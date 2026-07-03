package com.smartdisplayeval.core

import kotlin.test.Test
import kotlin.test.assertEquals

/**
 * JUnit entrypoint for the analysis verification suite so it runs under
 * `./gradlew :core:test`. The checks themselves live in [Verification] (which
 * is also runnable standalone via [main] in `TestRunner.kt`).
 */
class CoreVerificationTest {
    @Test
    fun `all analysis checks pass`() {
        assertEquals(0, Verification.runAll(), "one or more analysis verification checks failed")
    }
}
