#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * C Standard I/O Tutorial
 * Demonstrates file operations, formatted I/O, and stream handling
 */

void demonstrate_formatted_output(void) {
    printf("=== Formatted Output ===\n");
    
    int age = 25;
    float height = 5.9f;
    char name[] = "John";
    
    // Basic formatting
    printf("Name: %s, Age: %d, Height: %.1f\n", name, age, height);
    
    // Integer formatting
    int number = 42;
    printf("Decimal: %d\n", number);
    printf("Octal: %o\n", number);
    printf("Hexadecimal: %x\n", number);
    printf("Hexadecimal (uppercase): %X\n", number);
    
    // Floating point formatting
    double pi = 3.14159265359;
    printf("Pi: %.2f\n", pi);
    printf("Pi (scientific): %.2e\n", pi);
    printf("Pi (auto format): %g\n", pi);
    
    // Field width and precision
    printf("Field width 10: %10d\n", number);
    printf("Left aligned: %-10d\n", number);
    printf("Zero padded: %010d\n", number);
    
    // String formatting
    printf("String with width: %15s\n", name);
    printf("String left aligned: %-15s\n", name);
    printf("String truncated: %.2s\n", name);
}

void demonstrate_formatted_input(void) {
    printf("\n=== Formatted Input ===\n");
    
    // Note: In a real program, you would use scanf, but for demo purposes
    // we'll show the format strings and simulate input
    
    printf("Common scanf format specifiers:\n");
    printf("  %%d - integer\n");
    printf("  %%f - float\n");
    printf("  %%lf - double\n");
    printf("  %%c - character\n");
    printf("  %%s - string\n");
    printf("  %%x - hexadecimal\n");
    printf("  %%o - octal\n");
    
    // Example format strings
    printf("\nExample scanf calls:\n");
    printf("  scanf(\"%%d\", &age);\n");
    printf("  scanf(\"%%f\", &height);\n");
    printf("  scanf(\"%%s\", name);\n");
    printf("  scanf(\"%%d %%f\", &age, &height);\n");
}

void demonstrate_file_operations(void) {
    printf("\n=== File Operations ===\n");
    
    const char* filename = "test_file.txt";
    FILE* file;
    
    // Write to file
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing\n");
        return;
    }
    
    fprintf(file, "Hello, World!\n");
    fprintf(file, "This is a test file.\n");
    fprintf(file, "Number: %d\n", 42);
    fclose(file);
    printf("File written successfully\n");
    
    // Read from file
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading\n");
        return;
    }
    
    char buffer[256];
    printf("File contents:\n");
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("  %s", buffer);
    }
    fclose(file);
    
    // Clean up
    remove(filename);
    printf("File deleted\n");
}

void demonstrate_stream_operations(void) {
    printf("\n=== Stream Operations ===\n");
    
    // Standard streams
    printf("Standard streams:\n");
    printf("  stdin - standard input\n");
    printf("  stdout - standard output\n");
    printf("  stderr - standard error\n");
    
    // Redirecting output to stderr
    fprintf(stderr, "This goes to stderr\n");
    
    // Character I/O
    printf("Character I/O functions:\n");
    printf("  putchar() - output single character\n");
    printf("  getchar() - input single character\n");
    printf("  fputc() - output character to stream\n");
    printf("  fgetc() - input character from stream\n");
    
    // String I/O
    printf("String I/O functions:\n");
    printf("  puts() - output string with newline\n");
    printf("  fputs() - output string to stream\n");
    printf("  fgets() - input string from stream\n");
}

void demonstrate_binary_operations(void) {
    printf("\n=== Binary File Operations ===\n");
    
    const char* filename = "binary_test.bin";
    FILE* file;
    
    // Write binary data
    file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file for binary writing\n");
        return;
    }
    
    int numbers[] = {1, 2, 3, 4, 5};
    size_t written = fwrite(numbers, sizeof(int), 5, file);
    printf("Written %zu integers to binary file\n", written);
    fclose(file);
    
    // Read binary data
    file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file for binary reading\n");
        return;
    }
    
    int read_numbers[5];
    size_t read_count = fread(read_numbers, sizeof(int), 5, file);
    printf("Read %zu integers from binary file: ", read_count);
    for (size_t i = 0; i < read_count; i++) {
        printf("%d ", read_numbers[i]);
    }
    printf("\n");
    fclose(file);
    
    // Clean up
    remove(filename);
}

void demonstrate_file_positioning(void) {
    printf("\n=== File Positioning ===\n");
    
    const char* filename = "position_test.txt";
    FILE* file;
    
    // Create test file
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error creating test file\n");
        return;
    }
    
    fprintf(file, "Line 1\nLine 2\nLine 3\nLine 4\nLine 5\n");
    fclose(file);
    
    // Read with positioning
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading\n");
        return;
    }
    
    char buffer[256];
    
    // Read first line
    char* result = fgets(buffer, sizeof(buffer), file);
    if (result != NULL) {
        printf("First line: %s", buffer);
    }
    
    // Get current position
    long position = ftell(file);
    printf("Current position: %ld\n", position);
    
    // Seek to beginning
    fseek(file, 0, SEEK_SET);
    printf("After seeking to beginning:\n");
    
    // Read all lines
    int line_num = 1;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("  Line %d: %s", line_num++, buffer);
    }
    
    fclose(file);
    remove(filename);
}

void demonstrate_error_handling(void) {
    printf("\n=== Error Handling ===\n");
    
    // Check for errors
    printf("Error checking functions:\n");
    printf("  feof() - check for end-of-file\n");
    printf("  ferror() - check for stream error\n");
    printf("  perror() - print error message\n");
    printf("  clearerr() - clear error flags\n");
    
    // Example error handling
    FILE* file = fopen("nonexistent.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        printf("File does not exist\n");
    } else {
        fclose(file);
    }
}

void demonstrate_temporary_files(void) {
    printf("\n=== Temporary Files ===\n");
    
    // Create temporary file
    FILE* temp_file = tmpfile();
    if (temp_file == NULL) {
        printf("Error creating temporary file\n");
        return;
    }
    
    // Write to temporary file
    fprintf(temp_file, "This is a temporary file\n");
    fprintf(temp_file, "It will be automatically deleted\n");
    
    // Read from temporary file
    fseek(temp_file, 0, SEEK_SET);
    char buffer[256];
    printf("Temporary file contents:\n");
    while (fgets(buffer, sizeof(buffer), temp_file) != NULL) {
        printf("  %s", buffer);
    }
    
    // File is automatically deleted when closed
    fclose(temp_file);
    printf("Temporary file automatically deleted\n");
}

int main(void) {
    printf("C Standard I/O Tutorial\n");
    printf("=======================\n");
    
    demonstrate_formatted_output();
    demonstrate_formatted_input();
    demonstrate_file_operations();
    demonstrate_stream_operations();
    demonstrate_binary_operations();
    demonstrate_file_positioning();
    demonstrate_error_handling();
    demonstrate_temporary_files();
    
    return 0;
}

