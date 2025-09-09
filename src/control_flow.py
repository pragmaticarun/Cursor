"""
Control Flow in Python

This module demonstrates conditionals, loops, and exception handling.
"""


def demonstrate_if_else(age=18):
    """Demonstrate if-else statements."""
    if age < 0:
        category = "Invalid age"
    elif age < 13:
        category = "Child"
    elif age < 20:
        category = "Teenager"
    elif age < 60:
        category = "Adult"
    else:
        category = "Senior"
    
    # Ternary operator
    status = "Minor" if age < 18 else "Adult"
    
    return {
        'age': age,
        'category': category,
        'legal_status': status
    }


def demonstrate_nested_conditions(score=85):
    """Demonstrate nested if statements."""
    if score >= 0 and score <= 100:
        if score >= 90:
            grade = 'A'
            message = "Excellent!"
        elif score >= 80:
            grade = 'B'
            message = "Good job!"
        elif score >= 70:
            grade = 'C'
            message = "Satisfactory"
        elif score >= 60:
            grade = 'D'
            message = "Needs improvement"
        else:
            grade = 'F'
            message = "Failed"
    else:
        grade = 'Invalid'
        message = "Score must be between 0 and 100"
    
    return {
        'score': score,
        'grade': grade,
        'message': message
    }


def demonstrate_for_loop():
    """Demonstrate different for loop patterns."""
    results = {}
    
    # Basic for loop with list
    fruits = ["apple", "banana", "orange"]
    results['fruits'] = [f.upper() for f in fruits]
    
    # For loop with range
    results['range_5'] = list(range(5))
    results['range_2_10_2'] = list(range(2, 10, 2))  # start, stop, step
    
    # For loop with enumerate
    results['enumerated'] = [(i, fruit) for i, fruit in enumerate(fruits)]
    
    # For loop with zip
    colors = ["red", "yellow", "orange"]
    results['zipped'] = list(zip(fruits, colors))
    
    # Nested loops
    matrix = []
    for i in range(3):
        row = []
        for j in range(3):
            row.append(i * 3 + j)
        matrix.append(row)
    results['matrix'] = matrix
    
    return results


def demonstrate_while_loop():
    """Demonstrate while loop patterns."""
    results = {}
    
    # Basic while loop
    count = 0
    counts = []
    while count < 5:
        counts.append(count)
        count += 1
    results['basic_count'] = counts
    
    # While with break
    num = 0
    nums = []
    while True:
        if num >= 10:
            break
        nums.append(num)
        num += 2
    results['with_break'] = nums
    
    # Fibonacci sequence
    a, b = 0, 1
    fib = []
    while a < 100:
        fib.append(a)
        a, b = b, a + b
    results['fibonacci'] = fib
    
    return results


def demonstrate_loop_control():
    """Demonstrate break, continue, and else in loops."""
    results = {}
    
    # Continue example
    skip_evens = []
    for i in range(10):
        if i % 2 == 0:
            continue
        skip_evens.append(i)
    results['odds_only'] = skip_evens
    
    # Break example
    find_first = None
    for i in range(100):
        if i > 10 and i % 7 == 0:
            find_first = i
            break
    results['first_multiple_of_7_after_10'] = find_first
    
    # Loop else clause (executes if loop completes without break)
    search_list = [1, 3, 5, 7, 9]
    search_for = 6
    found = False
    for item in search_list:
        if item == search_for:
            found = True
            break
    else:
        found = False
    results['found_6'] = found
    
    # Pass statement (placeholder)
    placeholder = []
    for i in range(5):
        if i == 2:
            pass  # Do nothing, placeholder for future code
        else:
            placeholder.append(i)
    results['with_pass'] = placeholder
    
    return results


def demonstrate_list_comprehension():
    """Demonstrate list comprehensions."""
    # Basic list comprehension
    squares = [x**2 for x in range(10)]
    
    # With condition
    evens = [x for x in range(20) if x % 2 == 0]
    
    # Multiple conditions
    divisible = [x for x in range(100) if x % 3 == 0 if x % 5 == 0]
    
    # Nested list comprehension
    matrix = [[i*3 + j for j in range(3)] for i in range(3)]
    
    # With conditional expression
    pos_neg = [x if x > 0 else -x for x in [-3, -1, 0, 1, 3]]
    
    # String manipulation
    words = ['hello', 'world', 'python']
    upper_words = [word.upper() for word in words]
    
    return {
        'squares': squares,
        'evens': evens,
        'divisible_by_15': divisible,
        'matrix': matrix,
        'absolute_values': pos_neg,
        'uppercase': upper_words
    }


def demonstrate_dict_set_comprehension():
    """Demonstrate dictionary and set comprehensions."""
    # Dictionary comprehension
    squares_dict = {x: x**2 for x in range(5)}
    
    # Dict comprehension with condition
    even_squares = {x: x**2 for x in range(10) if x % 2 == 0}
    
    # Transform dictionary
    prices = {'apple': 0.5, 'banana': 0.3, 'orange': 0.7}
    discounted = {item: price * 0.8 for item, price in prices.items()}
    
    # Set comprehension
    unique_lengths = {len(word) for word in ['hi', 'hello', 'hi', 'python']}
    
    # Set with condition
    unique_vowels = {char.lower() for char in 'Hello World' if char.lower() in 'aeiou'}
    
    return {
        'squares_dict': squares_dict,
        'even_squares': even_squares,
        'discounted_prices': discounted,
        'unique_lengths': unique_lengths,
        'unique_vowels': unique_vowels
    }


def demonstrate_exception_handling(numerator=10, denominator=0):
    """Demonstrate exception handling patterns."""
    results = {}
    
    # Basic try-except
    try:
        result = numerator / denominator
        results['division'] = result
    except ZeroDivisionError:
        results['division'] = "Cannot divide by zero"
    
    # Multiple exception types
    test_values = ['10', '3.14', 'abc', None, 5]
    converted = []
    for val in test_values:
        try:
            converted.append(float(val))
        except (TypeError, ValueError) as e:
            converted.append(f"Error: {type(e).__name__}")
    results['conversions'] = converted
    
    # Try-except-else-finally
    try:
        file_content = "Simulated file read"
        results['file_status'] = 'read'
    except IOError:
        results['file_status'] = 'error'
    else:
        results['file_processed'] = True
    finally:
        results['cleanup_done'] = True
    
    return results


def demonstrate_raising_exceptions():
    """Demonstrate raising and custom exceptions."""
    
    class CustomError(Exception):
        """Custom exception class."""
        pass
    
    def validate_age(age):
        """Validate age with custom exceptions."""
        if not isinstance(age, (int, float)):
            raise TypeError("Age must be a number")
        if age < 0:
            raise ValueError("Age cannot be negative")
        if age > 150:
            raise CustomError("Age seems unrealistic")
        return True
    
    test_cases = [25, -5, 200, "twenty", 50.5]
    results = {}
    
    for test_age in test_cases:
        try:
            validate_age(test_age)
            results[str(test_age)] = "Valid"
        except TypeError as e:
            results[str(test_age)] = f"TypeError: {e}"
        except ValueError as e:
            results[str(test_age)] = f"ValueError: {e}"
        except CustomError as e:
            results[str(test_age)] = f"CustomError: {e}"
    
    return results


def demonstrate_context_managers():
    """Demonstrate context managers (with statement)."""
    results = {}
    
    # Simulated file operation
    class MockFile:
        def __init__(self, name):
            self.name = name
            self.closed = False
        
        def __enter__(self):
            results['file_opened'] = self.name
            return self
        
        def __exit__(self, exc_type, exc_val, exc_tb):
            self.closed = True
            results['file_closed'] = True
            return False
        
        def write(self, data):
            results['data_written'] = data
    
    # Using context manager
    with MockFile('test.txt') as f:
        f.write('Hello, World!')
    
    # Multiple context managers
    with MockFile('input.txt') as infile, MockFile('output.txt') as outfile:
        results['multiple_files'] = True
    
    return results


def demonstrate_match_statement():
    """Demonstrate match statement (Python 3.10+)."""
    def process_command(command):
        """Process different command patterns."""
        # Note: This is a simulation for Python < 3.10
        # In Python 3.10+, you would use actual match/case syntax
        
        if command == "quit":
            return "Exiting program"
        elif command == "help":
            return "Available commands: quit, help, status"
        elif command.startswith("echo "):
            return f"Echo: {command[5:]}"
        elif command.startswith("calc "):
            parts = command[5:].split()
            if len(parts) == 3:
                try:
                    a, op, b = parts
                    a, b = float(a), float(b)
                    if op == '+':
                        return f"Result: {a + b}"
                    elif op == '-':
                        return f"Result: {a - b}"
                    elif op == '*':
                        return f"Result: {a * b}"
                    elif op == '/':
                        return f"Result: {a / b}"
                except:
                    return "Invalid calculation"
            return "Invalid calculation format"
        else:
            return "Unknown command"
    
    test_commands = [
        "quit",
        "help",
        "echo Hello World",
        "calc 10 + 5",
        "calc 20 / 4",
        "unknown"
    ]
    
    return {cmd: process_command(cmd) for cmd in test_commands}


if __name__ == "__main__":
    # Demonstrate all features when run directly
    print("=== If-Else Demo ===")
    print(demonstrate_if_else(25))
    
    print("\n=== Nested Conditions Demo ===")
    print(demonstrate_nested_conditions(85))
    
    print("\n=== For Loop Demo ===")
    print(demonstrate_for_loop())
    
    print("\n=== While Loop Demo ===")
    print(demonstrate_while_loop())
    
    print("\n=== Loop Control Demo ===")
    print(demonstrate_loop_control())
    
    print("\n=== List Comprehension Demo ===")
    print(demonstrate_list_comprehension())
    
    print("\n=== Dict/Set Comprehension Demo ===")
    print(demonstrate_dict_set_comprehension())
    
    print("\n=== Exception Handling Demo ===")
    print(demonstrate_exception_handling())
    
    print("\n=== Raising Exceptions Demo ===")
    print(demonstrate_raising_exceptions())
    
    print("\n=== Context Managers Demo ===")
    print(demonstrate_context_managers())
    
    print("\n=== Match Statement Demo ===")
    print(demonstrate_match_statement())