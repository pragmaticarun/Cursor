use clap::{Parser, Subcommand};
use serde::{Deserialize, Serialize};
use std::fmt::Debug;

#[derive(Parser)]
#[command(name = "generic-data-structures")]
#[command(about = "Generic data structures demonstrating Rust's type system")]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    /// Demonstrate binary tree operations
    Tree {
        #[command(subcommand)]
        operation: TreeOperation,
    },
    /// Demonstrate linked list operations
    List {
        #[command(subcommand)]
        operation: ListOperation,
    },
    /// Demonstrate heap operations
    Heap {
        #[command(subcommand)]
        operation: HeapOperation,
    },
    /// Demonstrate hash map operations
    HashMap {
        #[command(subcommand)]
        operation: HashMapOperation,
    },
}

#[derive(Subcommand)]
enum TreeOperation {
    Insert { value: i32 },
    Search { value: i32 },
    Delete { value: i32 },
    Traverse,
    Size,
    Height,
}

#[derive(Subcommand)]
enum ListOperation {
    Push { value: i32 },
    Pop,
    Insert { index: usize, value: i32 },
    Remove { index: usize },
    Get { index: usize },
    Length,
    Reverse,
}

#[derive(Subcommand)]
enum HeapOperation {
    Insert { value: i32 },
    ExtractMax,
    Peek,
    Size,
    IsEmpty,
}

#[derive(Subcommand)]
enum HashMapOperation {
    Insert { key: String, value: String },
    Get { key: String },
    Remove { key: String },
    Contains { key: String },
    Size,
    Keys,
}

// Generic Binary Tree Node
#[derive(Debug, Clone, Serialize, Deserialize)]
struct TreeNode<T> {
    value: T,
    left: Option<Box<TreeNode<T>>>,
    right: Option<Box<TreeNode<T>>>,
}

impl<T: PartialOrd + Clone + Debug> TreeNode<T> {
    fn new(value: T) -> Self {
        Self {
            value,
            left: None,
            right: None,
        }
    }
}

// Generic Binary Search Tree
#[derive(Debug, Serialize, Deserialize)]
struct BinaryTree<T> {
    root: Option<Box<TreeNode<T>>>,
    size: usize,
}

impl<T: PartialOrd + Clone + Debug> BinaryTree<T> {
    fn new() -> Self {
        Self {
            root: None,
            size: 0,
        }
    }
    
    fn insert(&mut self, value: T) {
        self.root = Self::insert_recursive(self.root.take(), value);
        self.size += 1;
    }
    
    fn insert_recursive(node: Option<Box<TreeNode<T>>>, value: T) -> Option<Box<TreeNode<T>>> {
        match node {
            None => Some(Box::new(TreeNode::new(value))),
            Some(mut node) => {
                if value < node.value {
                    node.left = Self::insert_recursive(node.left.take(), value);
                } else if value > node.value {
                    node.right = Self::insert_recursive(node.right.take(), value);
                }
                Some(node)
            }
        }
    }
    
    fn search(&self, value: T) -> bool {
        Self::search_recursive(self.root.as_ref(), value)
    }
    
    fn search_recursive(node: Option<&Box<TreeNode<T>>>, value: T) -> bool {
        match node {
            None => false,
            Some(node) => {
                if value == node.value {
                    true
                } else if value < node.value {
                    Self::search_recursive(node.left.as_ref(), value)
                } else {
                    Self::search_recursive(node.right.as_ref(), value)
                }
            }
        }
    }
    
    fn inorder_traversal(&self) -> Vec<T> {
        let mut result = Vec::new();
        Self::inorder_recursive(self.root.as_ref(), &mut result);
        result
    }
    
    fn inorder_recursive(node: Option<&Box<TreeNode<T>>>, result: &mut Vec<T>) {
        if let Some(node) = node {
            Self::inorder_recursive(node.left.as_ref(), result);
            result.push(node.value.clone());
            Self::inorder_recursive(node.right.as_ref(), result);
        }
    }
    
    fn height(&self) -> usize {
        Self::height_recursive(self.root.as_ref())
    }
    
    fn height_recursive(node: Option<&Box<TreeNode<T>>>) -> usize {
        match node {
            None => 0,
            Some(node) => {
                1 + std::cmp::max(
                    Self::height_recursive(node.left.as_ref()),
                    Self::height_recursive(node.right.as_ref())
                )
            }
        }
    }
    
    fn size(&self) -> usize {
        self.size
    }
}

// Generic Linked List Node
#[derive(Debug, Clone, Serialize, Deserialize)]
struct ListNode<T> {
    value: T,
    next: Option<Box<ListNode<T>>>,
}

impl<T: Clone + Debug> ListNode<T> {
    fn new(value: T) -> Self {
        Self { value, next: None }
    }
}

// Generic Linked List
#[derive(Debug, Serialize, Deserialize)]
struct LinkedList<T> {
    head: Option<Box<ListNode<T>>>,
    length: usize,
}

impl<T: Clone + Debug> LinkedList<T> {
    fn new() -> Self {
        Self {
            head: None,
            length: 0,
        }
    }
    
    fn push(&mut self, value: T) {
        let new_node = Box::new(ListNode::new(value));
        let old_head = self.head.take();
        self.head = Some(Box::new(ListNode {
            value: new_node.value,
            next: old_head,
        }));
        self.length += 1;
    }
    
    fn pop(&mut self) -> Option<T> {
        self.head.take().map(|node| {
            self.head = node.next;
            self.length -= 1;
            node.value
        })
    }
    
    fn insert(&mut self, index: usize, value: T) -> Result<(), String> {
        if index > self.length {
            return Err("Index out of bounds".to_string());
        }
        
        if index == 0 {
            self.push(value);
            return Ok(());
        }
        
        let mut current = self.head.as_mut().unwrap();
        for _ in 0..index - 1 {
            current = current.next.as_mut().unwrap();
        }
        
        let mut new_node = Box::new(ListNode::new(value));
        let old_next = current.next.take();
        new_node.next = old_next;
        current.next = Some(new_node);
        self.length += 1;
        Ok(())
    }
    
    fn get(&self, index: usize) -> Option<&T> {
        if index >= self.length {
            return None;
        }
        
        let mut current = self.head.as_ref()?;
        for _ in 0..index {
            current = current.next.as_ref()?;
        }
        Some(&current.value)
    }
    
    fn reverse(&mut self) {
        let mut prev = None;
        let mut current = self.head.take();
        
        while let Some(mut node) = current {
            let next = node.next.take();
            node.next = prev;
            prev = Some(node);
            current = next;
        }
        
        self.head = prev;
    }
    
    fn length(&self) -> usize {
        self.length
    }
}

// Generic Max Heap
#[derive(Debug, Serialize, Deserialize)]
struct MaxHeap<T> {
    data: Vec<T>,
}

impl<T: PartialOrd + Clone + Debug> MaxHeap<T> {
    fn new() -> Self {
        Self { data: Vec::new() }
    }
    
    fn insert(&mut self, value: T) {
        self.data.push(value);
        self.heapify_up(self.data.len() - 1);
    }
    
    fn extract_max(&mut self) -> Option<T> {
        if self.data.is_empty() {
            return None;
        }
        
        let max = self.data[0].clone();
        let last = self.data.pop().unwrap();
        
        if !self.data.is_empty() {
            self.data[0] = last;
            self.heapify_down(0);
        }
        
        Some(max)
    }
    
    fn peek(&self) -> Option<&T> {
        self.data.first()
    }
    
    fn size(&self) -> usize {
        self.data.len()
    }
    
    fn is_empty(&self) -> bool {
        self.data.is_empty()
    }
    
    fn heapify_up(&mut self, index: usize) {
        if index == 0 {
            return;
        }
        
        let parent = (index - 1) / 2;
        if self.data[index] > self.data[parent] {
            self.data.swap(index, parent);
            self.heapify_up(parent);
        }
    }
    
    fn heapify_down(&mut self, index: usize) {
        let left = 2 * index + 1;
        let right = 2 * index + 2;
        let mut largest = index;
        
        if left < self.data.len() && self.data[left] > self.data[largest] {
            largest = left;
        }
        
        if right < self.data.len() && self.data[right] > self.data[largest] {
            largest = right;
        }
        
        if largest != index {
            self.data.swap(index, largest);
            self.heapify_down(largest);
        }
    }
}

// Generic HashMap
#[derive(Debug, Serialize, Deserialize)]
struct HashMap<K, V> {
    buckets: Vec<Vec<(K, V)>>,
    size: usize,
}

impl<K: PartialEq + Clone + Debug, V: Clone + Debug> HashMap<K, V> {
    fn new() -> Self {
        Self {
            buckets: vec![Vec::new(); 16],
            size: 0,
        }
    }
    
    fn insert(&mut self, key: K, value: V) {
        let bucket_index = self.hash(&key) % self.buckets.len();
        
        // Check if key already exists
        for (existing_key, existing_value) in &mut self.buckets[bucket_index] {
            if *existing_key == key {
                *existing_value = value;
                return;
            }
        }
        
        // Insert new key-value pair
        self.buckets[bucket_index].push((key, value));
        self.size += 1;
        
        // Resize if load factor is too high
        if self.size > self.buckets.len() * 2 {
            self.resize();
        }
    }
    
    fn get(&self, key: &K) -> Option<&V> {
        let bucket_index = self.hash(key) % self.buckets.len();
        
        for (existing_key, value) in &self.buckets[bucket_index] {
            if existing_key == key {
                return Some(value);
            }
        }
        
        None
    }
    
    fn remove(&mut self, key: &K) -> Option<V> {
        let bucket_index = self.hash(key) % self.buckets.len();
        
        for (i, (existing_key, _)) in self.buckets[bucket_index].iter().enumerate() {
            if existing_key == key {
                self.size -= 1;
                return Some(self.buckets[bucket_index].remove(i).1);
            }
        }
        
        None
    }
    
    fn contains(&self, key: &K) -> bool {
        self.get(key).is_some()
    }
    
    fn keys(&self) -> Vec<&K> {
        let mut keys = Vec::new();
        for bucket in &self.buckets {
            for (key, _) in bucket {
                keys.push(key);
            }
        }
        keys
    }
    
    fn size(&self) -> usize {
        self.size
    }
    
    fn hash(&self, key: &K) -> usize {
        // Simple hash function - in practice, you'd use a proper hash function
        format!("{:?}", key).len()
    }
    
    fn resize(&mut self) {
        let old_buckets = std::mem::take(&mut self.buckets);
        self.buckets = vec![Vec::new(); old_buckets.len() * 2];
        self.size = 0;
        
        for bucket in old_buckets {
            for (key, value) in bucket {
                self.insert(key, value);
            }
        }
    }
}

fn main() {
    let cli = Cli::parse();
    
    match cli.command {
        Commands::Tree { operation } => {
            let mut tree = BinaryTree::new();
            
            // Add some sample data
            tree.insert(5);
            tree.insert(3);
            tree.insert(7);
            tree.insert(1);
            tree.insert(9);
            
            match operation {
                TreeOperation::Insert { value } => {
                    tree.insert(value);
                    println!("Inserted {} into tree", value);
                }
                TreeOperation::Search { value } => {
                    let found = tree.search(value);
                    println!("Value {} {} found in tree", value, if found { "was" } else { "was not" });
                }
                TreeOperation::Traverse => {
                    let traversal = tree.inorder_traversal();
                    println!("Inorder traversal: {:?}", traversal);
                }
                TreeOperation::Size => {
                    println!("Tree size: {}", tree.size());
                }
                TreeOperation::Height => {
                    println!("Tree height: {}", tree.height());
                }
                TreeOperation::Delete { value: _ } => {
                    println!("Delete operation not implemented in this example");
                }
            }
        }
        Commands::List { operation } => {
            let mut list = LinkedList::new();
            
            // Add some sample data
            list.push(1);
            list.push(2);
            list.push(3);
            
            match operation {
                ListOperation::Push { value } => {
                    list.push(value);
                    println!("Pushed {} to list", value);
                }
                ListOperation::Pop => {
                    if let Some(value) = list.pop() {
                        println!("Popped {} from list", value);
                    } else {
                        println!("List is empty");
                    }
                }
                ListOperation::Insert { index, value } => {
                    match list.insert(index, value) {
                        Ok(_) => println!("Inserted {} at index {}", value, index),
                        Err(e) => println!("Error: {}", e),
                    }
                }
                ListOperation::Get { index } => {
                    if let Some(value) = list.get(index) {
                        println!("Value at index {}: {:?}", index, value);
                    } else {
                        println!("Index {} out of bounds", index);
                    }
                }
                ListOperation::Length => {
                    println!("List length: {}", list.length());
                }
                ListOperation::Reverse => {
                    list.reverse();
                    println!("List reversed");
                }
                ListOperation::Remove { index: _ } => {
                    println!("Remove operation not implemented in this example");
                }
            }
        }
        Commands::Heap { operation } => {
            let mut heap = MaxHeap::new();
            
            // Add some sample data
            heap.insert(10);
            heap.insert(5);
            heap.insert(15);
            heap.insert(3);
            
            match operation {
                HeapOperation::Insert { value } => {
                    heap.insert(value);
                    println!("Inserted {} into heap", value);
                }
                HeapOperation::ExtractMax => {
                    if let Some(max) = heap.extract_max() {
                        println!("Extracted max: {:?}", max);
                    } else {
                        println!("Heap is empty");
                    }
                }
                HeapOperation::Peek => {
                    if let Some(max) = heap.peek() {
                        println!("Max value: {:?}", max);
                    } else {
                        println!("Heap is empty");
                    }
                }
                HeapOperation::Size => {
                    println!("Heap size: {}", heap.size());
                }
                HeapOperation::IsEmpty => {
                    println!("Heap is empty: {}", heap.is_empty());
                }
            }
        }
        Commands::HashMap { operation } => {
            let mut map = HashMap::new();
            
            // Add some sample data
            map.insert("key1".to_string(), "value1".to_string());
            map.insert("key2".to_string(), "value2".to_string());
            
            match operation {
                HashMapOperation::Insert { key, value } => {
                    map.insert(key.clone(), value.clone());
                    println!("Inserted {} -> {}", key, value);
                }
                HashMapOperation::Get { key } => {
                    if let Some(value) = map.get(&key) {
                        println!("Value for {}: {}", key, value);
                    } else {
                        println!("Key {} not found", key);
                    }
                }
                HashMapOperation::Remove { key } => {
                    if let Some(value) = map.remove(&key) {
                        println!("Removed {} -> {}", key, value);
                    } else {
                        println!("Key {} not found", key);
                    }
                }
                HashMapOperation::Contains { key } => {
                    let contains = map.contains(&key);
                    println!("Map contains {}: {}", key, contains);
                }
                HashMapOperation::Size => {
                    println!("Map size: {}", map.size());
                }
                HashMapOperation::Keys => {
                    let keys = map.keys();
                    println!("Keys: {:?}", keys);
                }
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_binary_tree() {
        let mut tree = BinaryTree::new();
        tree.insert(5);
        tree.insert(3);
        tree.insert(7);
        
        assert!(tree.search(5));
        assert!(tree.search(3));
        assert!(!tree.search(1));
        assert_eq!(tree.size(), 3);
    }

    #[test]
    fn test_linked_list() {
        let mut list = LinkedList::new();
        list.push(1);
        list.push(2);
        
        assert_eq!(list.length(), 2);
        assert_eq!(list.pop(), Some(2));
        assert_eq!(list.length(), 1);
    }

    #[test]
    fn test_max_heap() {
        let mut heap = MaxHeap::new();
        heap.insert(10);
        heap.insert(5);
        heap.insert(15);
        
        assert_eq!(heap.peek(), Some(&15));
        assert_eq!(heap.extract_max(), Some(15));
        assert_eq!(heap.peek(), Some(&10));
    }

    #[test]
    fn test_hash_map() {
        let mut map = HashMap::new();
        map.insert("key1".to_string(), "value1".to_string());
        
        assert!(map.contains(&"key1".to_string()));
        assert_eq!(map.get(&"key1".to_string()), Some(&"value1".to_string()));
        assert_eq!(map.size(), 1);
    }
}