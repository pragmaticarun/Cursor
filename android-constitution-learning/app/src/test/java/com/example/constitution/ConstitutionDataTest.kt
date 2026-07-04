package com.example.constitution

import org.junit.Assert.assertEquals
import org.junit.Assert.assertTrue
import org.junit.Test

class ConstitutionDataTest {

    @Test
    fun quizAnswersAreWithinBounds() {
        ConstitutionData.quiz.forEach { q ->
            assertTrue(
                "Answer index out of range for: ${q.question}",
                q.answer in q.options.indices,
            )
        }
    }

    @Test
    fun quizQuestionsHaveAtLeastTwoOptions() {
        ConstitutionData.quiz.forEach { q ->
            assertTrue("Too few options for: ${q.question}", q.options.size >= 2)
        }
    }

    @Test
    fun articleFieldsAreNotBlank() {
        ConstitutionData.articles.forEach { a ->
            assertTrue(a.number.isNotBlank())
            assertTrue(a.title.isNotBlank())
            assertTrue(a.part.isNotBlank())
            assertTrue(a.text.isNotBlank())
            assertTrue(a.explanation.isNotBlank())
        }
    }

    @Test
    fun articleNumbersAreUnique() {
        val numbers = ConstitutionData.articles.map { it.number }
        assertEquals(numbers.size, numbers.distinct().size)
    }

    @Test
    fun flashcardsHaveContent() {
        assertTrue(ConstitutionData.flashcards.isNotEmpty())
        ConstitutionData.flashcards.forEach { c ->
            assertTrue(c.front.isNotBlank())
            assertTrue(c.back.isNotBlank())
        }
    }

    @Test
    fun expectedCollectionSizes() {
        assertEquals(12, ConstitutionData.schedules.size)
        assertTrue(ConstitutionData.parts.size >= 22)
        assertTrue(ConstitutionData.articles.size >= 30)
    }
}
