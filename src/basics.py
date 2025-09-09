"""
Basic Python Language Features

This module demonstrates variables, data types, and operators in Python.
"""


def demonstrate_variables():
    """Demonstrate different variable types in Python."""
    # Numbers
    integer_num = 42
    float_num = 3.14159
    complex_num = 3 + 4j
    
    # Strings
    single_quote = 'Hello'
    double_quote = "World"
    multi_line = """This is a
multi-line string"""
    
    # Booleans
    is_true = True
    is_false = False
    
    # None type
    nothing = None
    
    return {
        'integer': integer_num,
        'float': float_num,
        'complex': complex_num,
        'string': f"{single_quote} {double_quote}",
        'multiline': multi_line,
        'boolean_true': is_true,
        'boolean_false': is_false,
        'none': nothing
    }


def demonstrate_type_checking():
    """Demonstrate type checking and conversion."""
    num = 42
    text = "123"
    decimal = 3.14
    
    results = {
        'type_of_num': type(num).__name__,
        'is_int': isinstance(num, int),
        'is_float': isinstance(decimal, float),
        'str_to_int': int(text),
        'int_to_str': str(num),
        'float_to_int': int(decimal),
        'int_to_float': float(num)
    }
    
    return results


def demonstrate_arithmetic_operators(a=10, b=3):
    """Demonstrate arithmetic operators."""
    return {
        'addition': a + b,
        'subtraction': a - b,
        'multiplication': a * b,
        'division': a / b,
        'floor_division': a // b,
        'modulo': a % b,
        'exponentiation': a ** b
    }


def demonstrate_comparison_operators(a=10, b=3):
    """Demonstrate comparison operators."""
    return {
        'greater_than': a > b,
        'less_than': a < b,
        'equal': a == b,
        'not_equal': a != b,
        'greater_equal': a >= b,
        'less_equal': a <= b
    }


def demonstrate_logical_operators(x=True, y=False):
    """Demonstrate logical operators."""
    return {
        'and': x and y,
        'or': x or y,
        'not_x': not x,
        'not_y': not y
    }


def demonstrate_bitwise_operators(m=5, n=3):
    """Demonstrate bitwise operators.
    m = 5 (binary: 101)
    n = 3 (binary: 011)
    """
    return {
        'and': m & n,           # 001 = 1
        'or': m | n,            # 111 = 7
        'xor': m ^ n,           # 110 = 6
        'not': ~m,              # -6 (two's complement)
        'left_shift': m << 1,   # 1010 = 10
        'right_shift': m >> 1   # 010 = 2
    }


def demonstrate_string_operations():
    """Demonstrate string operations and methods."""
    text = "Python Programming"
    
    return {
        'length': len(text),
        'uppercase': text.upper(),
        'lowercase': text.lower(),
        'title_case': text.title(),
        'starts_with': text.startswith('Python'),
        'ends_with': text.endswith('ing'),
        'find_index': text.find('Pro'),
        'replace': text.replace('Python', 'Java'),
        'split': text.split(),
        'join': '-'.join(['Python', 'is', 'awesome']),
        'strip': "  spaces  ".strip(),
        'substring': text[0:6],
        'reverse': text[::-1],
        'count': text.count('m'),
        'formatted': f"Learning {text} is fun!"
    }


def demonstrate_string_formatting():
    """Demonstrate different string formatting methods."""
    name = "Alice"
    age = 30
    pi = 3.14159
    
    return {
        'f_string': f"Name: {name}, Age: {age}",
        'format_method': "Name: {}, Age: {}".format(name, age),
        'format_named': "Name: {n}, Age: {a}".format(n=name, a=age),
        'percent_format': "Name: %s, Age: %d" % (name, age),
        'number_format': f"Pi: {pi:.2f}",
        'padding': f"{name:>10}",  # Right align with padding
        'zero_padding': f"{age:05d}"  # Zero padding
    }


def demonstrate_assignment_operators():
    """Demonstrate assignment operators."""
    x = 10
    results = []
    
    results.append(('initial', x))
    
    x += 5  # x = x + 5
    results.append(('after_add', x))
    
    x -= 3  # x = x - 3
    results.append(('after_subtract', x))
    
    x *= 2  # x = x * 2
    results.append(('after_multiply', x))
    
    x //= 4  # x = x // 4
    results.append(('after_floor_div', x))
    
    x **= 2  # x = x ** 2
    results.append(('after_power', x))
    
    return dict(results)


def demonstrate_membership_operators():
    """Demonstrate membership operators (in, not in)."""
    fruits = ['apple', 'banana', 'orange']
    text = "Hello World"
    numbers = {1, 2, 3, 4, 5}
    
    return {
        'apple_in_list': 'apple' in fruits,
        'grape_in_list': 'grape' in fruits,
        'banana_not_in_list': 'banana' not in fruits,
        'Hello_in_string': 'Hello' in text,
        'bye_in_string': 'bye' in text,
        'three_in_set': 3 in numbers,
        'ten_not_in_set': 10 not in numbers
    }


def demonstrate_identity_operators():
    """Demonstrate identity operators (is, is not)."""
    a = [1, 2, 3]
    b = [1, 2, 3]
    c = a
    none_val = None
    
    return {
        'a_is_b': a is b,          # False (different objects)
        'a_equals_b': a == b,      # True (same content)
        'a_is_c': a is c,          # True (same object)
        'a_is_not_b': a is not b,  # True
        'none_is_none': none_val is None,  # True (singleton)
        'empty_list_is_none': [] is None   # False
    }


if __name__ == "__main__":
    # Demonstrate all features when run directly
    print("=== Variables Demo ===")
    print(demonstrate_variables())
    
    print("\n=== Type Checking Demo ===")
    print(demonstrate_type_checking())
    
    print("\n=== Arithmetic Operators Demo ===")
    print(demonstrate_arithmetic_operators())
    
    print("\n=== Comparison Operators Demo ===")
    print(demonstrate_comparison_operators())
    
    print("\n=== Logical Operators Demo ===")
    print(demonstrate_logical_operators())
    
    print("\n=== Bitwise Operators Demo ===")
    print(demonstrate_bitwise_operators())
    
    print("\n=== String Operations Demo ===")
    print(demonstrate_string_operations())
    
    print("\n=== String Formatting Demo ===")
    print(demonstrate_string_formatting())
    
    print("\n=== Assignment Operators Demo ===")
    print(demonstrate_assignment_operators())
    
    print("\n=== Membership Operators Demo ===")
    print(demonstrate_membership_operators())
    
    print("\n=== Identity Operators Demo ===")
    print(demonstrate_identity_operators())