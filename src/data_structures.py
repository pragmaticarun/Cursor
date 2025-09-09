"""
Data Structures in Python

This module demonstrates lists, dictionaries, sets, tuples, and other data structures.
"""

from collections import namedtuple, deque, Counter, defaultdict, OrderedDict, ChainMap
from typing import List, Dict, Set, Tuple, Any
import heapq
import bisect


# ============= LISTS =============

def demonstrate_list_operations():
    """Demonstrate list creation and operations."""
    # Creating lists
    empty_list = []
    numbers = [1, 2, 3, 4, 5]
    mixed = [1, "hello", 3.14, True, None]
    nested = [[1, 2], [3, 4], [5, 6]]
    
    # List operations
    results = {}
    
    # Adding elements
    lst = [1, 2, 3]
    lst.append(4)  # Add to end
    lst.insert(0, 0)  # Insert at index
    lst.extend([5, 6])  # Add multiple items
    results['after_adding'] = lst.copy()
    
    # Removing elements
    lst.remove(3)  # Remove first occurrence
    popped = lst.pop()  # Remove and return last
    popped_at = lst.pop(0)  # Remove at index
    results['after_removing'] = lst.copy()
    results['popped_values'] = (popped, popped_at)
    
    # Searching and counting
    test_list = [1, 2, 3, 2, 4, 2]
    results['count_2'] = test_list.count(2)
    results['index_of_3'] = test_list.index(3)
    results['4_in_list'] = 4 in test_list
    
    # Sorting and reversing
    unsorted = [3, 1, 4, 1, 5, 9, 2]
    sorted_list = sorted(unsorted)  # Returns new list
    unsorted.sort()  # Sorts in place
    unsorted.reverse()  # Reverses in place
    results['sorted'] = sorted_list
    results['reversed'] = unsorted
    
    # List as stack (LIFO)
    stack = []
    stack.append(1)  # Push
    stack.append(2)
    stack.append(3)
    top = stack.pop()  # Pop
    results['stack_operations'] = {'stack': stack, 'popped': top}
    
    # List as queue (FIFO) - not efficient, use deque instead
    queue = [1, 2, 3]
    queue.append(4)  # Enqueue
    first = queue.pop(0)  # Dequeue (inefficient)
    results['queue_operations'] = {'queue': queue, 'dequeued': first}
    
    return results


def demonstrate_list_slicing():
    """Demonstrate list slicing techniques."""
    nums = list(range(10))  # [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    
    return {
        'original': nums,
        'first_three': nums[:3],
        'last_three': nums[-3:],
        'middle': nums[3:7],
        'every_second': nums[::2],
        'reverse': nums[::-1],
        'reverse_every_second': nums[::-2],
        'copy': nums[:],
        'except_first_last': nums[1:-1],
        'slice_assignment': demonstrate_slice_assignment()
    }


def demonstrate_slice_assignment():
    """Demonstrate slice assignment."""
    lst = list(range(10))
    
    # Replace a slice
    lst[2:5] = ['a', 'b', 'c']
    result1 = lst.copy()
    
    # Delete a slice
    lst[2:5] = []
    result2 = lst.copy()
    
    # Insert at a position
    lst[2:2] = ['x', 'y', 'z']
    result3 = lst.copy()
    
    return {
        'after_replace': result1,
        'after_delete': result2,
        'after_insert': result3
    }


def demonstrate_list_comprehensions():
    """Demonstrate various list comprehension patterns."""
    # Basic comprehension
    squares = [x**2 for x in range(10)]
    
    # With condition
    evens = [x for x in range(20) if x % 2 == 0]
    
    # Multiple conditions
    divisible = [x for x in range(100) if x % 3 == 0 if x % 5 == 0]
    
    # Nested loops
    pairs = [(x, y) for x in range(3) for y in range(3)]
    
    # Nested comprehension
    matrix = [[i*3 + j for j in range(3)] for i in range(3)]
    
    # With conditional expression
    pos_neg = [x if x > 0 else -x for x in [-3, -1, 0, 1, 3]]
    
    # Flattening nested list
    nested = [[1, 2], [3, 4], [5, 6]]
    flattened = [item for sublist in nested for item in sublist]
    
    # String manipulation
    words = ['hello', 'world', 'python']
    lengths = [len(word) for word in words]
    upper = [word.upper() for word in words if len(word) > 4]
    
    return {
        'squares': squares,
        'evens': evens,
        'divisible_by_15': divisible,
        'pairs': pairs,
        'matrix': matrix,
        'absolute_values': pos_neg,
        'flattened': flattened,
        'word_lengths': lengths,
        'long_words_upper': upper
    }


# ============= DICTIONARIES =============

def demonstrate_dict_operations():
    """Demonstrate dictionary creation and operations."""
    # Creating dictionaries
    empty_dict = {}
    person = {"name": "Alice", "age": 30, "city": "New York"}
    from_tuples = dict([('a', 1), ('b', 2), ('c', 3)])
    from_kwargs = dict(name="Bob", age=25, city="London")
    from_zip = dict(zip(['x', 'y', 'z'], [1, 2, 3]))
    
    results = {}
    
    # Accessing values
    test_dict = {"a": 1, "b": 2, "c": 3}
    results['get_a'] = test_dict['a']
    results['get_with_default'] = test_dict.get('d', 0)
    
    # Adding/updating
    test_dict['d'] = 4
    test_dict.update({'e': 5, 'f': 6})
    results['after_update'] = test_dict.copy()
    
    # Removing
    removed = test_dict.pop('f')
    last_item = test_dict.popitem()  # Remove arbitrary item
    results['removed_values'] = (removed, last_item)
    
    # Dictionary methods
    results['keys'] = list(test_dict.keys())
    results['values'] = list(test_dict.values())
    results['items'] = list(test_dict.items())
    
    # Checking membership
    results['has_key_a'] = 'a' in test_dict
    results['has_value_2'] = 2 in test_dict.values()
    
    # Merging dictionaries
    dict1 = {'a': 1, 'b': 2}
    dict2 = {'b': 3, 'c': 4}
    merged = {**dict1, **dict2}  # Python 3.5+
    results['merged'] = merged
    
    # Dictionary comprehension
    squares_dict = {x: x**2 for x in range(5)}
    filtered = {k: v for k, v in test_dict.items() if v > 2}
    inverted = {v: k for k, v in test_dict.items()}
    
    results['comprehensions'] = {
        'squares': squares_dict,
        'filtered': filtered,
        'inverted': inverted
    }
    
    return results


def demonstrate_nested_dicts():
    """Demonstrate working with nested dictionaries."""
    # Nested dictionary
    company = {
        'employees': {
            'john': {'age': 30, 'department': 'IT', 'salary': 70000},
            'jane': {'age': 28, 'department': 'HR', 'salary': 65000},
            'bob': {'age': 35, 'department': 'IT', 'salary': 80000}
        },
        'departments': {
            'IT': {'budget': 500000, 'head': 'john'},
            'HR': {'budget': 200000, 'head': 'jane'}
        }
    }
    
    # Accessing nested values
    johns_age = company['employees']['john']['age']
    it_budget = company['departments']['IT']['budget']
    
    # Safely accessing nested values
    def safe_get(dct, *keys, default=None):
        """Safely get nested dictionary values."""
        for key in keys:
            if isinstance(dct, dict):
                dct = dct.get(key, default)
            else:
                return default
        return dct
    
    safe_value = safe_get(company, 'employees', 'alice', 'age', default=0)
    
    # Flattening nested dictionary
    def flatten_dict(d, parent_key='', sep='_'):
        """Flatten nested dictionary."""
        items = []
        for k, v in d.items():
            new_key = f"{parent_key}{sep}{k}" if parent_key else k
            if isinstance(v, dict):
                items.extend(flatten_dict(v, new_key, sep=sep).items())
            else:
                items.append((new_key, v))
        return dict(items)
    
    flat = flatten_dict({'a': {'b': 1, 'c': 2}, 'd': 3})
    
    return {
        'johns_age': johns_age,
        'it_budget': it_budget,
        'safe_access': safe_value,
        'flattened': flat
    }


# ============= SETS =============

def demonstrate_set_operations():
    """Demonstrate set creation and operations."""
    # Creating sets
    empty_set = set()
    fruits = {"apple", "banana", "orange"}
    from_list = set([1, 2, 3, 3, 4])  # Duplicates removed
    from_string = set("hello")  # {'h', 'e', 'l', 'o'}
    
    results = {}
    
    # Adding and removing
    test_set = {1, 2, 3}
    test_set.add(4)
    test_set.update([5, 6, 7])
    test_set.discard(7)  # No error if not found
    test_set.remove(6)   # Raises error if not found
    results['modified_set'] = test_set.copy()
    
    # Set operations
    set1 = {1, 2, 3, 4, 5}
    set2 = {4, 5, 6, 7, 8}
    
    results['union'] = set1 | set2  # or set1.union(set2)
    results['intersection'] = set1 & set2  # or set1.intersection(set2)
    results['difference'] = set1 - set2  # or set1.difference(set2)
    results['symmetric_difference'] = set1 ^ set2  # or set1.symmetric_difference(set2)
    
    # Subset and superset
    results['is_subset'] = {1, 2}.issubset(set1)
    results['is_superset'] = set1.issuperset({1, 2})
    results['is_disjoint'] = {1, 2}.isdisjoint({3, 4})
    
    # Set comprehension
    squares_set = {x**2 for x in range(10)}
    evens_set = {x for x in range(20) if x % 2 == 0}
    
    results['comprehensions'] = {
        'squares': squares_set,
        'evens': evens_set
    }
    
    # Frozen set (immutable)
    frozen = frozenset([1, 2, 3])
    results['frozen_set'] = {
        'type': type(frozen).__name__,
        'hashable': hash(frozen) is not None
    }
    
    return results


# ============= TUPLES =============

def demonstrate_tuple_operations():
    """Demonstrate tuple creation and operations."""
    # Creating tuples
    empty_tuple = ()
    single = (42,)  # Note the comma
    coordinates = (10, 20)
    mixed = (1, "hello", 3.14)
    nested = ((1, 2), (3, 4))
    
    results = {}
    
    # Accessing elements
    test_tuple = (1, 2, 3, 4, 5)
    results['first'] = test_tuple[0]
    results['last'] = test_tuple[-1]
    results['slice'] = test_tuple[1:4]
    
    # Tuple unpacking
    x, y = (10, 20)
    a, *rest, b = (1, 2, 3, 4, 5)
    results['unpacking'] = {
        'x_y': (x, y),
        'a_rest_b': (a, rest, b)
    }
    
    # Tuple methods
    test_tuple = (1, 2, 3, 2, 4, 2)
    results['count_2'] = test_tuple.count(2)
    results['index_of_3'] = test_tuple.index(3)
    
    # Named tuples
    Point = namedtuple('Point', ['x', 'y'])
    p1 = Point(11, y=22)
    p2 = Point._make([33, 44])
    
    results['named_tuple'] = {
        'point1': (p1.x, p1.y),
        'point2': p2._asdict(),
        'fields': Point._fields
    }
    
    # Tuple as dictionary key (hashable)
    dict_with_tuple_keys = {
        (0, 0): 'origin',
        (1, 0): 'right',
        (0, 1): 'up'
    }
    results['tuple_as_key'] = dict_with_tuple_keys[(0, 0)]
    
    return results


# ============= COLLECTIONS MODULE =============

def demonstrate_collections():
    """Demonstrate collections module data structures."""
    results = {}
    
    # Counter
    words = ['apple', 'banana', 'apple', 'orange', 'banana', 'apple']
    word_count = Counter(words)
    results['counter'] = {
        'counts': dict(word_count),
        'most_common': word_count.most_common(2),
        'total': sum(word_count.values())
    }
    
    # defaultdict
    dd = defaultdict(list)
    dd['fruits'].append('apple')
    dd['fruits'].append('banana')
    dd['vegetables'].append('carrot')
    results['defaultdict'] = dict(dd)
    
    # defaultdict with lambda
    dd_int = defaultdict(lambda: 0)
    dd_int['a'] += 1
    dd_int['b'] += 2
    results['defaultdict_int'] = dict(dd_int)
    
    # deque (double-ended queue)
    d = deque([1, 2, 3])
    d.append(4)        # Add to right
    d.appendleft(0)    # Add to left
    d.rotate(1)        # Rotate right
    popped_right = d.pop()
    popped_left = d.popleft()
    results['deque'] = {
        'final': list(d),
        'popped': (popped_left, popped_right)
    }
    
    # OrderedDict (maintains insertion order)
    od = OrderedDict()
    od['first'] = 1
    od['second'] = 2
    od['third'] = 3
    od.move_to_end('first')  # Move to end
    results['ordered_dict'] = dict(od)
    
    # ChainMap
    dict1 = {'a': 1, 'b': 2}
    dict2 = {'b': 3, 'c': 4}
    chain = ChainMap(dict1, dict2)
    results['chain_map'] = {
        'combined': dict(chain),
        'maps': chain.maps,
        'b_value': chain['b']  # Returns 2 (from first dict)
    }
    
    return results


# ============= ADVANCED DATA STRUCTURES =============

def demonstrate_heap_operations():
    """Demonstrate heap operations using heapq."""
    # Min heap
    heap = []
    heapq.heappush(heap, 5)
    heapq.heappush(heap, 3)
    heapq.heappush(heap, 7)
    heapq.heappush(heap, 1)
    
    smallest = heapq.heappop(heap)
    
    # Convert list to heap
    nums = [4, 1, 7, 3, 8, 5]
    heapq.heapify(nums)
    
    # N largest/smallest
    data = [1, 3, 5, 7, 9, 2, 4, 6, 8, 0]
    largest_3 = heapq.nlargest(3, data)
    smallest_3 = heapq.nsmallest(3, data)
    
    # Priority queue implementation
    class PriorityQueue:
        def __init__(self):
            self._queue = []
            self._index = 0
        
        def push(self, item, priority):
            heapq.heappush(self._queue, (priority, self._index, item))
            self._index += 1
        
        def pop(self):
            return heapq.heappop(self._queue)[-1]
    
    pq = PriorityQueue()
    pq.push('task1', 3)
    pq.push('task2', 1)
    pq.push('task3', 2)
    
    return {
        'smallest_popped': smallest,
        'heap_after_pop': heap,
        'heapified': nums,
        'largest_3': largest_3,
        'smallest_3': smallest_3,
        'priority_queue_pop': pq.pop()
    }


def demonstrate_bisect_operations():
    """Demonstrate bisect module for sorted lists."""
    # Maintaining sorted list
    sorted_list = [1, 3, 4, 7, 9]
    
    # Find insertion point
    pos = bisect.bisect_left(sorted_list, 5)
    
    # Insert while maintaining sort
    bisect.insort(sorted_list, 5)
    
    # Binary search implementation
    def binary_search(lst, item):
        """Binary search using bisect."""
        i = bisect.bisect_left(lst, item)
        if i != len(lst) and lst[i] == item:
            return i
        return -1
    
    # Grade calculation using bisect
    def calculate_grade(score, breakpoints=[60, 70, 80, 90], grades='FDCBA'):
        """Calculate grade based on score."""
        i = bisect.bisect(breakpoints, score)
        return grades[i]
    
    return {
        'insertion_point': pos,
        'after_insert': sorted_list,
        'search_for_7': binary_search(sorted_list, 7),
        'search_for_6': binary_search(sorted_list, 6),
        'grade_85': calculate_grade(85),
        'grade_72': calculate_grade(72)
    }


def demonstrate_stack_queue():
    """Demonstrate stack and queue implementations."""
    # Stack using list
    stack = []
    stack.append(1)
    stack.append(2)
    stack.append(3)
    stack_top = stack.pop() if stack else None
    
    # Queue using deque (efficient)
    queue = deque()
    queue.append(1)
    queue.append(2)
    queue.append(3)
    queue_first = queue.popleft() if queue else None
    
    # Custom Stack class
    class Stack:
        def __init__(self):
            self._items = []
        
        def push(self, item):
            self._items.append(item)
        
        def pop(self):
            if not self.is_empty():
                return self._items.pop()
            raise IndexError("Stack is empty")
        
        def peek(self):
            if not self.is_empty():
                return self._items[-1]
            raise IndexError("Stack is empty")
        
        def is_empty(self):
            return len(self._items) == 0
        
        def size(self):
            return len(self._items)
    
    # Custom Queue class
    class Queue:
        def __init__(self):
            self._items = deque()
        
        def enqueue(self, item):
            self._items.append(item)
        
        def dequeue(self):
            if not self.is_empty():
                return self._items.popleft()
            raise IndexError("Queue is empty")
        
        def front(self):
            if not self.is_empty():
                return self._items[0]
            raise IndexError("Queue is empty")
        
        def is_empty(self):
            return len(self._items) == 0
        
        def size(self):
            return len(self._items)
    
    # Test custom implementations
    custom_stack = Stack()
    custom_stack.push(10)
    custom_stack.push(20)
    
    custom_queue = Queue()
    custom_queue.enqueue('a')
    custom_queue.enqueue('b')
    
    return {
        'list_stack': {'remaining': stack, 'popped': stack_top},
        'deque_queue': {'remaining': list(queue), 'dequeued': queue_first},
        'custom_stack': {
            'peek': custom_stack.peek(),
            'size': custom_stack.size()
        },
        'custom_queue': {
            'front': custom_queue.front(),
            'size': custom_queue.size()
        }
    }


if __name__ == "__main__":
    # Demonstrate all features when run directly
    print("=== List Operations ===")
    print(demonstrate_list_operations())
    
    print("\n=== List Slicing ===")
    print(demonstrate_list_slicing())
    
    print("\n=== List Comprehensions ===")
    print(demonstrate_list_comprehensions())
    
    print("\n=== Dictionary Operations ===")
    print(demonstrate_dict_operations())
    
    print("\n=== Nested Dictionaries ===")
    print(demonstrate_nested_dicts())
    
    print("\n=== Set Operations ===")
    print(demonstrate_set_operations())
    
    print("\n=== Tuple Operations ===")
    print(demonstrate_tuple_operations())
    
    print("\n=== Collections Module ===")
    print(demonstrate_collections())
    
    print("\n=== Heap Operations ===")
    print(demonstrate_heap_operations())
    
    print("\n=== Bisect Operations ===")
    print(demonstrate_bisect_operations())
    
    print("\n=== Stack and Queue ===")
    print(demonstrate_stack_queue())