#include "../include/test_framework.h"
#include <string.h>
#include <ctype.h>

// Test functions for string operations
bool test_string_length(void) {
    const char* str1 = "Hello";
    const char* str2 = "World!";
    const char* empty = "";
    
    TEST_ASSERT_EQUAL(5, (int)strlen(str1));
    TEST_ASSERT_EQUAL(6, (int)strlen(str2));
    TEST_ASSERT_EQUAL(0, (int)strlen(empty));
    
    return true;
}

bool test_string_copying(void) {
    char dest[20];
    const char* src = "Hello, World!";
    
    // Test strcpy
    strcpy(dest, src);
    TEST_ASSERT_EQUAL_STRING(src, dest);
    
    // Test strncpy
    char dest2[10];
    strncpy(dest2, src, sizeof(dest2) - 1);
    dest2[sizeof(dest2) - 1] = '\0';
    TEST_ASSERT_EQUAL_STRING("Hello, Wo", dest2);
    
    return true;
}

bool test_string_concatenation(void) {
    char str[50] = "Hello";
    const char* append = ", World!";
    
    strcat(str, append);
    TEST_ASSERT_EQUAL_STRING("Hello, World!", str);
    
    return true;
}

bool test_string_comparison(void) {
    const char* str1 = "apple";
    const char* str2 = "banana";
    const char* str3 = "apple";
    
    TEST_ASSERT(strcmp(str1, str2) < 0);
    TEST_ASSERT(strcmp(str1, str3) == 0);
    TEST_ASSERT(strcmp(str2, str1) > 0);
    
    // Test strncmp
    TEST_ASSERT(strncmp(str1, str2, 3) < 0);
    TEST_ASSERT(strncmp(str1, "apples", 3) == 0);
    
    return true;
}

bool test_string_searching(void) {
    const char* text = "Hello, World!";
    
    // Test strstr
    char* found = strstr(text, "World");
    TEST_ASSERT_NOT_NULL(found);
    TEST_ASSERT_EQUAL(7, (int)(found - text));
    
    // Test strchr
    char* char_found = strchr(text, 'o');
    TEST_ASSERT_NOT_NULL(char_found);
    TEST_ASSERT_EQUAL(4, (int)(char_found - text));
    
    // Test strrchr
    char* last_found = strrchr(text, 'o');
    TEST_ASSERT_NOT_NULL(last_found);
    TEST_ASSERT_EQUAL(8, (int)(last_found - text));
    
    return true;
}

bool test_character_functions(void) {
    char ch = 'A';
    char digit = '5';
    char space = ' ';
    char punct = '!';
    
    TEST_ASSERT_TRUE(isalpha(ch));
    TEST_ASSERT_TRUE(isdigit(digit));
    TEST_ASSERT_TRUE(isspace(space));
    TEST_ASSERT_TRUE(ispunct(punct));
    TEST_ASSERT_TRUE(isalnum(ch));
    TEST_ASSERT_FALSE(islower(ch));
    TEST_ASSERT_TRUE(isupper(ch));
    
    // Test character conversion
    TEST_ASSERT_EQUAL('a', tolower(ch));
    TEST_ASSERT_EQUAL('A', toupper('a'));
    
    return true;
}

bool test_memory_functions(void) {
    char str1[] = "Hello, World!";
    char str2[20];
    char str3[20];
    
    // Test memcpy
    memcpy(str2, str1, strlen(str1) + 1);
    TEST_ASSERT_EQUAL_STRING(str1, str2);
    
    // Test memmove
    memmove(str3, str1, strlen(str1) + 1);
    TEST_ASSERT_EQUAL_STRING(str1, str3);
    
    // Test memcmp
    TEST_ASSERT_EQUAL(0, memcmp(str1, str2, strlen(str1)));
    
    // Test memset
    char buffer[10];
    memset(buffer, 'X', sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    TEST_ASSERT_EQUAL_STRING("XXXXXXXXX", buffer);
    
    return true;
}

// Test suite
test_function_t string_tests[] = {
    test_string_length,
    test_string_copying,
    test_string_concatenation,
    test_string_comparison,
    test_string_searching,
    test_character_functions,
    test_memory_functions
};

test_suite_t string_test_suite = {
    "Strings",
    string_tests,
    sizeof(string_tests) / sizeof(string_tests[0])
};

int main(void) {
    printf("C Strings Test Suite\n");
    printf("====================\n");
    
    run_test_suite(&string_test_suite);
    print_test_summary(tests_passed, tests_passed + tests_failed);
    
    return (tests_failed == 0) ? 0 : 1;
}


