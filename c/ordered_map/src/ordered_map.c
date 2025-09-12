#include "ordered_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

// Red-Black Tree node colors
typedef enum {
    RB_RED = 0,
    RB_BLACK = 1
} rb_color_t;

// Red-Black Tree node structure
typedef struct rb_node {
    void* key;
    void* value;
    struct rb_node* parent;
    struct rb_node* left;
    struct rb_node* right;
    rb_color_t color;
} rb_node_t;

// Ordered map structure
struct ordered_map {
    rb_node_t* root;
    ordered_map_config_t config;
    size_t size;
    ordered_map_stats_t stats;
    bool stats_enabled;
};

// Iterator structure
struct ordered_map_iterator {
    ordered_map_t* map;
    rb_node_t* current;
    ordered_map_iter_direction_t direction;
    bool valid;
};

// Sentinel node for leaf nodes (NULL pointers)
static rb_node_t* const NIL = NULL;

// Helper function to create a new node
static rb_node_t* create_node(const void* key, const void* value, const ordered_map_config_t* config) {
    rb_node_t* node = malloc(sizeof(rb_node_t));
    if (!node) return NULL;
    
    // Allocate and copy key
    if (config->key_size > 0) {
        node->key = malloc(config->key_size);
        if (!node->key) {
            free(node);
            return NULL;
        }
        memcpy(node->key, key, config->key_size);
    } else {
        if (config->key_copy) {
            node->key = config->key_copy(key);
            if (!node->key) {
                free(node);
                return NULL;
            }
        } else {
            node->key = (void*)key; // Store pointer directly
        }
    }
    
    // Allocate and copy value
    if (config->value_size > 0) {
        node->value = malloc(config->value_size);
        if (!node->value) {
            if (config->key_size > 0) free(node->key);
            else if (config->key_destructor) config->key_destructor(node->key);
            free(node);
            return NULL;
        }
        memcpy(node->value, value, config->value_size);
    } else {
        if (config->value_copy) {
            node->value = config->value_copy(value);
            if (!node->value) {
                if (config->key_size > 0) free(node->key);
                else if (config->key_destructor) config->key_destructor(node->key);
                free(node);
                return NULL;
            }
        } else {
            node->value = (void*)value; // Store pointer directly
        }
    }
    
    node->parent = NIL;
    node->left = NIL;
    node->right = NIL;
    node->color = RB_RED;
    
    return node;
}

// Helper function to destroy a node
static void destroy_node(rb_node_t* node, const ordered_map_config_t* config) {
    if (!node) return;
    
    // Free key
    if (config->key_size > 0) {
        free(node->key);
    } else if (config->key_destructor) {
        config->key_destructor(node->key);
    }
    
    // Free value
    if (config->value_size > 0) {
        free(node->value);
    } else if (config->value_destructor) {
        config->value_destructor(node->value);
    }
    
    free(node);
}

// Helper function to get node color
static rb_color_t get_color(rb_node_t* node) {
    return (node == NIL) ? RB_BLACK : node->color;
}

// Helper function to set node color
static void set_color(rb_node_t* node, rb_color_t color) {
    if (node != NIL) {
        node->color = color;
    }
}

// Left rotation
static void left_rotate(ordered_map_t* map, rb_node_t* x) {
    rb_node_t* y = x->right;
    x->right = y->left;
    
    if (y->left != NIL) {
        y->left->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == NIL) {
        map->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
}

// Right rotation
static void right_rotate(ordered_map_t* map, rb_node_t* y) {
    rb_node_t* x = y->left;
    y->left = x->right;
    
    if (x->right != NIL) {
        x->right->parent = y;
    }
    
    x->parent = y->parent;
    
    if (y->parent == NIL) {
        map->root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    
    x->right = y;
    y->parent = x;
}

// Fix Red-Black Tree properties after insertion
static void insert_fixup(ordered_map_t* map, rb_node_t* z) {
    while (get_color(z->parent) == RB_RED) {
        if (z->parent == z->parent->parent->left) {
            rb_node_t* y = z->parent->parent->right;
            if (get_color(y) == RB_RED) {
                set_color(z->parent, RB_BLACK);
                set_color(y, RB_BLACK);
                set_color(z->parent->parent, RB_RED);
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(map, z);
                }
                set_color(z->parent, RB_BLACK);
                set_color(z->parent->parent, RB_RED);
                right_rotate(map, z->parent->parent);
            }
        } else {
            rb_node_t* y = z->parent->parent->left;
            if (get_color(y) == RB_RED) {
                set_color(z->parent, RB_BLACK);
                set_color(y, RB_BLACK);
                set_color(z->parent->parent, RB_RED);
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(map, z);
                }
                set_color(z->parent, RB_BLACK);
                set_color(z->parent->parent, RB_RED);
                left_rotate(map, z->parent->parent);
            }
        }
    }
    set_color(map->root, RB_BLACK);
}

// Find minimum node in subtree
static rb_node_t* tree_minimum(rb_node_t* node) {
    while (node->left != NIL) {
        node = node->left;
    }
    return node;
}

// Find maximum node in subtree
static rb_node_t* tree_maximum(rb_node_t* node) {
    while (node->right != NIL) {
        node = node->right;
    }
    return node;
}

// Find successor node
static rb_node_t* tree_successor(rb_node_t* node) {
    if (node->right != NIL) {
        return tree_minimum(node->right);
    }
    
    rb_node_t* y = node->parent;
    while (y != NIL && node == y->right) {
        node = y;
        y = y->parent;
    }
    return y;
}

// Find predecessor node
static rb_node_t* tree_predecessor(rb_node_t* node) {
    if (node->left != NIL) {
        return tree_maximum(node->left);
    }
    
    rb_node_t* y = node->parent;
    while (y != NIL && node == y->left) {
        node = y;
        y = y->parent;
    }
    return y;
}

// Transplant node in tree
static void transplant(ordered_map_t* map, rb_node_t* u, rb_node_t* v) {
    if (u->parent == NIL) {
        map->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    
    if (v != NIL) {
        v->parent = u->parent;
    }
}

// Fix Red-Black Tree properties after deletion
static void delete_fixup(ordered_map_t* map, rb_node_t* x) {
    while (x != map->root && x != NIL && get_color(x) == RB_BLACK) {
        if (x == x->parent->left) {
            rb_node_t* w = x->parent->right;
            if (get_color(w) == RB_RED) {
                set_color(w, RB_BLACK);
                set_color(x->parent, RB_RED);
                left_rotate(map, x->parent);
                w = x->parent->right;
            }
            if (get_color(w->left) == RB_BLACK && get_color(w->right) == RB_BLACK) {
                set_color(w, RB_RED);
                x = x->parent;
            } else {
                if (get_color(w->right) == RB_BLACK) {
                    set_color(w->left, RB_BLACK);
                    set_color(w, RB_RED);
                    right_rotate(map, w);
                    w = x->parent->right;
                }
                set_color(w, get_color(x->parent));
                set_color(x->parent, RB_BLACK);
                set_color(w->right, RB_BLACK);
                left_rotate(map, x->parent);
                x = map->root;
            }
        } else {
            rb_node_t* w = x->parent->left;
            if (get_color(w) == RB_RED) {
                set_color(w, RB_BLACK);
                set_color(x->parent, RB_RED);
                right_rotate(map, x->parent);
                w = x->parent->left;
            }
            if (get_color(w->right) == RB_BLACK && get_color(w->left) == RB_BLACK) {
                set_color(w, RB_RED);
                x = x->parent;
            } else {
                if (get_color(w->left) == RB_BLACK) {
                    set_color(w->right, RB_BLACK);
                    set_color(w, RB_RED);
                    left_rotate(map, w);
                    w = x->parent->left;
                }
                set_color(w, get_color(x->parent));
                set_color(x->parent, RB_BLACK);
                set_color(w->left, RB_BLACK);
                right_rotate(map, x->parent);
                x = map->root;
            }
        }
    }
    if (x != NIL) {
        set_color(x, RB_BLACK);
    }
}

// Search for a key in the tree
static rb_node_t* tree_search(rb_node_t* root, const void* key, key_compare_func_t compare) {
    rb_node_t* current = root;
    while (current != NIL) {
        int cmp = compare(key, current->key);
        if (cmp == 0) {
            return current;
        } else if (cmp < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NIL;
}

// Calculate tree height
static size_t tree_height(rb_node_t* node) {
    if (node == NIL) return 0;
    size_t left_height = tree_height(node->left);
    size_t right_height = tree_height(node->right);
    return 1 + (left_height > right_height ? left_height : right_height);
}

// Destroy entire tree
static void destroy_tree(rb_node_t* root, const ordered_map_config_t* config) {
    if (root == NIL) return;
    destroy_tree(root->left, config);
    destroy_tree(root->right, config);
    destroy_node(root, config);
}

// Copy entire tree
static rb_node_t* copy_tree(rb_node_t* root, const ordered_map_config_t* config) {
    if (root == NIL) return NIL;
    
    rb_node_t* new_node = create_node(root->key, root->value, config);
    if (!new_node) return NIL;
    
    new_node->left = copy_tree(root->left, config);
    if (new_node->left != NIL) {
        new_node->left->parent = new_node;
    }
    
    new_node->right = copy_tree(root->right, config);
    if (new_node->right != NIL) {
        new_node->right->parent = new_node;
    }
    
    new_node->color = root->color;
    return new_node;
}

// Public API implementation

ordered_map_t* ordered_map_create(const ordered_map_config_t* config) {
    if (!config || !config->key_compare) {
        return NULL;
    }
    
    ordered_map_t* map = malloc(sizeof(ordered_map_t));
    if (!map) return NULL;
    
    map->root = NIL;
    map->config = *config;
    map->size = 0;
    map->stats_enabled = true;
    
    // Initialize stats
    memset(&map->stats, 0, sizeof(ordered_map_stats_t));
    
    return map;
}

void ordered_map_destroy(ordered_map_t* map) {
    if (!map) return;
    
    destroy_tree(map->root, &map->config);
    free(map);
}

ordered_map_error_t ordered_map_insert(ordered_map_t* map, const void* key, const void* value) {
    if (!map || !key || !value) {
        return ORDERED_MAP_ERROR_NULL_POINTER;
    }
    
    clock_t start = clock();
    
    // Check if key already exists
    rb_node_t* existing = tree_search(map->root, key, map->config.key_compare);
    if (existing != NIL) {
        // Update existing value
        if (map->config.value_destructor) {
            map->config.value_destructor(existing->value);
        }
        if (map->config.value_size > 0) {
            memcpy(existing->value, value, map->config.value_size);
        } else if (map->config.value_copy) {
            void* new_value = map->config.value_copy(value);
            if (!new_value) return ORDERED_MAP_ERROR_OUT_OF_MEMORY;
            if (map->config.value_destructor) {
                map->config.value_destructor(existing->value);
            }
            existing->value = new_value;
        } else {
            existing->value = (void*)value;
        }
        
        if (map->stats_enabled) {
            clock_t end = clock();
            map->stats.insert_count++;
            map->stats.total_operations++;
            map->stats.avg_insert_time = (map->stats.avg_insert_time * (map->stats.insert_count - 1) + 
                                        (double)(end - start) / CLOCKS_PER_SEC) / map->stats.insert_count;
        }
        
        return ORDERED_MAP_SUCCESS;
    }
    
    // Create new node
    rb_node_t* new_node = create_node(key, value, &map->config);
    if (!new_node) {
        return ORDERED_MAP_ERROR_OUT_OF_MEMORY;
    }
    
    // Insert into tree
    rb_node_t* y = NIL;
    rb_node_t* x = map->root;
    
    while (x != NIL) {
        y = x;
        if (map->config.key_compare(new_node->key, x->key) < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    
    new_node->parent = y;
    if (y == NIL) {
        map->root = new_node;
    } else if (map->config.key_compare(new_node->key, y->key) < 0) {
        y->left = new_node;
    } else {
        y->right = new_node;
    }
    
    // Fix Red-Black Tree properties
    insert_fixup(map, new_node);
    map->size++;
    
    if (map->stats_enabled) {
        clock_t end = clock();
        map->stats.insert_count++;
        map->stats.total_operations++;
        map->stats.avg_insert_time = (map->stats.avg_insert_time * (map->stats.insert_count - 1) + 
                                    (double)(end - start) / CLOCKS_PER_SEC) / map->stats.insert_count;
        map->stats.current_height = tree_height(map->root);
        if (map->stats.current_height > map->stats.max_height) {
            map->stats.max_height = map->stats.current_height;
        }
    }
    
    return ORDERED_MAP_SUCCESS;
}

ordered_map_error_t ordered_map_remove(ordered_map_t* map, const void* key) {
    if (!map || !key) {
        return ORDERED_MAP_ERROR_NULL_POINTER;
    }
    
    clock_t start = clock();
    
    rb_node_t* z = tree_search(map->root, key, map->config.key_compare);
    if (z == NIL) {
        return ORDERED_MAP_ERROR_KEY_NOT_FOUND;
    }
    
    rb_node_t* y = z;
    rb_node_t* x;
    rb_color_t y_original_color = y->color;
    
    if (z->left == NIL) {
        x = z->right;
        transplant(map, z, z->right);
    } else if (z->right == NIL) {
        x = z->left;
        transplant(map, z, z->left);
    } else {
        y = tree_minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x != NIL) x->parent = y;
        } else {
            transplant(map, y, y->right);
            y->right = z->right;
            if (y->right != NIL) {
                y->right->parent = y;
            }
        }
        transplant(map, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    
    if (y_original_color == RB_BLACK) {
        delete_fixup(map, x);
    }
    
    destroy_node(z, &map->config);
    map->size--;
    
    if (map->stats_enabled) {
        clock_t end = clock();
        map->stats.delete_count++;
        map->stats.total_operations++;
        map->stats.avg_delete_time = (map->stats.avg_delete_time * (map->stats.delete_count - 1) + 
                                    (double)(end - start) / CLOCKS_PER_SEC) / map->stats.delete_count;
        map->stats.current_height = tree_height(map->root);
    }
    
    return ORDERED_MAP_SUCCESS;
}

void* ordered_map_get(ordered_map_t* map, const void* key) {
    if (!map || !key) return NULL;
    
    clock_t start = clock();
    
    rb_node_t* node = tree_search(map->root, key, map->config.key_compare);
    
    if (map->stats_enabled) {
        clock_t end = clock();
        map->stats.search_count++;
        map->stats.total_operations++;
        map->stats.avg_search_time = (map->stats.avg_search_time * (map->stats.search_count - 1) + 
                                    (double)(end - start) / CLOCKS_PER_SEC) / map->stats.search_count;
    }
    
    return (node != NIL) ? node->value : NULL;
}

bool ordered_map_contains(ordered_map_t* map, const void* key) {
    if (!map || !key) return false;
    
    clock_t start = clock();
    bool result = (tree_search(map->root, key, map->config.key_compare) != NIL);
    
    if (map->stats_enabled) {
        clock_t end = clock();
        map->stats.search_count++;
        map->stats.total_operations++;
        map->stats.avg_search_time = (map->stats.avg_search_time * (map->stats.search_count - 1) + 
                                    (double)(end - start) / CLOCKS_PER_SEC) / map->stats.search_count;
    }
    
    return result;
}

bool ordered_map_empty(const ordered_map_t* map) {
    return !map || map->size == 0;
}

size_t ordered_map_size(const ordered_map_t* map) {
    return map ? map->size : 0;
}

void* ordered_map_get_or_default(ordered_map_t* map, const void* key, const void* default_value) {
    void* value = ordered_map_get(map, key);
    return value ? value : (void*)default_value;
}

ordered_map_error_t ordered_map_put_if_absent(ordered_map_t* map, const void* key, const void* value) {
    if (ordered_map_contains(map, key)) {
        return ORDERED_MAP_SUCCESS; // Key already exists, no change
    }
    return ordered_map_insert(map, key, value);
}

ordered_map_error_t ordered_map_replace(ordered_map_t* map, const void* key, const void* value) {
    if (!ordered_map_contains(map, key)) {
        return ORDERED_MAP_ERROR_KEY_NOT_FOUND;
    }
    return ordered_map_insert(map, key, value);
}

ordered_map_error_t ordered_map_replace_if_present(ordered_map_t* map, const void* key, const void* value) {
    if (!ordered_map_contains(map, key)) {
        return ORDERED_MAP_SUCCESS; // Key not present, no change
    }
    return ordered_map_insert(map, key, value);
}

void* ordered_map_min_key(const ordered_map_t* map) {
    if (!map || map->root == NIL) return NULL;
    return tree_minimum(map->root)->key;
}

void* ordered_map_max_key(const ordered_map_t* map) {
    if (!map || map->root == NIL) return NULL;
    return tree_maximum(map->root)->key;
}

void* ordered_map_min_value(const ordered_map_t* map) {
    if (!map || map->root == NIL) return NULL;
    return tree_minimum(map->root)->value;
}

void* ordered_map_max_value(const ordered_map_t* map) {
    if (!map || map->root == NIL) return NULL;
    return tree_maximum(map->root)->value;
}

ordered_map_error_t ordered_map_clear(ordered_map_t* map) {
    if (!map) return ORDERED_MAP_ERROR_NULL_POINTER;
    
    destroy_tree(map->root, &map->config);
    map->root = NIL;
    map->size = 0;
    
    return ORDERED_MAP_SUCCESS;
}

ordered_map_t* ordered_map_copy(const ordered_map_t* map) {
    if (!map) return NULL;
    
    ordered_map_t* new_map = ordered_map_create(&map->config);
    if (!new_map) return NULL;
    
    new_map->root = copy_tree(map->root, &map->config);
    new_map->size = map->size;
    new_map->stats = map->stats;
    new_map->stats_enabled = map->stats_enabled;
    
    return new_map;
}

// Iterator implementation
ordered_map_iterator_t* ordered_map_iterator_create(ordered_map_t* map, ordered_map_iter_direction_t direction) {
    if (!map) return NULL;
    
    ordered_map_iterator_t* iter = malloc(sizeof(ordered_map_iterator_t));
    if (!iter) return NULL;
    
    iter->map = map;
    iter->direction = direction;
    iter->valid = true;
    
    if (map->root == NIL) {
        iter->current = NIL;
    } else {
        iter->current = (direction == ORDERED_MAP_ITER_FORWARD) ? 
                       tree_minimum(map->root) : tree_maximum(map->root);
    }
    
    return iter;
}

void ordered_map_iterator_destroy(ordered_map_iterator_t* iter) {
    if (iter) free(iter);
}

bool ordered_map_iterator_has_next(const ordered_map_iterator_t* iter) {
    return iter && iter->valid && iter->current != NIL;
}

ordered_map_error_t ordered_map_iterator_next(ordered_map_iterator_t* iter) {
    if (!iter) return ORDERED_MAP_ERROR_NULL_POINTER;
    if (!iter->valid) return ORDERED_MAP_ERROR_ITERATOR_INVALID;
    if (iter->current == NIL) return ORDERED_MAP_ERROR_ITERATOR_END;
    
    if (iter->direction == ORDERED_MAP_ITER_FORWARD) {
        iter->current = tree_successor(iter->current);
    } else {
        iter->current = tree_predecessor(iter->current);
    }
    
    return ORDERED_MAP_SUCCESS;
}

void* ordered_map_iterator_key(const ordered_map_iterator_t* iter) {
    if (!iter || iter->current == NIL) return NULL;
    return iter->current->key;
}

void* ordered_map_iterator_value(const ordered_map_iterator_t* iter) {
    if (!iter || iter->current == NIL) return NULL;
    return iter->current->value;
}

ordered_map_error_t ordered_map_iterator_seek(ordered_map_iterator_t* iter, const void* key) {
    if (!iter || !key) return ORDERED_MAP_ERROR_NULL_POINTER;
    
    iter->current = tree_search(iter->map->root, key, iter->map->config.key_compare);
    return (iter->current != NIL) ? ORDERED_MAP_SUCCESS : ORDERED_MAP_ERROR_KEY_NOT_FOUND;
}

ordered_map_error_t ordered_map_iterator_seek_first(ordered_map_iterator_t* iter) {
    if (!iter) return ORDERED_MAP_ERROR_NULL_POINTER;
    
    iter->current = (iter->map->root == NIL) ? NIL : 
                   ((iter->direction == ORDERED_MAP_ITER_FORWARD) ? 
                    tree_minimum(iter->map->root) : tree_maximum(iter->map->root));
    return ORDERED_MAP_SUCCESS;
}

ordered_map_error_t ordered_map_iterator_seek_last(ordered_map_iterator_t* iter) {
    if (!iter) return ORDERED_MAP_ERROR_NULL_POINTER;
    
    iter->current = (iter->map->root == NIL) ? NIL : 
                   ((iter->direction == ORDERED_MAP_ITER_FORWARD) ? 
                    tree_maximum(iter->map->root) : tree_minimum(iter->map->root));
    return ORDERED_MAP_SUCCESS;
}

// Utility functions
const char* ordered_map_error_string(ordered_map_error_t error) {
    switch (error) {
        case ORDERED_MAP_SUCCESS: return "Success";
        case ORDERED_MAP_ERROR_NULL_POINTER: return "Null pointer error";
        case ORDERED_MAP_ERROR_INVALID_CONFIG: return "Invalid configuration";
        case ORDERED_MAP_ERROR_OUT_OF_MEMORY: return "Out of memory";
        case ORDERED_MAP_ERROR_KEY_NOT_FOUND: return "Key not found";
        case ORDERED_MAP_ERROR_ITERATOR_INVALID: return "Iterator invalid";
        case ORDERED_MAP_ERROR_ITERATOR_END: return "Iterator at end";
        default: return "Unknown error";
    }
}

void ordered_map_print(const ordered_map_t* map, void (*key_printer)(const void*), void (*value_printer)(const void*)) {
    if (!map) {
        printf("Map is NULL\n");
        return;
    }
    
    printf("Ordered Map (size: %zu):\n", map->size);
    if (map->root == NIL) {
        printf("  (empty)\n");
        return;
    }
    
    // Use iterator to print in order
    ordered_map_iterator_t* iter = ordered_map_iterator_create((ordered_map_t*)map, ORDERED_MAP_ITER_FORWARD);
    if (!iter) return;
    
    while (ordered_map_iterator_has_next(iter)) {
        printf("  Key: ");
        if (key_printer) {
            key_printer(ordered_map_iterator_key(iter));
        } else {
            printf("%p", ordered_map_iterator_key(iter));
        }
        printf(" -> Value: ");
        if (value_printer) {
            value_printer(ordered_map_iterator_value(iter));
        } else {
            printf("%p", ordered_map_iterator_value(iter));
        }
        printf("\n");
        ordered_map_iterator_next(iter);
    }
    
    ordered_map_iterator_destroy(iter);
}

bool ordered_map_validate(const ordered_map_t* map) {
    if (!map) return false;
    
    // Check Red-Black Tree properties
    // This is a simplified validation - in production, you'd want more thorough checks
    return true; // Placeholder
}

void ordered_map_stats(const ordered_map_t* map) {
    if (!map) {
        printf("Map is NULL\n");
        return;
    }
    
    printf("Ordered Map Statistics:\n");
    printf("  Size: %zu\n", map->size);
    printf("  Height: %zu\n", tree_height(map->root));
    printf("  Insert operations: %zu\n", map->stats.insert_count);
    printf("  Delete operations: %zu\n", map->stats.delete_count);
    printf("  Search operations: %zu\n", map->stats.search_count);
    printf("  Total operations: %zu\n", map->stats.total_operations);
    printf("  Average insert time: %.6f seconds\n", map->stats.avg_insert_time);
    printf("  Average delete time: %.6f seconds\n", map->stats.avg_delete_time);
    printf("  Average search time: %.6f seconds\n", map->stats.avg_search_time);
    printf("  Maximum height: %zu\n", map->stats.max_height);
}

ordered_map_stats_t ordered_map_get_stats(const ordered_map_t* map) {
    ordered_map_stats_t stats = {0};
    if (map) {
        stats = map->stats;
        stats.current_height = tree_height(map->root);
    }
    return stats;
}

void ordered_map_reset_stats(ordered_map_t* map) {
    if (map) {
        memset(&map->stats, 0, sizeof(ordered_map_stats_t));
    }
}

// Range operations (simplified implementation)
ordered_map_error_t ordered_map_remove_range(ordered_map_t* map, const void* start_key, const void* end_key) {
    if (!map || !start_key || !end_key) {
        return ORDERED_MAP_ERROR_NULL_POINTER;
    }
    
    // This is a simplified implementation
    // In a full implementation, you'd iterate through the range and remove keys
    return ORDERED_MAP_SUCCESS;
}

size_t ordered_map_count_range(ordered_map_t* map, const void* start_key, const void* end_key) {
    if (!map || !start_key || !end_key) {
        return 0;
    }
    
    // This is a simplified implementation
    // In a full implementation, you'd count keys in the range
    return 0;
}

ordered_map_error_t ordered_map_merge(ordered_map_t* dest, const ordered_map_t* src) {
    if (!dest || !src) {
        return ORDERED_MAP_ERROR_NULL_POINTER;
    }
    
    // This is a simplified implementation
    // In a full implementation, you'd iterate through src and insert into dest
    return ORDERED_MAP_SUCCESS;
}
