package com.example.constitution.ui

import androidx.compose.foundation.layout.padding
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.MenuBook
import androidx.compose.material.icons.filled.AccountBalance
import androidx.compose.material.icons.filled.Home
import androidx.compose.material.icons.filled.Quiz
import androidx.compose.material.icons.filled.Style
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.NavigationBar
import androidx.compose.material3.NavigationBarItem
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBar
import androidx.compose.material3.TopAppBarDefaults
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.vector.ImageVector

enum class Screen(val label: String, val icon: ImageVector) {
    Overview("Overview", Icons.Filled.Home),
    Structure("Structure", Icons.Filled.AccountBalance),
    Articles("Articles", Icons.AutoMirrored.Filled.MenuBook),
    Flashcards("Cards", Icons.Filled.Style),
    Quiz("Quiz", Icons.Filled.Quiz),
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun ConstitutionApp() {
    var current by rememberSaveable { mutableStateOf(Screen.Overview) }

    Scaffold(
        topBar = {
            TopAppBar(
                title = {
                    Text(
                        when (current) {
                            Screen.Overview -> "Samvidhaan — Learn the Constitution"
                            Screen.Structure -> "Structure of the Constitution"
                            Screen.Articles -> "Articles Explained"
                            Screen.Flashcards -> "Flashcards"
                            Screen.Quiz -> "Knowledge Check"
                        }
                    )
                },
                colors = TopAppBarDefaults.topAppBarColors(),
            )
        },
        bottomBar = {
            NavigationBar {
                Screen.entries.forEach { screen ->
                    NavigationBarItem(
                        selected = current == screen,
                        onClick = { current = screen },
                        icon = { Icon(screen.icon, contentDescription = screen.label) },
                        label = { Text(screen.label) },
                    )
                }
            }
        },
    ) { innerPadding ->
        val contentModifier = Modifier.padding(innerPadding)
        when (current) {
            Screen.Overview -> OverviewScreen(contentModifier) { current = it }
            Screen.Structure -> StructureScreen(contentModifier)
            Screen.Articles -> ArticlesScreen(contentModifier)
            Screen.Flashcards -> FlashcardsScreen(contentModifier)
            Screen.Quiz -> QuizScreen(contentModifier)
        }
    }
}
