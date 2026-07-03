package com.smartdisplayeval.core

/** Standalone entrypoint: runs the verification suite and exits non-zero on failure. */
fun main() {
    val failures = Verification.runAll()
    if (failures > 0) {
        System.err.println("VERIFICATION FAILED: $failures check(s) failed")
        kotlin.system.exitProcess(1)
    }
    println("VERIFICATION PASSED")
}
