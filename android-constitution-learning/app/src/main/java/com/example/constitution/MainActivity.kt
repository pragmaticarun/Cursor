package com.example.constitution

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import com.example.constitution.ui.ConstitutionApp
import com.example.constitution.ui.theme.ConstitutionTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        enableEdgeToEdge()
        super.onCreate(savedInstanceState)
        setContent {
            ConstitutionTheme {
                ConstitutionApp()
            }
        }
    }
}
