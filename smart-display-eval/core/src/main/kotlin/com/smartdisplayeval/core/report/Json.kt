package com.smartdisplayeval.core.report

/**
 * A tiny, dependency-free JSON writer. Enough to emit machine-readable eval
 * reports without pulling a serialization library into the core (the Android
 * app and any grading harness can parse it with their own tools).
 */
class JsonWriter {
    private val sb = StringBuilder()

    fun obj(build: Obj.() -> Unit): String {
        Obj().build()
        sb.append('}')
        return sb.toString()
    }

    inner class Obj {
        private var first = true
        init { sb.append('{') }

        private fun key(name: String) {
            if (!first) sb.append(',')
            first = false
            sb.append('"').append(escape(name)).append("\":")
        }

        fun str(name: String, value: String) { key(name); sb.append('"').append(escape(value)).append('"') }
        fun num(name: String, value: Number) { key(name); sb.append(formatNumber(value)) }
        fun bool(name: String, value: Boolean) { key(name); sb.append(if (value) "true" else "false") }
        fun nul(name: String) { key(name); sb.append("null") }

        fun obj(name: String, build: ObjBody.() -> Unit) {
            key(name)
            sb.append('{'); val nested = ObjBody(); nested.build(); sb.append('}')
        }

        fun <T> arr(name: String, items: Iterable<T>, render: ObjBody.(T) -> Unit) {
            key(name)
            sb.append('[')
            var f = true
            for (item in items) {
                if (!f) sb.append(',')
                f = false
                sb.append('{'); val nested = ObjBody(); nested.render(item); sb.append('}')
            }
            sb.append(']')
        }

        fun arrStr(name: String, items: Iterable<String>) {
            key(name)
            sb.append('[')
            var f = true
            for (item in items) {
                if (!f) sb.append(',')
                f = false
                sb.append('"').append(escape(item)).append('"')
            }
            sb.append(']')
        }
    }

    /** Body of a nested object already opened with '{'. */
    inner class ObjBody {
        private var first = true
        private fun key(name: String) {
            if (!first) sb.append(',')
            first = false
            sb.append('"').append(escape(name)).append("\":")
        }
        fun str(name: String, value: String) { key(name); sb.append('"').append(escape(value)).append('"') }
        fun num(name: String, value: Number) { key(name); sb.append(formatNumber(value)) }
        fun bool(name: String, value: Boolean) { key(name); sb.append(if (value) "true" else "false") }
        fun nul(name: String) { key(name); sb.append("null") }
        fun obj(name: String, build: ObjBody.() -> Unit) {
            key(name); sb.append('{'); val nested = ObjBody(); nested.build(); sb.append('}')
        }
        fun <T> arr(name: String, items: Iterable<T>, render: ObjBody.(T) -> Unit) {
            key(name); sb.append('[')
            var f = true
            for (item in items) {
                if (!f) sb.append(',')
                f = false
                sb.append('{'); val nested = ObjBody(); nested.render(item); sb.append('}')
            }
            sb.append(']')
        }
        fun arrStr(name: String, items: Iterable<String>) {
            key(name); sb.append('[')
            var f = true
            for (item in items) {
                if (!f) sb.append(',')
                f = false
                sb.append('"').append(escape(item)).append('"')
            }
            sb.append(']')
        }
    }

    companion object {
        private fun formatNumber(value: Number): String {
            return when (value) {
                is Double -> {
                    if (value.isNaN() || value.isInfinite()) "0"
                    else if (value == value.toLong().toDouble()) value.toLong().toString()
                    else String.format(java.util.Locale.ROOT, "%.4f", value)
                }
                is Float -> formatNumber(value.toDouble())
                else -> value.toString()
            }
        }

        private fun escape(s: String): String {
            val out = StringBuilder(s.length + 8)
            for (c in s) {
                when (c) {
                    '"' -> out.append("\\\"")
                    '\\' -> out.append("\\\\")
                    '\n' -> out.append("\\n")
                    '\r' -> out.append("\\r")
                    '\t' -> out.append("\\t")
                    else -> if (c < ' ') out.append("\\u%04x".format(c.code)) else out.append(c)
                }
            }
            return out.toString()
        }
    }
}
