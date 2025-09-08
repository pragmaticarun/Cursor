#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * C String Functions Tutorial
 * Demonstrates string manipulation, comparison, and utility functions
 */

void demonstrate_string_length(void) {
    printf("=== String Length ===\n");
    
    const char* str1 = "Hello, World!";
    const char* str2 = "C Programming";
    char str3[] = "Short";
    
    printf("strlen('%s') = %zu\n", str1, strlen(str1));
    printf("strlen('%s') = %zu\n", str2, strlen(str2));
    printf("strlen('%s') = %zu\n", str3, strlen(str3));
    
    // Empty string
    const char* empty = "";
    printf("strlen('') = %zu\n", strlen(empty));
}

void demonstrate_string_copying(void) {
    printf("\n=== String Copying ===\n");
    
    char dest[50];
    const char* src = "Hello, World!";
    
    // strcpy - copy string
    strcpy(dest, src);
    printf("strcpy result: '%s'\n", dest);
    
    // strncpy - copy with length limit
    char dest2[20];
    strncpy(dest2, src, sizeof(dest2) - 1);
    dest2[sizeof(dest2) - 1] = '\0';  // Ensure null termination
    printf("strncpy result: '%s'\n", dest2);
    
    // Note: strdup is POSIX, not standard C
    // For standard C, we would use malloc + strcpy
    char* duplicate = malloc(strlen(src) + 1);
    if (duplicate != NULL) {
        strcpy(duplicate, src);
        printf("duplicate result: '%s'\n", duplicate);
        free(duplicate);
    }
}

void demonstrate_string_concatenation(void) {
    printf("\n=== String Concatenation ===\n");
    
    char str1[50] = "Hello";
    const char* str2 = ", World!";
    const char* str3 = " Welcome to C!";
    
    printf("Before strcat: '%s'\n", str1);
    
    // strcat - concatenate strings
    strcat(str1, str2);
    printf("After strcat: '%s'\n", str1);
    
    // strncat - concatenate with length limit
    strncat(str1, str3, 10);  // Only first 10 characters
    printf("After strncat: '%s'\n", str1);
}

void demonstrate_string_comparison(void) {
    printf("\n=== String Comparison ===\n");
    
    const char* str1 = "apple";
    const char* str2 = "banana";
    const char* str3 = "apple";
    const char* str4 = "Apple";
    
    // strcmp - compare strings
    printf("strcmp('%s', '%s') = %d\n", str1, str2, strcmp(str1, str2));
    printf("strcmp('%s', '%s') = %d\n", str1, str3, strcmp(str1, str3));
    printf("strcmp('%s', '%s') = %d\n", str1, str4, strcmp(str1, str4));
    
    // strncmp - compare with length limit
    printf("strncmp('%s', '%s', 3) = %d\n", str1, str2, strncmp(str1, str2, 3));
    
    // Case-insensitive comparison (POSIX function, not standard C)
    printf("Case comparison: '%s' vs '%s'\n", str1, str4);
}

void demonstrate_string_searching(void) {
    printf("\n=== String Searching ===\n");
    
    const char* text = "Hello, World! Welcome to C Programming!";
    const char* substring = "World";
    char character = 'o';
    
    // strstr - find substring
    char* found = strstr(text, substring);
    if (found != NULL) {
        printf("Found '%s' at position: %ld\n", substring, found - text);
    } else {
        printf("'%s' not found\n", substring);
    }
    
    // strchr - find character
    char* char_found = strchr(text, character);
    if (char_found != NULL) {
        printf("Found '%c' at position: %ld\n", character, char_found - text);
    } else {
        printf("'%c' not found\n", character);
    }
    
    // strrchr - find last occurrence of character
    char* last_found = strrchr(text, character);
    if (last_found != NULL) {
        printf("Last occurrence of '%c' at position: %ld\n", character, last_found - text);
    }
    
    // strpbrk - find any character from set
    const char* charset = "aeiou";
    char* vowel_found = strpbrk(text, charset);
    if (vowel_found != NULL) {
        printf("First vowel '%c' at position: %ld\n", *vowel_found, vowel_found - text);
    }
}

void demonstrate_string_tokenization(void) {
    printf("\n=== String Tokenization ===\n");
    
    char text[] = "apple,banana,cherry,date";
    char* token;
    
    printf("Original string: '%s'\n", text);
    printf("Tokens:\n");
    
    // strtok - tokenize string
    token = strtok(text, ",");
    int count = 1;
    while (token != NULL) {
        printf("  %d: '%s'\n", count++, token);
        token = strtok(NULL, ",");
    }
    
    // Note: strtok_r is POSIX, not standard C
    // For standard C, we use strtok (not reentrant)
    char text2[] = "one two three four five";
    printf("\nTokenization of '%s' (using strtok):\n", text2);
    
    token = strtok(text2, " ");
    count = 1;
    while (token != NULL) {
        printf("  %d: '%s'\n", count++, token);
        token = strtok(NULL, " ");
    }
}

void demonstrate_character_functions(void) {
    printf("\n=== Character Functions ===\n");
    
    char ch = 'A';
    char digit = '5';
    char space = ' ';
    char punct = '!';
    
    // Character classification
    printf("Character classification:\n");
    printf("  '%c' isalpha: %d\n", ch, isalpha(ch));
    printf("  '%c' isdigit: %d\n", digit, isdigit(digit));
    printf("  '%c' isspace: %d\n", space, isspace(space));
    printf("  '%c' ispunct: %d\n", punct, ispunct(punct));
    printf("  '%c' isalnum: %d\n", ch, isalnum(ch));
    printf("  '%c' islower: %d\n", ch, islower(ch));
    printf("  '%c' isupper: %d\n", ch, isupper(ch));
    
    // Character conversion
    printf("\nCharacter conversion:\n");
    printf("  tolower('%c') = '%c'\n", ch, tolower(ch));
    printf("  toupper('%c') = '%c'\n", tolower(ch), toupper(tolower(ch)));
}

void demonstrate_string_transformation(void) {
    printf("\n=== String Transformation ===\n");
    
    char text[] = "Hello, World!";
    printf("Original: '%s'\n", text);
    
    // Convert to uppercase
    for (int i = 0; text[i] != '\0'; i++) {
        text[i] = toupper(text[i]);
    }
    printf("Uppercase: '%s'\n", text);
    
    // Convert to lowercase
    for (int i = 0; text[i] != '\0'; i++) {
        text[i] = tolower(text[i]);
    }
    printf("Lowercase: '%s'\n", text);
    
    // Remove punctuation
    char clean_text[100];
    int j = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalnum(text[i]) || isspace(text[i])) {
            clean_text[j++] = text[i];
        }
    }
    clean_text[j] = '\0';
    printf("Without punctuation: '%s'\n", clean_text);
}

void demonstrate_string_utilities(void) {
    printf("\n=== String Utilities ===\n");
    
    // strspn - length of initial segment
    const char* str = "12345abc";
    const char* digits = "0123456789";
    size_t span = strspn(str, digits);
    printf("strspn('%s', '%s') = %zu\n", str, digits, span);
    
    // strcspn - length of initial segment not in set
    const char* not_digits = "abcdefghijklmnopqrstuvwxyz";
    size_t cspan = strcspn(str, not_digits);
    printf("strcspn('%s', '%s') = %zu\n", str, not_digits, cspan);
    
    // strstr with multiple occurrences
    const char* text = "the quick brown fox jumps over the lazy dog";
    const char* word = "the";
    const char* pos = text;
    int count = 0;
    
    printf("Finding all occurrences of '%s' in '%s':\n", word, text);
    while ((pos = strstr(pos, word)) != NULL) {
        count++;
        printf("  Occurrence %d at position: %ld\n", count, pos - text);
        pos += strlen(word);
    }
    printf("Total occurrences: %d\n", count);
}

void demonstrate_memory_functions(void) {
    printf("\n=== Memory Functions ===\n");
    
    char str1[] = "Hello, World!";
    char str2[20];
    char str3[20];
    
    // memcpy - copy memory
    memcpy(str2, str1, strlen(str1) + 1);
    printf("memcpy result: '%s'\n", str2);
    
    // memmove - move memory (handles overlapping)
    memmove(str3, str1, strlen(str1) + 1);
    printf("memmove result: '%s'\n", str3);
    
    // memcmp - compare memory
    int cmp_result = memcmp(str1, str2, strlen(str1));
    printf("memcmp(str1, str2): %d\n", cmp_result);
    
    // memset - set memory
    char buffer[20];
    memset(buffer, 'X', sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    printf("memset result: '%s'\n", buffer);
}

int main(void) {
    printf("C String Functions Tutorial\n");
    printf("===========================\n");
    
    demonstrate_string_length();
    demonstrate_string_copying();
    demonstrate_string_concatenation();
    demonstrate_string_comparison();
    demonstrate_string_searching();
    demonstrate_string_tokenization();
    demonstrate_character_functions();
    demonstrate_string_transformation();
    demonstrate_string_utilities();
    demonstrate_memory_functions();
    
    return 0;
}

