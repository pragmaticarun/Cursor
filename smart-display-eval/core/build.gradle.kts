plugins {
    kotlin("jvm")
    application
}

application {
    // Headless demo of the whole eval pipeline on a synthetic Fire TV session.
    mainClass.set("com.smartdisplayeval.core.demo.DemoCliKt")
}

dependencies {
    testImplementation(kotlin("test"))
    testImplementation("org.junit.jupiter:junit-jupiter:5.10.2")
    testRuntimeOnly("org.junit.platform:junit-platform-launcher")
}

kotlin {
    jvmToolchain(17)
}

tasks.test {
    useJUnitPlatform()
    testLogging { showStandardStreams = true }
}
