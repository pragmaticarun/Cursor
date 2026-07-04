package com.example.constitution.ui

import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.itemsIndexed
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.LinearProgressIndicator
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedCard
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import com.example.constitution.ConstitutionData

@Composable
fun QuizScreen(modifier: Modifier = Modifier) {
    var questions by remember { mutableStateOf(ConstitutionData.quiz.shuffled()) }
    var index by remember { mutableIntStateOf(0) }
    var score by remember { mutableIntStateOf(0) }
    var selected by remember { mutableStateOf<Int?>(null) }
    var finished by remember { mutableStateOf(false) }

    fun restart() {
        questions = ConstitutionData.quiz.shuffled()
        index = 0
        score = 0
        selected = null
        finished = false
    }

    if (finished) {
        QuizResult(score, questions.size, ::restart, modifier)
        return
    }

    val question = questions[index]

    LazyColumn(
        modifier = modifier
            .fillMaxSize()
            .padding(16.dp),
        verticalArrangement = Arrangement.spacedBy(10.dp),
    ) {
        item {
            Row(
                Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween,
            ) {
                Text(
                    "Question ${index + 1} of ${questions.size}",
                    style = MaterialTheme.typography.labelLarge,
                    color = MaterialTheme.colorScheme.onSurfaceVariant,
                )
                Text(
                    "Score: $score",
                    style = MaterialTheme.typography.labelLarge,
                    fontWeight = FontWeight.Bold,
                )
            }
        }
        item {
            LinearProgressIndicator(
                progress = { (index).toFloat() / questions.size },
                modifier = Modifier.fillMaxWidth(),
            )
        }
        item {
            Text(
                question.question,
                style = MaterialTheme.typography.titleLarge,
                fontWeight = FontWeight.Bold,
                modifier = Modifier.padding(vertical = 4.dp),
            )
        }

        itemsIndexed(question.options) { i, option ->
            OptionRow(
                index = i,
                text = option,
                selected = selected,
                answer = question.answer,
                onClick = {
                    if (selected == null) {
                        selected = i
                        if (i == question.answer) score++
                    }
                },
            )
        }

        if (selected != null) {
            item {
                Card(
                    colors = CardDefaults.cardColors(containerColor = MaterialTheme.colorScheme.surfaceVariant),
                    shape = RoundedCornerShape(12.dp),
                ) {
                    Text(
                        (if (selected == question.answer) "Correct! " else "Not quite. ") + question.explain,
                        modifier = Modifier.padding(14.dp),
                        style = MaterialTheme.typography.bodyMedium,
                    )
                }
            }
            item {
                Button(
                    onClick = {
                        if (index < questions.size - 1) {
                            index++
                            selected = null
                        } else {
                            finished = true
                        }
                    },
                    modifier = Modifier.fillMaxWidth(),
                ) {
                    Text(if (index < questions.size - 1) "Next Question" else "See Results")
                }
            }
        }
    }
}

@Composable
private fun OptionRow(
    index: Int,
    text: String,
    selected: Int?,
    answer: Int,
    onClick: () -> Unit,
) {
    val letters = listOf("A", "B", "C", "D", "E")
    val isAnswered = selected != null
    val isCorrect = index == answer
    val isChosenWrong = selected == index && index != answer

    val border = when {
        isAnswered && isCorrect -> BorderStroke(2.dp, MaterialTheme.colorScheme.tertiary)
        isChosenWrong -> BorderStroke(2.dp, MaterialTheme.colorScheme.error)
        else -> BorderStroke(1.dp, MaterialTheme.colorScheme.outlineVariant)
    }
    val container = when {
        isAnswered && isCorrect -> MaterialTheme.colorScheme.tertiaryContainer
        isChosenWrong -> MaterialTheme.colorScheme.errorContainer
        else -> MaterialTheme.colorScheme.surface
    }

    OutlinedCard(
        onClick = onClick,
        enabled = !isAnswered,
        border = border,
        colors = CardDefaults.outlinedCardColors(containerColor = container),
        shape = RoundedCornerShape(12.dp),
        modifier = Modifier.fillMaxWidth(),
    ) {
        Row(
            Modifier.padding(14.dp),
            verticalAlignment = Alignment.CenterVertically,
            horizontalArrangement = Arrangement.spacedBy(12.dp),
        ) {
            Text(
                letters.getOrElse(index) { "?" },
                style = MaterialTheme.typography.titleMedium,
                fontWeight = FontWeight.Bold,
                color = MaterialTheme.colorScheme.primary,
            )
            Text(text, style = MaterialTheme.typography.bodyLarge)
        }
    }
}

@Composable
private fun QuizResult(score: Int, total: Int, onRestart: () -> Unit, modifier: Modifier = Modifier) {
    val pct = if (total == 0) 0 else (score * 100) / total
    val message = when {
        pct == 100 -> "Outstanding! You have an excellent grasp of the Constitution."
        pct >= 70 -> "Great job! A solid understanding — review the missed ones to perfect it."
        pct >= 40 -> "Good start. Revisit the Articles and Flashcards, then try again."
        else -> "Keep learning! Explore the Structure and Articles sections and come back."
    }
    Column(
        modifier = modifier
            .fillMaxSize()
            .padding(24.dp),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Center,
    ) {
        Text("Quiz complete!", style = MaterialTheme.typography.headlineMedium, fontWeight = FontWeight.Bold)
        Spacer(Modifier.height(12.dp))
        Text(
            "$score / $total  ($pct%)",
            style = MaterialTheme.typography.displaySmall,
            fontWeight = FontWeight.Bold,
            color = MaterialTheme.colorScheme.primary,
        )
        Spacer(Modifier.height(12.dp))
        Text(message, style = MaterialTheme.typography.bodyLarge, modifier = Modifier.padding(horizontal = 8.dp))
        Spacer(Modifier.height(24.dp))
        Button(onClick = onRestart) { Text("Try Again") }
    }
}
