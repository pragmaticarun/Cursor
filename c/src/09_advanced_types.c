#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * C Advanced Types Tutorial
 * Demonstrates structs, unions, enums, bitfields, and advanced type concepts
 */

// Enumeration example
enum color {
    RED = 1,
    GREEN = 2,
    BLUE = 4,
    YELLOW = RED | GREEN,
    CYAN = GREEN | BLUE,
    MAGENTA = RED | BLUE,
    WHITE = RED | GREEN | BLUE
};

// Struct with various data types
typedef struct {
    char name[50];
    int age;
    float height;
    bool is_student;
    enum color favorite_color;
} Person;

// Struct with bitfields
typedef struct {
    unsigned int is_active : 1;      // 1 bit
    unsigned int priority : 3;       // 3 bits
    unsigned int type : 4;           // 4 bits
    unsigned int reserved : 24;      // 24 bits (padding)
} StatusFlags;

// Union for different data representations
typedef union {
    uint32_t as_int;
    float as_float;
    struct {
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
        uint8_t byte4;
    } as_bytes;
} DataUnion;

// Linked list node using struct
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Function prototypes
void demonstrate_structs(void);
void demonstrate_unions(void);
void demonstrate_enums(void);
void demonstrate_bitfields(void);
void demonstrate_linked_list(void);
void demonstrate_advanced_memory(void);
void demonstrate_type_punning(void);
void demonstrate_alignment(void);

void demonstrate_structs(void) {
    printf("=== Structs ===\n");
    
    // Create and initialize struct
    Person person1 = {
        .name = "Alice Johnson",
        .age = 25,
        .height = 5.6f,
        .is_student = true,
        .favorite_color = BLUE
    };
    
    // Access struct members
    printf("Person 1:\n");
    printf("  Name: %s\n", person1.name);
    printf("  Age: %d\n", person1.age);
    printf("  Height: %.1f\n", person1.height);
    printf("  Is Student: %s\n", person1.is_student ? "Yes" : "No");
    printf("  Favorite Color: %d\n", person1.favorite_color);
    
    // Modify struct members
    person1.age = 26;
    strcpy(person1.name, "Alice Smith");
    
    printf("\nAfter modification:\n");
    printf("  Name: %s\n", person1.name);
    printf("  Age: %d\n", person1.age);
    
    // Struct assignment
    Person person2 = person1;
    printf("\nPerson 2 (copy of Person 1):\n");
    printf("  Name: %s\n", person2.name);
    printf("  Age: %d\n", person2.age);
    
    // Struct size and alignment
    printf("\nStruct information:\n");
    printf("  Size of Person: %zu bytes\n", sizeof(Person));
    printf("  Size of name: %zu bytes\n", sizeof(person1.name));
    printf("  Size of age: %zu bytes\n", sizeof(person1.age));
    printf("  Size of height: %zu bytes\n", sizeof(person1.height));
    printf("  Size of is_student: %zu bytes\n", sizeof(person1.is_student));
    printf("  Size of favorite_color: %zu bytes\n", sizeof(person1.favorite_color));
}

void demonstrate_unions(void) {
    printf("\n=== Unions ===\n");
    
    DataUnion data;
    
    // Set as integer
    data.as_int = 0x3F800000;  // IEEE 754 representation of 1.0f
    printf("As integer: 0x%08X\n", data.as_int);
    printf("As float: %.6f\n", data.as_float);
    printf("As bytes: 0x%02X 0x%02X 0x%02X 0x%02X\n", 
           data.as_bytes.byte1, data.as_bytes.byte2, 
           data.as_bytes.byte3, data.as_bytes.byte4);
    
    // Set as float
    data.as_float = 3.14159f;
    printf("\nAfter setting as float (3.14159):\n");
    printf("As integer: 0x%08X\n", data.as_int);
    printf("As float: %.6f\n", data.as_float);
    printf("As bytes: 0x%02X 0x%02X 0x%02X 0x%02X\n", 
           data.as_bytes.byte1, data.as_bytes.byte2, 
           data.as_bytes.byte3, data.as_bytes.byte4);
    
    // Union size
    printf("\nUnion size: %zu bytes\n", sizeof(DataUnion));
    printf("Size of uint32_t: %zu bytes\n", sizeof(uint32_t));
    printf("Size of float: %zu bytes\n", sizeof(float));
}

void demonstrate_enums(void) {
    printf("\n=== Enums ===\n");
    
    enum color c1 = RED;
    enum color c2 = GREEN;
    enum color c3 = YELLOW;
    
    printf("Red: %d\n", c1);
    printf("Green: %d\n", c2);
    printf("Yellow (RED | GREEN): %d\n", c3);
    printf("Blue: %d\n", BLUE);
    printf("White (all colors): %d\n", WHITE);
    
    // Enum in switch statement
    printf("\nColor analysis:\n");
    switch (c3) {
        case RED:
            printf("Primary color: Red\n");
            break;
        case GREEN:
            printf("Primary color: Green\n");
            break;
        case BLUE:
            printf("Primary color: Blue\n");
            break;
        case YELLOW:
            printf("Secondary color: Yellow (Red + Green)\n");
            break;
        case CYAN:
            printf("Secondary color: Cyan (Green + Blue)\n");
            break;
        case MAGENTA:
            printf("Secondary color: Magenta (Red + Blue)\n");
            break;
        case WHITE:
            printf("All colors combined: White\n");
            break;
        default:
            printf("Unknown color\n");
            break;
    }
    
    // Enum size
    printf("\nEnum size: %zu bytes\n", sizeof(enum color));
}

void demonstrate_bitfields(void) {
    printf("\n=== Bitfields ===\n");
    
    StatusFlags flags = {0};
    
    // Set bitfield values
    flags.is_active = 1;
    flags.priority = 5;  // Max value for 3 bits is 7
    flags.type = 12;     // Max value for 4 bits is 15
    
    printf("Status Flags:\n");
    printf("  Is Active: %u\n", flags.is_active);
    printf("  Priority: %u\n", flags.priority);
    printf("  Type: %u\n", flags.type);
    printf("  Reserved: %u\n", flags.reserved);
    
    // Bitfield size
    printf("\nBitfield size: %zu bytes\n", sizeof(StatusFlags));
    
    // Demonstrate bit operations
    printf("\nBit operations:\n");
    printf("  Max priority (3 bits): %u\n", (1U << 3) - 1);
    printf("  Max type (4 bits): %u\n", (1U << 4) - 1);
    
    // Test bitfield limits
    flags.priority = 8;  // This will be truncated to 0 (8 & 7 = 0)
    printf("  Priority set to 8, actual value: %u\n", flags.priority);
}

void demonstrate_linked_list(void) {
    printf("\n=== Linked List ===\n");
    
    // Create linked list
    Node* head = NULL;
    Node* current = NULL;
    
    // Add nodes
    for (int i = 1; i <= 5; i++) {
        Node* new_node = malloc(sizeof(Node));
        if (new_node == NULL) {
            printf("Memory allocation failed\n");
            return;
        }
        
        new_node->data = i * 10;
        new_node->next = NULL;
        
        if (head == NULL) {
            head = new_node;
            current = new_node;
        } else {
            current->next = new_node;
            current = new_node;
        }
    }
    
    // Traverse and print list
    printf("Linked list contents:\n");
    current = head;
    int position = 1;
    while (current != NULL) {
        printf("  Node %d: %d\n", position++, current->data);
        current = current->next;
    }
    
    // Search for value
    int search_value = 30;
    current = head;
    position = 1;
    while (current != NULL) {
        if (current->data == search_value) {
            printf("Found value %d at position %d\n", search_value, position);
            break;
        }
        current = current->next;
        position++;
    }
    
    // Free memory
    current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    printf("Linked list freed\n");
}

void demonstrate_advanced_memory(void) {
    printf("\n=== Advanced Memory Management ===\n");
    
    // Allocate aligned memory
    void* aligned_ptr = aligned_alloc(16, 64);
    if (aligned_ptr == NULL) {
        printf("Aligned allocation failed\n");
        return;
    }
    
    printf("Aligned memory allocated at: %p\n", aligned_ptr);
    printf("Alignment check: %s\n", 
           ((uintptr_t)aligned_ptr % 16 == 0) ? "OK" : "FAIL");
    
    // Use aligned memory
    int* int_array = (int*)aligned_ptr;
    for (int i = 0; i < 16; i++) {
        int_array[i] = i * i;
    }
    
    printf("Aligned array values: ");
    for (int i = 0; i < 16; i++) {
        printf("%d ", int_array[i]);
    }
    printf("\n");
    
    free(aligned_ptr);
    
    // Demonstrate memory pools
    printf("\nMemory pool simulation:\n");
    const size_t pool_size = 1024;
    char* memory_pool = malloc(pool_size);
    if (memory_pool == NULL) {
        printf("Pool allocation failed\n");
        return;
    }
    
    // Simple memory pool implementation
    size_t pool_offset = 0;
    
    // Allocate from pool
    int* pool_int = (int*)(memory_pool + pool_offset);
    *pool_int = 42;
    pool_offset += sizeof(int);
    
    float* pool_float = (float*)(memory_pool + pool_offset);
    *pool_float = 3.14f;
    pool_offset += sizeof(float);
    
    printf("Pool allocated values: int=%d, float=%.2f\n", *pool_int, *pool_float);
    printf("Pool usage: %zu/%zu bytes\n", pool_offset, pool_size);
    
    free(memory_pool);
}

void demonstrate_type_punning(void) {
    printf("\n=== Type Punning ===\n");
    
    // Safe type punning using union
    union {
        float f;
        uint32_t i;
    } pun;
    
    pun.f = 1.0f;
    printf("Float 1.0 as integer: 0x%08X\n", pun.i);
    
    pun.i = 0x3F800000;  // IEEE 754 representation of 1.0f
    printf("Integer 0x3F800000 as float: %.6f\n", pun.f);
    
    // Demonstrate endianness
    uint32_t value = 0x12345678;
    uint8_t* bytes = (uint8_t*)&value;
    
    printf("\nEndianness test (0x12345678):\n");
    printf("Byte order: 0x%02X 0x%02X 0x%02X 0x%02X\n", 
           bytes[0], bytes[1], bytes[2], bytes[3]);
    
    if (bytes[0] == 0x78) {
        printf("System is little-endian\n");
    } else if (bytes[0] == 0x12) {
        printf("System is big-endian\n");
    } else {
        printf("Unknown endianness\n");
    }
}

void demonstrate_alignment(void) {
    printf("\n=== Memory Alignment ===\n");
    
    // Demonstrate struct alignment
    struct {
        char c;     // 1 byte
        int i;      // 4 bytes
        char c2;    // 1 byte
    } unaligned;
    
    struct {
        char c;     // 1 byte
        char c2;    // 1 byte
        int i;      // 4 bytes
    } aligned;
    
    printf("Unaligned struct size: %zu bytes\n", sizeof(unaligned));
    printf("Aligned struct size: %zu bytes\n", sizeof(aligned));
    
    // Show alignment requirements
    printf("\nAlignment requirements:\n");
    printf("char: %zu bytes\n", _Alignof(char));
    printf("int: %zu bytes\n", _Alignof(int));
    printf("float: %zu bytes\n", _Alignof(float));
    printf("double: %zu bytes\n", _Alignof(double));
    printf("void*: %zu bytes\n", _Alignof(void*));
    
    // Demonstrate alignment in arrays
    struct {
        char c;
        int i;
    } array[3];
    
    printf("\nArray alignment:\n");
    for (int i = 0; i < 3; i++) {
        printf("Element %d at offset: %zu\n", i, 
               (char*)&array[i] - (char*)&array[0]);
    }
}

int main(void) {
    printf("C Advanced Types Tutorial\n");
    printf("=========================\n");
    
    demonstrate_structs();
    demonstrate_unions();
    demonstrate_enums();
    demonstrate_bitfields();
    demonstrate_linked_list();
    demonstrate_advanced_memory();
    demonstrate_type_punning();
    demonstrate_alignment();
    
    return 0;
}

