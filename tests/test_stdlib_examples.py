"""
Tests for the stdlib_examples module.
"""

import pytest
import sys
import os
import math
import datetime
import re
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))

from src import stdlib_examples


class TestOSModule:
    """Test os module demonstrations."""
    
    def test_os_module(self):
        """Test os module functionality."""
        result = stdlib_examples.demonstrate_os_module()
        
        assert 'cwd' in result
        assert os.path.exists(result['cwd'])
        
        path_ops = result['path_operations']
        assert path_ops['dirname'] == '/home/user/documents'
        assert path_ops['basename'] == 'file.txt'
        assert path_ops['split'] == ('/home/user/documents', 'file.txt')
        assert path_ops['splitext'] == ('/home/user/documents/file', '.txt')
        
        sys_info = result['system_info']
        assert 'platform' in sys_info
        assert 'python_version' in sys_info
        
        proc_info = result['process_info']
        assert proc_info['pid'] > 0
        assert proc_info['cpu_count'] > 0


class TestDateTimeModule:
    """Test datetime module demonstrations."""
    
    def test_datetime_module(self):
        """Test datetime module functionality."""
        result = stdlib_examples.demonstrate_datetime_module()
        
        # Check current date/time
        assert 'current' in result
        assert 'now' in result['current']
        assert 'today' in result['current']
        
        # Check formatting
        formatting = result['formatting']
        assert 'custom' in formatting
        assert 'readable' in formatting
        assert 'iso' in formatting
        
        # Check parsing
        assert '2023-12-25' in result['parsed']
        
        # Check arithmetic
        arithmetic = result['arithmetic']
        assert 'tomorrow' in arithmetic
        assert 'next_week' in arithmetic
        assert arithmetic['days_until_new_year'] >= 0
        
        # Check components
        components = result['components']
        assert 0 <= components['month'] <= 12
        assert 0 <= components['hour'] <= 23
        assert 0 <= components['weekday'] <= 6


class TestMathModule:
    """Test math module demonstrations."""
    
    def test_math_module(self):
        """Test math module functionality."""
        result = stdlib_examples.demonstrate_math_module()
        
        # Constants
        constants = result['constants']
        assert constants['pi'] == pytest.approx(3.14159, rel=0.0001)
        assert constants['e'] == pytest.approx(2.71828, rel=0.0001)
        assert constants['tau'] == pytest.approx(6.28318, rel=0.0001)
        
        # Basic functions
        basic = result['basic']
        assert basic['sqrt_16'] == 4.0
        assert basic['pow_2_3'] == 8.0
        assert basic['factorial_5'] == 120
        assert basic['gcd_48_18'] == 6
        
        # Rounding
        rounding = result['rounding']
        assert rounding['ceil_4.3'] == 5
        assert rounding['floor_4.7'] == 4
        assert rounding['trunc_4.7'] == 4
        
        # Logarithms
        logs = result['logarithms']
        assert logs['log_e'] == pytest.approx(1.0)
        assert logs['log10_100'] == pytest.approx(2.0)
        assert logs['log2_8'] == pytest.approx(3.0)
        
        # Trigonometry
        trig = result['trigonometry']
        assert trig['sin_45'] == pytest.approx(0.7071, rel=0.001)
        assert trig['cos_45'] == pytest.approx(0.7071, rel=0.001)
        assert trig['radians_to_degrees'] == pytest.approx(180.0)
        
        # Special functions
        special = result['special']
        assert special['isnan'] is True
        assert special['isinf'] is True
        assert special['isfinite'] is True
        assert special['hypot'] == 5.0


class TestRandomModule:
    """Test random module demonstrations."""
    
    def test_random_module(self):
        """Test random module functionality."""
        result = stdlib_examples.demonstrate_random_module()
        
        # Basic random
        basic = result['basic']
        assert 0 <= basic['random'] <= 1
        assert 1 <= basic['uniform'] <= 10
        assert 1 <= basic['randint'] <= 10
        assert basic['randrange'] in [0, 2, 4, 6, 8]
        
        # Choice
        choice = result['choice']
        fruits = ['apple', 'banana', 'orange', 'grape', 'kiwi']
        assert choice['single'] in fruits
        assert len(choice['multiple']) == 3
        assert len(choice['sample']) == 3
        assert len(set(choice['sample'])) == 3  # No duplicates
        
        # Shuffled
        assert len(result['shuffled']) == 10
        assert set(result['shuffled']) == set(range(1, 11))
        
        # Random string
        assert len(result['random_string']) == 10
        
        # Secure random
        secure = result['secure']
        assert len(secure['token_hex']) == 32  # 16 bytes = 32 hex chars
        assert secure['secure_choice'] in fruits


class TestRegex:
    """Test regular expression demonstrations."""
    
    def test_regex(self):
        """Test regex operations."""
        result = stdlib_examples.demonstrate_regex()
        
        assert result['basic_match'] == 'quick brown fox'
        assert result['emails'] == ['john@email.com']
        assert result['phones'] == ['555-1234']
        assert 'XXX-XXXX' in result['phone_masked']
        assert result['words'][:3] == ['The', 'quick', 'brown']
        
        groups = result['groups']
        assert groups['username'] == 'john'
        assert groups['domain'] == 'email.com'
        
        named = result['named_groups']
        assert named['username'] == 'john'
        assert named['domain'] == 'email.com'
        
        assert 'quick' in result['five_letter_words']
        assert 'brown' in result['five_letter_words']
        assert result['case_insensitive_the'] == 2


class TestItertools:
    """Test itertools demonstrations."""
    
    def test_itertools(self):
        """Test itertools functionality."""
        result = stdlib_examples.demonstrate_itertools()
        
        assert result['count'] == [1, 3, 5, 7, 9]
        assert result['cycle'] == ['A', 'B', 'C', 'A', 'B', 'C', 'A']
        assert result['repeat'] == ['X', 'X', 'X']
        
        assert len(result['permutations']) == 6
        assert ('A', 'B') in result['permutations']
        
        assert len(result['combinations']) == 3
        assert ('A', 'B') in result['combinations']
        
        assert result['product'] == [(1, 'A'), (1, 'B'), (2, 'A'), (2, 'B')]
        assert result['accumulate'] == [1, 3, 6, 10, 15]
        assert result['chain'] == [1, 2, 3, 4, 5, 6]
        assert result['compress'] == ['A', 'C', 'E', 'F']
        
        groupby = result['groupby']
        assert groupby[0][0] == 'A'
        assert len(groupby[0][1]) == 2
        
        assert result['filterfalse'] == [0, 2, 4, 6, 8]
        assert result['starmap'] == [8, 9, 100]
        assert result['islice'] == [5, 7, 9, 11, 13]
        
        tee = result['tee']
        assert tee['iter1'] == [0, 1, 2, 3, 4]
        assert tee['iter2'] == [0, 1, 2, 3, 4]


class TestFunctools:
    """Test functools demonstrations."""
    
    def test_functools(self):
        """Test functools functionality."""
        result = stdlib_examples.demonstrate_functools()
        
        # Partial
        partial = result['partial']
        assert partial['double_5'] == 10
        assert partial['triple_5'] == 15
        
        # Reduce
        reduce_res = result['reduce']
        assert reduce_res['sum'] == 15
        assert reduce_res['product'] == 120
        
        # LRU Cache
        cache = result['lru_cache']
        assert cache['result'] == 25
        assert float(cache['first_call_time'][:-1]) >= 0.1
        assert float(cache['cached_call_time'][:-1]) < 0.01
        
        # Wraps
        wraps = result['wraps']
        assert wraps['name'] == 'example_function'
        assert 'Example function' in wraps['doc']
        
        # Total ordering
        ordering = result['total_ordering']
        assert ordering['s1_lt_s2'] is True
        assert ordering['s1_gt_s2'] is False
        assert ordering['s1_le_s2'] is True
        assert ordering['s1_ge_s2'] is False


class TestStatistics:
    """Test statistics module demonstrations."""
    
    def test_statistics(self):
        """Test statistics functionality."""
        result = stdlib_examples.demonstrate_statistics()
        
        # Central tendency
        central = result['central']
        assert central['mean'] == pytest.approx(7.0)
        assert central['median'] == 7
        assert central['mode'] in [4, 10]  # Multiple modes possible
        
        # Spread
        spread = result['spread']
        assert spread['stdev'] > 0
        assert spread['variance'] > 0
        assert len(spread['quantiles']) == 3  # Quartiles split into 4 parts
        
        # Additional
        if 'additional' in result:
            assert result['additional']['harmonic_mean'] > 0
        
        # Correlation
        if 'correlation' in result:
            assert result['correlation'] == pytest.approx(1.0)  # Perfect correlation


class TestDecimal:
    """Test decimal module demonstrations."""
    
    def test_decimal(self):
        """Test decimal functionality."""
        result = stdlib_examples.demonstrate_decimal()
        
        # Basic
        basic = result['basic']
        assert basic['float_addition'] != 0.3  # Float imprecision
        assert basic['decimal_addition'] == '0.3'  # Decimal precision
        assert basic['multiplication'] == '9.25'
        
        # High precision
        assert '3.14159' in result['high_precision_pi']
        
        # Rounding
        rounding = result['rounding']
        assert rounding['quantize_2'] == '3.14'
        assert rounding['quantize_3'] == '3.142'
        
        # Special values
        special = result['special']
        assert special['infinity'] == 'Infinity'
        assert special['nan'] == 'NaN'


class TestHashlib:
    """Test hashlib module demonstrations."""
    
    def test_hashlib(self):
        """Test hashlib functionality."""
        result = stdlib_examples.demonstrate_hashlib()
        
        # Hashes
        hashes = result['hashes']
        assert len(hashes['md5']) == 32
        assert len(hashes['sha1']) == 40
        assert len(hashes['sha256']) == 64
        assert len(hashes['sha512']) == 128
        
        # Incremental hash should match direct hash
        assert result['incremental'] == hashes['sha256']
        
        # File hash
        assert len(result['file_hash']) == 64
        
        # HMAC
        assert len(result['hmac']) == 64
        
        # Base64
        base64 = result['base64']
        assert base64['decoded'] == 'Hello, Python!'


class TestSubprocess:
    """Test subprocess module demonstrations."""
    
    def test_subprocess(self):
        """Test subprocess functionality."""
        result = stdlib_examples.demonstrate_subprocess()
        
        if 'error' not in result:
            # Echo command
            if 'echo' in result:
                assert 'Hello from subprocess' in result['echo']['stdout']
                assert result['echo']['returncode'] == 0
            
            # Python version
            if 'python_version' in result:
                assert 'Python' in result['python_version'] or 'python' in result['python_version'].lower()


class TestFractions:
    """Test fractions module demonstrations."""
    
    def test_fractions(self):
        """Test fractions functionality."""
        result = stdlib_examples.demonstrate_fractions()
        
        # Creation
        creation = result['creation']
        assert creation['from_ints'] == '3/4'
        assert creation['from_string_decimal'] == '3/4'
        assert creation['from_string_fraction'] == '1/3'
        
        # Arithmetic
        arithmetic = result['arithmetic']
        assert arithmetic['addition'] == '5/4'
        assert arithmetic['subtraction'] == '1/4'
        assert arithmetic['multiplication'] == '3/8'
        assert arithmetic['division'] == '3/2'
        
        # Properties
        props = result['properties']
        assert props['numerator'] == 3
        assert props['denominator'] == 4
        
        # Conversion
        conversion = result['conversion']
        assert conversion['to_float'] == 0.75


if __name__ == "__main__":
    pytest.main([__file__, "-v"])