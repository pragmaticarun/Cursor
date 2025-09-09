"""
Functions and Classes in Python

This module demonstrates functions, decorators, and object-oriented programming.
"""

import time
import functools
from typing import Any, List, Dict, Optional, Union


# ============= FUNCTIONS =============

def basic_function(name: str) -> str:
    """A basic function with type hints."""
    return f"Hello, {name}!"


def function_with_defaults(base: float, exponent: float = 2) -> float:
    """Function with default parameters."""
    return base ** exponent


def function_with_args(*args) -> int:
    """Function with variable positional arguments."""
    return sum(args)


def function_with_kwargs(**kwargs) -> dict:
    """Function with variable keyword arguments."""
    return kwargs


def function_with_all(required: str, default: str = "default", *args, **kwargs) -> dict:
    """Function with all parameter types."""
    return {
        'required': required,
        'default': default,
        'args': args,
        'kwargs': kwargs
    }


# Lambda functions
square = lambda x: x ** 2
add = lambda x, y: x + y
is_even = lambda x: x % 2 == 0


# ============= DECORATORS =============

def timer(func):
    """Decorator to measure function execution time."""
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        start = time.perf_counter()
        result = func(*args, **kwargs)
        end = time.perf_counter()
        wrapper.execution_time = end - start
        return result
    return wrapper


def memoize(func):
    """Decorator to cache function results."""
    cache = {}
    
    @functools.wraps(func)
    def wrapper(*args):
        if args in cache:
            wrapper.cache_hits = getattr(wrapper, 'cache_hits', 0) + 1
            return cache[args]
        result = func(*args)
        cache[args] = result
        wrapper.cache_misses = getattr(wrapper, 'cache_misses', 0) + 1
        return result
    
    wrapper.cache = cache
    return wrapper


def validate_positive(func):
    """Decorator to validate positive arguments."""
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        for arg in args:
            if isinstance(arg, (int, float)) and arg < 0:
                raise ValueError(f"Negative value not allowed: {arg}")
        return func(*args, **kwargs)
    return wrapper


def repeat(times: int):
    """Parameterized decorator to repeat function execution."""
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            results = []
            for _ in range(times):
                results.append(func(*args, **kwargs))
            return results
        return wrapper
    return decorator


# Using decorators
@timer
def slow_function(duration: float = 0.1):
    """A function that simulates slow execution."""
    time.sleep(duration)
    return "Completed"


@memoize
def fibonacci(n: int) -> int:
    """Calculate fibonacci number with memoization."""
    if n <= 1:
        return n
    return fibonacci(n - 1) + fibonacci(n - 2)


@validate_positive
def calculate_area(length: float, width: float) -> float:
    """Calculate area with positive validation."""
    return length * width


@repeat(times=3)
def get_random_number():
    """Get random number, repeated 3 times."""
    import random
    return random.randint(1, 100)


# ============= CLOSURES =============

def create_multiplier(factor: float):
    """Create a closure that multiplies by a factor."""
    def multiplier(x: float) -> float:
        return x * factor
    return multiplier


def create_counter(start: int = 0):
    """Create a closure that counts."""
    count = start
    
    def counter():
        nonlocal count
        count += 1
        return count
    
    counter.reset = lambda: None
    counter.get_count = lambda: count
    
    return counter


# ============= GENERATORS =============

def fibonacci_generator(limit: int):
    """Generate fibonacci numbers up to a limit."""
    a, b = 0, 1
    while a < limit:
        yield a
        a, b = b, a + b


def infinite_counter(start: int = 0, step: int = 1):
    """Create an infinite counter generator."""
    current = start
    while True:
        yield current
        current += step


# ============= CLASSES =============

class Person:
    """Basic class demonstrating OOP concepts."""
    
    # Class variable
    species = "Homo sapiens"
    population = 0
    
    def __init__(self, name: str, age: int):
        """Initialize a Person instance."""
        # Instance variables
        self.name = name
        self.age = age
        self._id = id(self)  # Private convention
        Person.population += 1
    
    def __str__(self) -> str:
        """String representation for users."""
        return f"{self.name} ({self.age} years old)"
    
    def __repr__(self) -> str:
        """String representation for developers."""
        return f"Person(name='{self.name}', age={self.age})"
    
    def __eq__(self, other) -> bool:
        """Equality comparison."""
        if not isinstance(other, Person):
            return False
        return self.name == other.name and self.age == other.age
    
    def introduce(self) -> str:
        """Instance method."""
        return f"Hi, I'm {self.name}, {self.age} years old"
    
    @classmethod
    def from_birth_year(cls, name: str, birth_year: int):
        """Alternative constructor using birth year."""
        from datetime import datetime
        age = datetime.now().year - birth_year
        return cls(name, age)
    
    @staticmethod
    def is_adult(age: int) -> bool:
        """Static method to check if age is adult."""
        return age >= 18
    
    @property
    def age_group(self) -> str:
        """Property to get age group."""
        if self.age < 13:
            return "Child"
        elif self.age < 20:
            return "Teenager"
        elif self.age < 60:
            return "Adult"
        else:
            return "Senior"
    
    @property
    def info(self) -> dict:
        """Property returning person info as dict."""
        return {
            'name': self.name,
            'age': self.age,
            'age_group': self.age_group,
            'is_adult': self.is_adult(self.age)
        }


class Student(Person):
    """Inheritance example - Student extends Person."""
    
    def __init__(self, name: str, age: int, student_id: str, grade: float = 0.0):
        """Initialize a Student instance."""
        super().__init__(name, age)
        self.student_id = student_id
        self.grade = grade
        self._courses = []
    
    def study(self, subject: str) -> str:
        """Student-specific method."""
        return f"{self.name} is studying {subject}"
    
    def enroll(self, course: str):
        """Enroll in a course."""
        self._courses.append(course)
    
    @property
    def courses(self) -> List[str]:
        """Get enrolled courses."""
        return self._courses.copy()
    
    @property
    def info(self) -> dict:
        """Override parent property."""
        parent_info = super().info
        parent_info.update({
            'student_id': self.student_id,
            'grade': self.grade,
            'courses': self.courses
        })
        return parent_info


class BankAccount:
    """Example of encapsulation and private attributes."""
    
    def __init__(self, account_number: str, initial_balance: float = 0):
        """Initialize bank account."""
        self._account_number = account_number
        self.__balance = initial_balance  # Name mangling for privacy
        self._transaction_history = []
    
    def deposit(self, amount: float) -> float:
        """Deposit money to account."""
        if amount <= 0:
            raise ValueError("Deposit amount must be positive")
        self.__balance += amount
        self._transaction_history.append(('deposit', amount))
        return self.__balance
    
    def withdraw(self, amount: float) -> float:
        """Withdraw money from account."""
        if amount <= 0:
            raise ValueError("Withdrawal amount must be positive")
        if amount > self.__balance:
            raise ValueError("Insufficient funds")
        self.__balance -= amount
        self._transaction_history.append(('withdrawal', amount))
        return self.__balance
    
    @property
    def balance(self) -> float:
        """Get current balance (read-only)."""
        return self.__balance
    
    @property
    def account_number(self) -> str:
        """Get masked account number."""
        return f"***{self._account_number[-4:]}"
    
    def get_statement(self) -> str:
        """Get account statement."""
        statement = f"Account: {self.account_number}\n"
        statement += f"Current Balance: ${self.balance:.2f}\n"
        statement += "Recent Transactions:\n"
        for trans_type, amount in self._transaction_history[-5:]:
            statement += f"  - {trans_type}: ${amount:.2f}\n"
        return statement


class Shape:
    """Abstract base class for shapes."""
    
    def __init__(self, color: str = "white"):
        """Initialize shape with color."""
        self.color = color
    
    def area(self) -> float:
        """Calculate area - to be implemented by subclasses."""
        raise NotImplementedError("Subclasses must implement area()")
    
    def perimeter(self) -> float:
        """Calculate perimeter - to be implemented by subclasses."""
        raise NotImplementedError("Subclasses must implement perimeter()")
    
    def describe(self) -> str:
        """Describe the shape."""
        return f"A {self.color} {self.__class__.__name__} with area {self.area():.2f}"


class Rectangle(Shape):
    """Rectangle implementation."""
    
    def __init__(self, width: float, height: float, color: str = "white"):
        """Initialize rectangle."""
        super().__init__(color)
        self.width = width
        self.height = height
    
    def area(self) -> float:
        """Calculate rectangle area."""
        return self.width * self.height
    
    def perimeter(self) -> float:
        """Calculate rectangle perimeter."""
        return 2 * (self.width + self.height)


class Circle(Shape):
    """Circle implementation."""
    
    def __init__(self, radius: float, color: str = "white"):
        """Initialize circle."""
        super().__init__(color)
        self.radius = radius
    
    def area(self) -> float:
        """Calculate circle area."""
        import math
        return math.pi * self.radius ** 2
    
    def perimeter(self) -> float:
        """Calculate circle perimeter (circumference)."""
        import math
        return 2 * math.pi * self.radius


class DataClass:
    """Example of a data class with special methods."""
    
    def __init__(self, data: List[Any]):
        """Initialize with data."""
        self._data = data
    
    def __len__(self) -> int:
        """Return length of data."""
        return len(self._data)
    
    def __getitem__(self, index: int) -> Any:
        """Get item by index."""
        return self._data[index]
    
    def __setitem__(self, index: int, value: Any):
        """Set item by index."""
        self._data[index] = value
    
    def __contains__(self, item: Any) -> bool:
        """Check if item is in data."""
        return item in self._data
    
    def __iter__(self):
        """Make class iterable."""
        return iter(self._data)
    
    def __add__(self, other):
        """Add two DataClass instances."""
        if isinstance(other, DataClass):
            return DataClass(self._data + other._data)
        return NotImplemented


# ============= METACLASSES AND ADVANCED =============

class Singleton(type):
    """Metaclass for creating singleton classes."""
    _instances = {}
    
    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super().__call__(*args, **kwargs)
        return cls._instances[cls]


class DatabaseConnection(metaclass=Singleton):
    """Example singleton class using metaclass."""
    
    def __init__(self):
        """Initialize database connection."""
        self.connected = False
        self.connection_string = "localhost:5432"
    
    def connect(self):
        """Connect to database."""
        self.connected = True
        return "Connected to database"
    
    def disconnect(self):
        """Disconnect from database."""
        self.connected = False
        return "Disconnected from database"


# ============= DEMONSTRATION FUNCTIONS =============

def demonstrate_functions():
    """Demonstrate various function features."""
    results = {}
    
    # Basic function
    results['basic'] = basic_function("Python")
    
    # Default parameters
    results['default_used'] = function_with_defaults(3)
    results['default_overridden'] = function_with_defaults(2, 4)
    
    # Variable arguments
    results['var_args'] = function_with_args(1, 2, 3, 4, 5)
    
    # Keyword arguments
    results['kwargs'] = function_with_kwargs(name="Alice", age=30, city="NYC")
    
    # All parameter types
    results['all_params'] = function_with_all("required", "custom", 1, 2, x=10, y=20)
    
    # Lambda functions
    results['lambda_square'] = square(5)
    results['lambda_add'] = add(3, 7)
    results['lambda_filter'] = list(filter(is_even, range(10)))
    
    return results


def demonstrate_decorators():
    """Demonstrate decorator usage."""
    results = {}
    
    # Timer decorator
    slow_result = slow_function(0.01)
    results['timed_function'] = {
        'result': slow_result,
        'execution_time': slow_function.execution_time
    }
    
    # Memoization
    fib_10 = fibonacci(10)
    fib_10_again = fibonacci(10)  # Should be cached
    results['memoized'] = {
        'fibonacci_10': fib_10,
        'cache_hits': getattr(fibonacci, 'cache_hits', 0),
        'cache_misses': getattr(fibonacci, 'cache_misses', 0)
    }
    
    # Validation decorator
    try:
        area = calculate_area(5, 3)
        results['valid_area'] = area
    except ValueError:
        results['valid_area'] = "Error"
    
    try:
        invalid_area = calculate_area(-5, 3)
        results['invalid_area'] = invalid_area
    except ValueError as e:
        results['invalid_area'] = str(e)
    
    # Repeated decorator
    results['repeated'] = get_random_number()
    
    return results


def demonstrate_classes():
    """Demonstrate class features."""
    results = {}
    
    # Basic class
    person = Person("Alice", 30)
    results['person'] = {
        'str': str(person),
        'repr': repr(person),
        'introduce': person.introduce(),
        'age_group': person.age_group,
        'info': person.info
    }
    
    # Class method
    person2 = Person.from_birth_year("Bob", 1990)
    results['from_birth_year'] = person2.info
    
    # Inheritance
    student = Student("Charlie", 20, "S12345", 85.5)
    student.enroll("Math")
    student.enroll("Physics")
    results['student'] = {
        'study': student.study("Python"),
        'courses': student.courses,
        'info': student.info
    }
    
    # Encapsulation
    account = BankAccount("1234567890", 1000)
    account.deposit(500)
    account.withdraw(200)
    results['bank_account'] = {
        'balance': account.balance,
        'account_number': account.account_number,
        'statement': account.get_statement()
    }
    
    # Polymorphism with shapes
    shapes = [
        Rectangle(5, 3, "red"),
        Circle(4, "blue"),
        Rectangle(2, 8, "green")
    ]
    results['shapes'] = [
        {
            'type': shape.__class__.__name__,
            'area': shape.area(),
            'perimeter': shape.perimeter(),
            'description': shape.describe()
        }
        for shape in shapes
    ]
    
    # Singleton
    db1 = DatabaseConnection()
    db2 = DatabaseConnection()
    results['singleton'] = {
        'same_instance': db1 is db2,
        'id1': id(db1),
        'id2': id(db2)
    }
    
    return results


if __name__ == "__main__":
    # Demonstrate all features when run directly
    print("=== Functions Demo ===")
    print(demonstrate_functions())
    
    print("\n=== Decorators Demo ===")
    print(demonstrate_decorators())
    
    print("\n=== Classes Demo ===")
    print(demonstrate_classes())
    
    print("\n=== Closures Demo ===")
    double = create_multiplier(2)
    triple = create_multiplier(3)
    print(f"Double 5: {double(5)}")
    print(f"Triple 5: {triple(5)}")
    
    print("\n=== Generators Demo ===")
    print("Fibonacci sequence:")
    for num in fibonacci_generator(100):
        print(num, end=" ")
    print()
    
    print("\n=== Special Methods Demo ===")
    dc = DataClass([1, 2, 3, 4, 5])
    print(f"Length: {len(dc)}")
    print(f"Contains 3: {3 in dc}")
    print(f"Items: {list(dc)}")