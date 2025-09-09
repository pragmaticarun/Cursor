"""
Standard Library Examples

This module demonstrates commonly used Python standard library modules.
"""

import os
import sys
import math
import random
import datetime
import time
import re
import itertools
import functools
import subprocess
import urllib.request
import urllib.parse
import hashlib
import base64
import secrets
import statistics
import decimal
import fractions
from typing import List, Dict, Any, Optional


# ============= OS MODULE =============

def demonstrate_os_module():
    """Demonstrate os module functionality."""
    results = {}
    
    # Environment variables
    results['python_path'] = os.environ.get('PYTHONPATH', 'Not set')
    results['home_dir'] = os.environ.get('HOME', os.environ.get('USERPROFILE', 'Not found'))
    
    # Current working directory
    results['cwd'] = os.getcwd()
    
    # Path operations
    path = '/home/user/documents/file.txt'
    results['path_operations'] = {
        'dirname': os.path.dirname(path),
        'basename': os.path.basename(path),
        'split': os.path.split(path),
        'splitext': os.path.splitext(path),
        'join': os.path.join('folder', 'subfolder', 'file.txt')
    }
    
    # System information
    results['system_info'] = {
        'platform': sys.platform,
        'python_version': sys.version.split()[0],
        'path_separator': os.pathsep,
        'line_separator_repr': repr(os.linesep)
    }
    
    # Process information
    results['process_info'] = {
        'pid': os.getpid(),
        'cpu_count': os.cpu_count()
    }
    
    return results


# ============= DATETIME MODULE =============

def demonstrate_datetime_module():
    """Demonstrate datetime module functionality."""
    from datetime import datetime, date, time, timedelta, timezone
    
    results = {}
    
    # Current date and time
    now = datetime.now()
    today = date.today()
    current_time = datetime.now().time()
    
    results['current'] = {
        'now': now.isoformat(),
        'today': today.isoformat(),
        'time': current_time.isoformat()
    }
    
    # Creating specific dates
    birthday = datetime(1990, 5, 15, 10, 30, 45)
    results['specific_date'] = birthday.isoformat()
    
    # Formatting dates
    results['formatting'] = {
        'default': str(now),
        'custom': now.strftime("%Y-%m-%d %H:%M:%S"),
        'readable': now.strftime("%B %d, %Y at %I:%M %p"),
        'iso': now.isoformat(),
        'date_only': now.strftime("%Y-%m-%d"),
        'time_only': now.strftime("%H:%M:%S")
    }
    
    # Parsing dates
    date_string = "2023-12-25 15:30:00"
    parsed = datetime.strptime(date_string, "%Y-%m-%d %H:%M:%S")
    results['parsed'] = parsed.isoformat()
    
    # Date arithmetic
    tomorrow = today + timedelta(days=1)
    next_week = today + timedelta(weeks=1)
    past_date = today - timedelta(days=30)
    
    results['arithmetic'] = {
        'tomorrow': tomorrow.isoformat(),
        'next_week': next_week.isoformat(),
        'past_date': past_date.isoformat(),
        'days_until_new_year': (date(today.year + 1, 1, 1) - today).days
    }
    
    # Time zones (basic)
    utc_now = datetime.now(timezone.utc)
    results['timezone'] = {
        'utc': utc_now.isoformat(),
        'local': now.isoformat(),
        'offset': time.strftime('%z')
    }
    
    # Date components
    results['components'] = {
        'year': now.year,
        'month': now.month,
        'day': now.day,
        'hour': now.hour,
        'minute': now.minute,
        'second': now.second,
        'microsecond': now.microsecond,
        'weekday': now.weekday(),  # 0 = Monday
        'isoweekday': now.isoweekday()  # 1 = Monday
    }
    
    return results


# ============= MATH MODULE =============

def demonstrate_math_module():
    """Demonstrate math module functionality."""
    results = {}
    
    # Constants
    results['constants'] = {
        'pi': math.pi,
        'e': math.e,
        'tau': math.tau,  # 2π
        'inf': math.inf,
        'nan': math.nan
    }
    
    # Basic functions
    results['basic'] = {
        'sqrt_16': math.sqrt(16),
        'pow_2_3': math.pow(2, 3),
        'exp_1': math.exp(1),  # e^1
        'abs_neg5': math.fabs(-5),
        'factorial_5': math.factorial(5),
        'gcd_48_18': math.gcd(48, 18),
        'lcm_12_18': math.lcm(12, 18) if hasattr(math, 'lcm') else 'N/A'
    }
    
    # Rounding functions
    results['rounding'] = {
        'ceil_4.3': math.ceil(4.3),
        'floor_4.7': math.floor(4.7),
        'trunc_4.7': math.trunc(4.7),
        'round_4.5': round(4.5),  # Built-in
        'round_4.6': round(4.6)
    }
    
    # Logarithmic functions
    results['logarithms'] = {
        'log_e': math.log(math.e),  # Natural log
        'log10_100': math.log10(100),
        'log2_8': math.log2(8),
        'log_base_3_27': math.log(27, 3)
    }
    
    # Trigonometric functions (in radians)
    angle = math.pi / 4  # 45 degrees
    results['trigonometry'] = {
        'sin_45': math.sin(angle),
        'cos_45': math.cos(angle),
        'tan_45': math.tan(angle),
        'degrees_to_radians': math.radians(180),
        'radians_to_degrees': math.degrees(math.pi)
    }
    
    # Hyperbolic functions
    results['hyperbolic'] = {
        'sinh_1': math.sinh(1),
        'cosh_1': math.cosh(1),
        'tanh_1': math.tanh(1)
    }
    
    # Special functions
    results['special'] = {
        'isnan': math.isnan(float('nan')),
        'isinf': math.isinf(float('inf')),
        'isfinite': math.isfinite(100),
        'copysign': math.copysign(5, -1),  # Copy sign of second to first
        'hypot': math.hypot(3, 4),  # Euclidean distance
        'dist': math.dist([1, 2], [4, 6])  # Distance between points
    }
    
    return results


# ============= RANDOM MODULE =============

def demonstrate_random_module():
    """Demonstrate random module functionality."""
    # Set seed for reproducibility
    random.seed(42)
    
    results = {}
    
    # Basic random numbers
    results['basic'] = {
        'random': random.random(),  # 0.0 to 1.0
        'uniform': random.uniform(1, 10),  # Float in range
        'randint': random.randint(1, 10),  # Int inclusive
        'randrange': random.randrange(0, 10, 2)  # Even number 0-8
    }
    
    # Random choice
    fruits = ['apple', 'banana', 'orange', 'grape', 'kiwi']
    results['choice'] = {
        'single': random.choice(fruits),
        'multiple': random.choices(fruits, k=3),  # With replacement
        'sample': random.sample(fruits, 3),  # Without replacement
        'weighted': random.choices(fruits, weights=[10, 5, 5, 2, 1], k=3)
    }
    
    # Shuffling
    numbers = list(range(1, 11))
    random.shuffle(numbers)
    results['shuffled'] = numbers
    
    # Random distributions
    results['distributions'] = {
        'gauss': random.gauss(0, 1),  # Normal distribution
        'triangular': random.triangular(0, 10, 5),  # Triangular
        'betavariate': random.betavariate(2, 5),  # Beta
        'expovariate': random.expovariate(1/5),  # Exponential
        'gammavariate': random.gammavariate(2, 1)  # Gamma
    }
    
    # Random string generation
    import string
    
    def generate_random_string(length=10):
        """Generate random string."""
        return ''.join(random.choices(string.ascii_letters + string.digits, k=length))
    
    results['random_string'] = generate_random_string(10)
    
    # Secure random (for cryptographic use)
    results['secure'] = {
        'token_hex': secrets.token_hex(16),
        'token_urlsafe': secrets.token_urlsafe(16),
        'randbits': secrets.randbits(32),
        'secure_choice': secrets.choice(fruits)
    }
    
    return results


# ============= REGULAR EXPRESSIONS =============

def demonstrate_regex():
    """Demonstrate regular expression operations."""
    results = {}
    
    text = "The quick brown fox jumps over the lazy dog. Contact: john@email.com or call 555-1234."
    
    # Basic matching
    pattern = r'quick.*fox'
    match = re.search(pattern, text)
    results['basic_match'] = match.group() if match else None
    
    # Finding all matches
    email_pattern = r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b'
    emails = re.findall(email_pattern, text)
    results['emails'] = emails
    
    # Phone number pattern
    phone_pattern = r'\d{3}-\d{4}'
    phones = re.findall(phone_pattern, text)
    results['phones'] = phones
    
    # Substitution
    cleaned = re.sub(r'\d{3}-\d{4}', 'XXX-XXXX', text)
    results['phone_masked'] = cleaned
    
    # Splitting
    words = re.split(r'\s+', text)
    results['words'] = words[:5]  # First 5 words
    
    # Groups
    pattern_with_groups = r'(\w+)@(\w+\.\w+)'
    match = re.search(pattern_with_groups, text)
    if match:
        results['groups'] = {
            'full': match.group(0),
            'username': match.group(1),
            'domain': match.group(2)
        }
    
    # Named groups
    named_pattern = r'(?P<username>\w+)@(?P<domain>\w+\.\w+)'
    match = re.search(named_pattern, text)
    if match:
        results['named_groups'] = match.groupdict()
    
    # Compiled patterns (more efficient for repeated use)
    compiled = re.compile(r'\b\w{5}\b')  # 5-letter words
    five_letter_words = compiled.findall(text)
    results['five_letter_words'] = five_letter_words
    
    # Flags
    case_insensitive = re.findall(r'the', text, re.IGNORECASE)
    results['case_insensitive_the'] = len(case_insensitive)
    
    return results


# ============= ITERTOOLS MODULE =============

def demonstrate_itertools():
    """Demonstrate itertools functionality."""
    results = {}
    
    # Infinite iterators
    counter = itertools.count(start=1, step=2)
    results['count'] = [next(counter) for _ in range(5)]
    
    cycler = itertools.cycle(['A', 'B', 'C'])
    results['cycle'] = [next(cycler) for _ in range(7)]
    
    repeater = itertools.repeat('X', 3)
    results['repeat'] = list(repeater)
    
    # Combinatoric iterators
    items = ['A', 'B', 'C']
    results['permutations'] = list(itertools.permutations(items, 2))
    results['combinations'] = list(itertools.combinations(items, 2))
    results['combinations_with_replacement'] = list(itertools.combinations_with_replacement(items, 2))
    results['product'] = list(itertools.product([1, 2], ['A', 'B']))
    
    # Terminating iterators
    results['accumulate'] = list(itertools.accumulate([1, 2, 3, 4, 5]))
    results['chain'] = list(itertools.chain([1, 2], [3, 4], [5, 6]))
    results['compress'] = list(itertools.compress('ABCDEF', [1, 0, 1, 0, 1, 1]))
    
    # Groupby
    data = [('A', 1), ('A', 2), ('B', 3), ('B', 4), ('C', 5)]
    grouped = []
    for key, group in itertools.groupby(data, key=lambda x: x[0]):
        grouped.append((key, list(group)))
    results['groupby'] = grouped
    
    # Filter and map alternatives
    results['filterfalse'] = list(itertools.filterfalse(lambda x: x % 2, range(10)))
    results['starmap'] = list(itertools.starmap(pow, [(2, 3), (3, 2), (10, 2)]))
    
    # Slicing iterators
    results['islice'] = list(itertools.islice(range(100), 5, 15, 2))
    
    # Tee - split iterator
    iter1, iter2 = itertools.tee(range(5))
    results['tee'] = {
        'iter1': list(iter1),
        'iter2': list(iter2)
    }
    
    return results


# ============= FUNCTOOLS MODULE =============

def demonstrate_functools():
    """Demonstrate functools functionality."""
    results = {}
    
    # Partial functions
    def multiply(x, y):
        return x * y
    
    double = functools.partial(multiply, 2)
    triple = functools.partial(multiply, 3)
    
    results['partial'] = {
        'double_5': double(5),
        'triple_5': triple(5)
    }
    
    # Reduce
    numbers = [1, 2, 3, 4, 5]
    sum_result = functools.reduce(lambda x, y: x + y, numbers)
    product_result = functools.reduce(lambda x, y: x * y, numbers)
    
    results['reduce'] = {
        'sum': sum_result,
        'product': product_result
    }
    
    # LRU Cache
    @functools.lru_cache(maxsize=128)
    def expensive_function(n):
        """Simulate expensive computation."""
        time.sleep(0.1)  # Simulate delay
        return n * n
    
    # First call is slow
    start = time.time()
    result1 = expensive_function(5)
    time1 = time.time() - start
    
    # Second call is cached (fast)
    start = time.time()
    result2 = expensive_function(5)
    time2 = time.time() - start
    
    results['lru_cache'] = {
        'result': result1,
        'first_call_time': f"{time1:.4f}s",
        'cached_call_time': f"{time2:.4f}s",
        'cache_info': str(expensive_function.cache_info())
    }
    
    # Wraps decorator
    def my_decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            return func(*args, **kwargs)
        return wrapper
    
    @my_decorator
    def example_function():
        """Example function docstring."""
        return "example"
    
    results['wraps'] = {
        'name': example_function.__name__,
        'doc': example_function.__doc__
    }
    
    # Total ordering
    @functools.total_ordering
    class Student:
        def __init__(self, name, grade):
            self.name = name
            self.grade = grade
        
        def __eq__(self, other):
            return self.grade == other.grade
        
        def __lt__(self, other):
            return self.grade < other.grade
    
    s1 = Student("Alice", 85)
    s2 = Student("Bob", 90)
    
    results['total_ordering'] = {
        's1_lt_s2': s1 < s2,
        's1_gt_s2': s1 > s2,
        's1_le_s2': s1 <= s2,
        's1_ge_s2': s1 >= s2
    }
    
    return results


# ============= STATISTICS MODULE =============

def demonstrate_statistics():
    """Demonstrate statistics module functionality."""
    results = {}
    
    data = [2, 4, 4, 4, 5, 5, 7, 9, 10, 10, 10, 11, 13]
    
    # Measures of central tendency
    results['central'] = {
        'mean': statistics.mean(data),
        'median': statistics.median(data),
        'mode': statistics.mode(data),
        'median_low': statistics.median_low(data),
        'median_high': statistics.median_high(data)
    }
    
    # Measures of spread
    results['spread'] = {
        'stdev': statistics.stdev(data),  # Sample standard deviation
        'pstdev': statistics.pstdev(data),  # Population standard deviation
        'variance': statistics.variance(data),  # Sample variance
        'pvariance': statistics.pvariance(data),  # Population variance
        'quantiles': statistics.quantiles(data, n=4)  # Quartiles
    }
    
    # Additional statistics
    if hasattr(statistics, 'harmonic_mean'):
        results['additional'] = {
            'harmonic_mean': statistics.harmonic_mean(data),
            'geometric_mean': statistics.geometric_mean(data) if hasattr(statistics, 'geometric_mean') else 'N/A'
        }
    
    # Correlation (if available)
    x = [1, 2, 3, 4, 5]
    y = [2, 4, 6, 8, 10]
    if hasattr(statistics, 'correlation'):
        results['correlation'] = statistics.correlation(x, y)
    
    return results


# ============= DECIMAL MODULE =============

def demonstrate_decimal():
    """Demonstrate decimal module for precise arithmetic."""
    from decimal import Decimal, getcontext
    
    results = {}
    
    # Set precision
    getcontext().prec = 10
    
    # Basic decimal operations
    a = Decimal('0.1')
    b = Decimal('0.2')
    c = a + b
    
    results['basic'] = {
        'float_addition': 0.1 + 0.2,  # Imprecise
        'decimal_addition': str(c),  # Precise
        'multiplication': str(Decimal('2.5') * Decimal('3.7')),
        'division': str(Decimal('10') / Decimal('3'))
    }
    
    # Precision control
    getcontext().prec = 50
    pi = Decimal('3.1415926535897932384626433832795028841971693993751')
    results['high_precision_pi'] = str(pi)
    
    # Rounding
    getcontext().prec = 6
    num = Decimal('3.14159')
    results['rounding'] = {
        'original': str(num),
        'quantize_2': str(num.quantize(Decimal('0.01'))),
        'quantize_3': str(num.quantize(Decimal('0.001')))
    }
    
    # Special values
    results['special'] = {
        'infinity': str(Decimal('Infinity')),
        'neg_infinity': str(Decimal('-Infinity')),
        'nan': str(Decimal('NaN'))
    }
    
    return results


# ============= HASHLIB MODULE =============

def demonstrate_hashlib():
    """Demonstrate hashlib for cryptographic hashing."""
    results = {}
    
    text = "Hello, Python!"
    binary_data = text.encode('utf-8')
    
    # Common hash algorithms
    results['hashes'] = {
        'md5': hashlib.md5(binary_data).hexdigest(),
        'sha1': hashlib.sha1(binary_data).hexdigest(),
        'sha256': hashlib.sha256(binary_data).hexdigest(),
        'sha512': hashlib.sha512(binary_data).hexdigest()
    }
    
    # Update hash incrementally
    hasher = hashlib.sha256()
    hasher.update(b"Hello, ")
    hasher.update(b"Python!")
    results['incremental'] = hasher.hexdigest()
    
    # File hashing
    def hash_file_content(content: bytes) -> str:
        """Hash file content."""
        return hashlib.sha256(content).hexdigest()
    
    file_content = b"File content to hash"
    results['file_hash'] = hash_file_content(file_content)
    
    # HMAC for message authentication
    import hmac
    
    key = b'secret_key'
    message = b'Important message'
    signature = hmac.new(key, message, hashlib.sha256).hexdigest()
    results['hmac'] = signature
    
    # Base64 encoding
    encoded = base64.b64encode(binary_data).decode('ascii')
    decoded = base64.b64decode(encoded).decode('utf-8')
    
    results['base64'] = {
        'encoded': encoded,
        'decoded': decoded
    }
    
    return results


# ============= SUBPROCESS MODULE =============

def demonstrate_subprocess():
    """Demonstrate subprocess for running external commands."""
    results = {}
    
    try:
        # Run simple command
        result = subprocess.run(['echo', 'Hello from subprocess'], 
                              capture_output=True, text=True)
        results['echo'] = {
            'stdout': result.stdout.strip(),
            'returncode': result.returncode
        }
        
        # Run with shell (platform dependent)
        if sys.platform != 'win32':
            result = subprocess.run('echo $HOME', shell=True, 
                                  capture_output=True, text=True)
            results['shell_command'] = result.stdout.strip()
        
        # Get Python version
        result = subprocess.run([sys.executable, '--version'], 
                              capture_output=True, text=True)
        results['python_version'] = result.stdout.strip() or result.stderr.strip()
        
        # List directory (platform dependent)
        cmd = 'dir' if sys.platform == 'win32' else 'ls'
        result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
        output_lines = result.stdout.strip().split('\n')
        results['directory_listing'] = output_lines[:3]  # First 3 lines
        
    except Exception as e:
        results['error'] = str(e)
    
    return results


# ============= FRACTIONS MODULE =============

def demonstrate_fractions():
    """Demonstrate fractions module for rational numbers."""
    results = {}
    
    # Creating fractions
    f1 = fractions.Fraction(3, 4)
    f2 = fractions.Fraction(1, 2)
    f3 = fractions.Fraction('0.75')
    f4 = fractions.Fraction('1/3')
    
    results['creation'] = {
        'from_ints': str(f1),
        'from_string_decimal': str(f3),
        'from_string_fraction': str(f4)
    }
    
    # Arithmetic operations
    results['arithmetic'] = {
        'addition': str(f1 + f2),
        'subtraction': str(f1 - f2),
        'multiplication': str(f1 * f2),
        'division': str(f1 / f2)
    }
    
    # Properties
    results['properties'] = {
        'numerator': f1.numerator,
        'denominator': f1.denominator,
        'limit_denominator': str(fractions.Fraction(math.pi).limit_denominator(100))
    }
    
    # Conversion
    results['conversion'] = {
        'to_float': float(f1),
        'from_float': str(fractions.Fraction(0.333333333333).limit_denominator())
    }
    
    return results


if __name__ == "__main__":
    # Demonstrate all features when run directly
    print("=== OS Module ===")
    print(demonstrate_os_module())
    
    print("\n=== DateTime Module ===")
    print(demonstrate_datetime_module())
    
    print("\n=== Math Module ===")
    print(demonstrate_math_module())
    
    print("\n=== Random Module ===")
    print(demonstrate_random_module())
    
    print("\n=== Regular Expressions ===")
    print(demonstrate_regex())
    
    print("\n=== Itertools Module ===")
    print(demonstrate_itertools())
    
    print("\n=== Functools Module ===")
    print(demonstrate_functools())
    
    print("\n=== Statistics Module ===")
    print(demonstrate_statistics())
    
    print("\n=== Decimal Module ===")
    print(demonstrate_decimal())
    
    print("\n=== Hashlib Module ===")
    print(demonstrate_hashlib())
    
    print("\n=== Subprocess Module ===")
    print(demonstrate_subprocess())
    
    print("\n=== Fractions Module ===")
    print(demonstrate_fractions())