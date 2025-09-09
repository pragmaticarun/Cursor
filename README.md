# Python Programming Tutorial 🐍

A comprehensive Python programming tutorial covering basic language features and standard library facilities with inline examples and complete test coverage.

## 📚 Table of Contents

1. [Getting Started](#getting-started)
2. [Quick Examples](#quick-examples)
3. [Module Overview](#module-overview)
4. [Running the Code](#running-the-code)
5. [Running Tests](#running-tests)
6. [Learning Path](#learning-path)

## 🚀 Getting Started

### Prerequisites

- Python 3.7 or higher
- pip (Python package installer)

### Installation

1. Clone or download this repository
2. Navigate to the project directory
3. Install dependencies:

```bash
pip install -r requirements.txt
```

### Project Structure

```
python-tutorial/
├── README.md                  # This file
├── requirements.txt           # Project dependencies
├── src/                      # Source code modules
│   ├── __init__.py
│   ├── basics.py             # Variables, data types, operators
│   ├── control_flow.py       # Conditionals, loops, exceptions
│   ├── functions_classes.py  # Functions and OOP
│   ├── data_structures.py    # Lists, dicts, sets, tuples
│   ├── file_operations.py    # File I/O
│   └── stdlib_examples.py    # Standard library examples
└── tests/                    # Test files
    ├── __init__.py
    ├── test_basics.py
    ├── test_control_flow.py
    ├── test_functions_classes.py
    ├── test_data_structures.py
    ├── test_file_operations.py
    └── test_stdlib_examples.py
```

## 💡 Quick Examples

### Variables and Data Types
```python
# From src/basics.py
integer_num = 42
float_num = 3.14159
text = "Python Programming"
is_active = True

# String operations
print(text.upper())  # PYTHON PROGRAMMING
print(text.split())  # ['Python', 'Programming']
print(f"Learning {text} is fun!")  # String formatting
```

### Control Flow
```python
# From src/control_flow.py
# List comprehension
squares = [x**2 for x in range(10)]
evens = [x for x in range(20) if x % 2 == 0]

# Exception handling
try:
    result = 10 / 0
except ZeroDivisionError:
    print("Cannot divide by zero")
```

### Functions and Classes
```python
# From src/functions_classes.py
# Decorator example
@timer
def slow_function():
    time.sleep(0.1)
    return "Done"

# Class example
class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age
    
    def introduce(self):
        return f"Hi, I'm {self.name}"
```

### Data Structures
```python
# From src/data_structures.py
# Dictionary comprehension
squares = {x: x**2 for x in range(5)}

# Set operations
set1 = {1, 2, 3, 4}
set2 = {3, 4, 5, 6}
intersection = set1 & set2  # {3, 4}
```

## 📖 Module Overview

### 1. **basics.py** - Fundamentals
- Variables and data types
- Arithmetic, comparison, logical operators
- String operations and formatting
- Type checking and conversion

### 2. **control_flow.py** - Program Flow
- If-else statements and nested conditions
- For and while loops
- List/dict/set comprehensions
- Exception handling and context managers
- Loop control (break, continue, pass)

### 3. **functions_classes.py** - Functions & OOP
- Function definitions with various parameter types
- Lambda functions and closures
- Decorators (timer, memoize, validation)
- Classes, inheritance, and polymorphism
- Special methods and properties
- Generators and iterators

### 4. **data_structures.py** - Collections
- Lists: operations, slicing, comprehensions
- Dictionaries: nested dicts, operations
- Sets: mathematical operations
- Tuples and named tuples
- Collections module (Counter, defaultdict, deque)
- Advanced: heaps, bisect, stacks, queues

### 5. **file_operations.py** - File I/O
- Text and binary file operations
- JSON and CSV handling
- Path operations with pathlib
- File utilities (copy, move, delete)
- Context managers
- In-memory files (StringIO, BytesIO)

### 6. **stdlib_examples.py** - Standard Library
- os: system and path operations
- datetime: date/time manipulation
- math: mathematical functions
- random: random number generation
- re: regular expressions
- itertools: iteration tools
- functools: functional programming
- statistics: statistical functions
- hashlib: cryptographic hashing

## 🏃 Running the Code

### Run Individual Modules
Each module can be run directly to see demonstrations:

```bash
# Run basics module
python src/basics.py

# Run control flow examples
python src/control_flow.py

# Run functions and classes
python src/functions_classes.py

# Run data structures
python src/data_structures.py

# Run file operations
python src/file_operations.py

# Run standard library examples
python src/stdlib_examples.py
```

### Interactive Python Session
```python
# Import and use modules interactively
from src import basics
result = basics.demonstrate_variables()
print(result)

from src import data_structures
list_ops = data_structures.demonstrate_list_operations()
print(list_ops)
```

## 🧪 Running Tests

### Run All Tests
```bash
# Simple test run
python -m pytest tests/

# With verbose output
python -m pytest tests/ -v

# With coverage report
python -m pytest tests/ --cov=src --cov-report=html

# Open coverage report (after running with coverage)
open htmlcov/index.html  # On Mac/Linux
start htmlcov/index.html  # On Windows
```

### Run Specific Tests
```bash
# Test a specific module
python -m pytest tests/test_basics.py -v

# Test a specific class
python -m pytest tests/test_basics.py::TestVariables -v

# Test a specific function
python -m pytest tests/test_basics.py::TestVariables::test_demonstrate_variables -v
```

### Run Tests with Different Options
```bash
# Stop on first failure
python -m pytest tests/ -x

# Run only failed tests from last run
python -m pytest tests/ --lf

# Show print statements
python -m pytest tests/ -s

# Parallel execution (requires pytest-xdist)
pip install pytest-xdist
python -m pytest tests/ -n auto
```

## 📚 Learning Path

### Beginner Path
1. Start with `basics.py` - understand variables and operators
2. Move to `control_flow.py` - learn program control
3. Practice with `data_structures.py` - master Python collections
4. Run tests to verify understanding

### Intermediate Path
1. Study `functions_classes.py` - advanced functions and OOP
2. Explore `file_operations.py` - file handling
3. Write your own examples based on the patterns

### Advanced Path
1. Deep dive into `stdlib_examples.py` - standard library
2. Study the test files to understand testing patterns
3. Extend the modules with your own functions
4. Add new test cases

## 🎯 Practice Exercises

After studying each module, try these exercises:

1. **Basics**: Create a calculator function using different operators
2. **Control Flow**: Write a prime number generator using comprehensions
3. **Functions**: Create a decorator that logs function calls
4. **Classes**: Build a BankAccount class with deposit/withdraw methods
5. **Data Structures**: Implement a word frequency counter
6. **Files**: Create a CSV to JSON converter
7. **Standard Library**: Build a file organizer using os and pathlib

## 📝 Tips for Learning

1. **Run the code**: Don't just read - execute each module
2. **Modify examples**: Change values and see what happens
3. **Read the tests**: Tests show expected behavior
4. **Use the debugger**: Step through code to understand flow
5. **Write your own**: Create new functions using learned patterns

## 🔧 Troubleshooting

### Common Issues

1. **Import errors**: Ensure you're in the project root directory
2. **Module not found**: Check Python path includes the src directory
3. **Test failures**: Ensure all dependencies are installed
4. **Permission errors**: Some file operations may need appropriate permissions

### Getting Help

- Read the source code comments
- Check the test files for usage examples
- Python official documentation: https://docs.python.org/3/
- Run `help(module_name)` in Python interpreter

## 📄 License

This tutorial is provided for educational purposes. Feel free to use, modify, and share!

## 🙏 Contributing

Found an issue or want to add more examples? Contributions are welcome!

1. Add your example to the appropriate module
2. Write corresponding tests
3. Ensure all tests pass
4. Document your additions

Happy Learning! 🎉