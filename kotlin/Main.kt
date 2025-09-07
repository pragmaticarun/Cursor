import java.nio.file.Files
import java.nio.file.Path
import java.time.Instant

data class User(val id: Int, val name: String)

fun wordCount(s: String): Map<String, Int> =
    s.lowercase().split(" ", "\n", "\t").filter { it.isNotBlank() }
        .groupingBy { it }.eachCount()

fun main() {
    println("=== Kotlin demo ===")

    val users = listOf(User(1, "Ada"), User(2, "Alan"))
    println(users.joinToString("\n") { "${it.id},${it.name}" })

    println(wordCount("Hello hello world"))

    val here = Path.of(".")
    Files.list(here).limit(3).forEach { println("- $it") }

    println("Now: ${Instant.now()}")
}