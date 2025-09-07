fun main() {
    val seq = generateSequence(1) { it + 1 }.take(5).map { it * it }
    println(seq.toList())
}