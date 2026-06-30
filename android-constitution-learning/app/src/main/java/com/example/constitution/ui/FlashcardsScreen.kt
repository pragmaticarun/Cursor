package com.example.constitution.ui

import androidx.compose.animation.core.animateFloatAsState
import androidx.compose.animation.core.tween
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowBack
import androidx.compose.material.icons.automirrored.filled.ArrowForward
import androidx.compose.material.icons.filled.Shuffle
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.graphicsLayer
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import com.example.constitution.ConstitutionData
import com.example.constitution.Flashcard

@Composable
fun FlashcardsScreen(modifier: Modifier = Modifier) {
    var deck by remember { mutableStateOf(ConstitutionData.flashcards) }
    var index by remember { mutableIntStateOf(0) }
    var flipped by remember { mutableStateOf(false) }

    val card = deck[index]
    val rotation by animateFloatAsState(
        targetValue = if (flipped) 180f else 0f,
        animationSpec = tween(500),
        label = "flip",
    )

    Column(
        modifier = modifier
            .fillMaxSize()
            .padding(16.dp),
        horizontalAlignment = Alignment.CenterHorizontally,
    ) {
        Text(
            "Tap the card to flip it.",
            style = MaterialTheme.typography.bodyMedium,
            color = MaterialTheme.colorScheme.onSurfaceVariant,
        )
        Spacer(Modifier.height(16.dp))

        FlipCard(
            card = card,
            rotation = rotation,
            modifier = Modifier
                .fillMaxWidth()
                .height(300.dp)
                .clickable { flipped = !flipped },
        )

        Spacer(Modifier.height(20.dp))
        Text(
            "${index + 1} / ${deck.size}",
            style = MaterialTheme.typography.titleMedium,
            fontWeight = FontWeight.Bold,
        )
        Spacer(Modifier.height(12.dp))

        Row(horizontalArrangement = Arrangement.spacedBy(8.dp)) {
            OutlinedButton(onClick = {
                flipped = false
                index = (index - 1 + deck.size) % deck.size
            }) {
                Icon(Icons.AutoMirrored.Filled.ArrowBack, contentDescription = "Previous")
                Spacer(Modifier.width(6.dp))
                Text("Prev")
            }
            OutlinedButton(onClick = {
                flipped = false
                index = (index + 1) % deck.size
            }) {
                Text("Next")
                Spacer(Modifier.width(6.dp))
                Icon(Icons.AutoMirrored.Filled.ArrowForward, contentDescription = "Next")
            }
        }
        Spacer(Modifier.height(10.dp))
        Button(onClick = {
            deck = deck.shuffled()
            index = 0
            flipped = false
        }) {
            Icon(Icons.Filled.Shuffle, contentDescription = null)
            Spacer(Modifier.width(6.dp))
            Text("Shuffle")
        }
    }
}

@Composable
private fun FlipCard(card: Flashcard, rotation: Float, modifier: Modifier = Modifier) {
    val showFront = rotation <= 90f
    Card(
        modifier = modifier.graphicsLayer {
            rotationY = rotation
            cameraDistance = 12f * density
        },
        shape = RoundedCornerShape(20.dp),
        colors = CardDefaults.cardColors(
            containerColor = if (showFront) MaterialTheme.colorScheme.primary
            else MaterialTheme.colorScheme.tertiaryContainer,
        ),
    ) {
        Box(
            Modifier
                .fillMaxSize()
                .padding(24.dp)
                .graphicsLayer { if (!showFront) rotationY = 180f },
            contentAlignment = Alignment.Center,
        ) {
            Column(horizontalAlignment = Alignment.CenterHorizontally) {
                Text(
                    if (showFront) "QUESTION" else "ANSWER",
                    style = MaterialTheme.typography.labelMedium,
                    fontWeight = FontWeight.Bold,
                    color = if (showFront) MaterialTheme.colorScheme.onPrimary
                    else MaterialTheme.colorScheme.tertiary,
                )
                Spacer(Modifier.height(14.dp))
                Text(
                    if (showFront) card.front else card.back,
                    style = MaterialTheme.typography.titleMedium,
                    textAlign = TextAlign.Center,
                    color = if (showFront) MaterialTheme.colorScheme.onPrimary
                    else MaterialTheme.colorScheme.onTertiaryContainer,
                )
            }
        }
    }
}
