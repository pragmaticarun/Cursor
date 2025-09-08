#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

/**
 * Comprehensive C Programming Example
 * Demonstrates integration of multiple C concepts
 */

// Structure to represent a student
typedef struct {
    char name[50];
    int age;
    float gpa;
    int* grades;
    int num_grades;
} Student;

// Function prototypes
Student* create_student(const char* name, int age);
void add_grade(Student* student, int grade);
float calculate_average(const Student* student);
void print_student(const Student* student);
void free_student(Student* student);
int compare_students(const void* a, const void* b);
void demonstrate_data_structures(void);
void demonstrate_algorithms(void);
void demonstrate_file_operations(void);

// Create a new student
Student* create_student(const char* name, int age) {
    Student* student = malloc(sizeof(Student));
    if (student == NULL) {
        return NULL;
    }
    
    strncpy(student->name, name, sizeof(student->name) - 1);
    student->name[sizeof(student->name) - 1] = '\0';
    student->age = age;
    student->gpa = 0.0f;
    student->grades = NULL;
    student->num_grades = 0;
    
    return student;
}

// Add a grade to student's record
void add_grade(Student* student, int grade) {
    if (student == NULL || grade < 0 || grade > 100) {
        return;
    }
    
    student->num_grades++;
    student->grades = realloc(student->grades, 
                             student->num_grades * sizeof(int));
    
    if (student->grades != NULL) {
        student->grades[student->num_grades - 1] = grade;
        student->gpa = calculate_average(student);
    }
}

// Calculate average grade
float calculate_average(const Student* student) {
    if (student == NULL || student->num_grades == 0) {
        return 0.0f;
    }
    
    int sum = 0;
    for (int i = 0; i < student->num_grades; i++) {
        sum += student->grades[i];
    }
    
    return (float)sum / student->num_grades;
}

// Print student information
void print_student(const Student* student) {
    if (student == NULL) {
        printf("Invalid student\n");
        return;
    }
    
    printf("Name: %s\n", student->name);
    printf("Age: %d\n", student->age);
    printf("GPA: %.2f\n", student->gpa);
    printf("Grades: ");
    
    for (int i = 0; i < student->num_grades; i++) {
        printf("%d", student->grades[i]);
        if (i < student->num_grades - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

// Free student memory
void free_student(Student* student) {
    if (student != NULL) {
        free(student->grades);
        free(student);
    }
}

// Comparison function for sorting students by GPA
int compare_students(const void* a, const void* b) {
    const Student* student_a = *(const Student**)a;
    const Student* student_b = *(const Student**)b;
    
    if (student_a->gpa > student_b->gpa) return -1;
    if (student_a->gpa < student_b->gpa) return 1;
    return 0;
}

// Demonstrate data structures and memory management
void demonstrate_data_structures(void) {
    printf("=== Data Structures and Memory Management ===\n");
    
    // Create students
    Student* students[3];
    students[0] = create_student("Alice Johnson", 20);
    students[1] = create_student("Bob Smith", 19);
    students[2] = create_student("Carol Davis", 21);
    
    // Add grades
    add_grade(students[0], 85);
    add_grade(students[0], 92);
    add_grade(students[0], 78);
    
    add_grade(students[1], 95);
    add_grade(students[1], 88);
    add_grade(students[1], 91);
    
    add_grade(students[2], 76);
    add_grade(students[2], 82);
    add_grade(students[2], 79);
    
    // Print all students
    printf("\nAll students:\n");
    for (int i = 0; i < 3; i++) {
        print_student(students[i]);
        printf("\n");
    }
    
    // Sort students by GPA
    qsort(students, 3, sizeof(Student*), compare_students);
    
    printf("Students sorted by GPA (highest first):\n");
    for (int i = 0; i < 3; i++) {
        print_student(students[i]);
        printf("\n");
    }
    
    // Free memory
    for (int i = 0; i < 3; i++) {
        free_student(students[i]);
    }
}

// Demonstrate algorithms and control flow
void demonstrate_algorithms(void) {
    printf("=== Algorithms and Control Flow ===\n");
    
    // Generate random numbers
    srand((unsigned int)time(NULL));
    int numbers[10];
    
    printf("Generated numbers: ");
    for (int i = 0; i < 10; i++) {
        numbers[i] = rand() % 100;
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    // Bubble sort
    for (int i = 0; i < 10 - 1; i++) {
        for (int j = 0; j < 10 - i - 1; j++) {
            if (numbers[j] > numbers[j + 1]) {
                int temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }
    
    printf("Sorted numbers: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    // Binary search
    int target = numbers[5];  // Search for middle element
    int left = 0, right = 9;
    bool found = false;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (numbers[mid] == target) {
            printf("Found %d at index %d\n", target, mid);
            found = true;
            break;
        } else if (numbers[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    if (!found) {
        printf("Target %d not found\n", target);
    }
}

// Demonstrate file operations
void demonstrate_file_operations(void) {
    printf("\n=== File Operations ===\n");
    
    const char* filename = "student_data.txt";
    FILE* file;
    
    // Write data to file
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing\n");
        return;
    }
    
    fprintf(file, "Student Grade Report\n");
    fprintf(file, "===================\n");
    fprintf(file, "Generated: %s", ctime(&(time_t){time(NULL)}));
    fprintf(file, "\n");
    
    // Write some sample data
    fprintf(file, "Alice Johnson, 20, 85.0\n");
    fprintf(file, "Bob Smith, 19, 91.3\n");
    fprintf(file, "Carol Davis, 21, 79.0\n");
    
    fclose(file);
    printf("Data written to %s\n", filename);
    
    // Read data from file
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading\n");
        return;
    }
    
    char line[256];
    printf("\nFile contents:\n");
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
    
    fclose(file);
    
    // Clean up
    remove(filename);
    printf("\nFile deleted\n");
}

// Demonstrate string processing
void demonstrate_string_processing(void) {
    printf("\n=== String Processing ===\n");
    
    char text[] = "  Hello, World! Welcome to C Programming!  ";
    printf("Original: '%s'\n", text);
    
    // Remove leading and trailing whitespace
    char* start = text;
    char* end = text + strlen(text) - 1;
    
    // Skip leading whitespace
    while (*start == ' ' || *start == '\t') {
        start++;
    }
    
    // Skip trailing whitespace
    while (end > start && (*end == ' ' || *end == '\t')) {
        end--;
    }
    
    *(end + 1) = '\0';
    printf("Trimmed: '%s'\n", start);
    
    // Convert to uppercase
    for (char* p = start; *p; p++) {
        if (*p >= 'a' && *p <= 'z') {
            *p = *p - 'a' + 'A';
        }
    }
    printf("Uppercase: '%s'\n", start);
    
    // Count words
    int word_count = 0;
    bool in_word = false;
    
    for (char* p = start; *p; p++) {
        if (*p != ' ' && *p != '\t' && *p != '\n') {
            if (!in_word) {
                word_count++;
                in_word = true;
            }
        } else {
            in_word = false;
        }
    }
    
    printf("Word count: %d\n", word_count);
}

int main(void) {
    printf("C Programming Comprehensive Example\n");
    printf("===================================\n\n");
    
    demonstrate_data_structures();
    demonstrate_algorithms();
    demonstrate_file_operations();
    demonstrate_string_processing();
    
    printf("\n=== Program Complete ===\n");
    printf("This example demonstrates:\n");
    printf("- Dynamic memory allocation and management\n");
    printf("- Data structures (structs, arrays)\n");
    printf("- Algorithms (sorting, searching)\n");
    printf("- File I/O operations\n");
    printf("- String processing\n");
    printf("- Error handling\n");
    printf("- Memory cleanup\n");
    
    return 0;
}

