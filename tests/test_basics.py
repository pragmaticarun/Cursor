"""
Tests for the basics module.
"""

import pytest
import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))

from src import basics


class TestVariables:
    """Test variable demonstrations."""
    
    def test_demonstrate_variables(self):
        """Test variable types demonstration."""
        result = basics.demonstrate_variables()
        
        assert result['integer'] == 42
        assert result['float'] == 3.14159
        assert result['complex'] == 3 + 4j
        assert result['string'] == "Hello World"
        assert result['boolean_true'] is True
        assert result['boolean_false'] is False
        assert result['none'] is None
        assert 'multi-line' in result['multiline']
    
    def test_demonstrate_type_checking(self):
        """Test type checking demonstration."""
        result = basics.demonstrate_type_checking()
        
        assert result['type_of_num'] == 'int'
        assert result['is_int'] is True
        assert result['is_float'] is True
        assert result['str_to_int'] == 123
        assert result['int_to_str'] == '42'
        assert result['float_to_int'] == 3
        assert result['int_to_float'] == 42.0


class TestOperators:
    """Test operator demonstrations."""
    
    def test_arithmetic_operators(self):
        """Test arithmetic operators."""
        result = basics.demonstrate_arithmetic_operators(10, 3)
        
        assert result['addition'] == 13
        assert result['subtraction'] == 7
        assert result['multiplication'] == 30
        assert result['division'] == pytest.approx(3.333333)
        assert result['floor_division'] == 3
        assert result['modulo'] == 1
        assert result['exponentiation'] == 1000
    
    def test_comparison_operators(self):
        """Test comparison operators."""
        result = basics.demonstrate_comparison_operators(10, 3)
        
        assert result['greater_than'] is True
        assert result['less_than'] is False
        assert result['equal'] is False
        assert result['not_equal'] is True
        assert result['greater_equal'] is True
        assert result['less_equal'] is False
    
    def test_logical_operators(self):
        """Test logical operators."""
        result = basics.demonstrate_logical_operators(True, False)
        
        assert result['and'] is False
        assert result['or'] is True
        assert result['not_x'] is False
        assert result['not_y'] is True
    
    def test_bitwise_operators(self):
        """Test bitwise operators."""
        result = basics.demonstrate_bitwise_operators(5, 3)
        
        assert result['and'] == 1
        assert result['or'] == 7
        assert result['xor'] == 6
        assert result['not'] == -6
        assert result['left_shift'] == 10
        assert result['right_shift'] == 2
    
    def test_assignment_operators(self):
        """Test assignment operators."""
        result = basics.demonstrate_assignment_operators()
        
        assert result['initial'] == 10
        assert result['after_add'] == 15
        assert result['after_subtract'] == 12
        assert result['after_multiply'] == 24
        assert result['after_floor_div'] == 6
        assert result['after_power'] == 36
    
    def test_membership_operators(self):
        """Test membership operators."""
        result = basics.demonstrate_membership_operators()
        
        assert result['apple_in_list'] is True
        assert result['grape_in_list'] is False
        assert result['banana_not_in_list'] is False
        assert result['Hello_in_string'] is True
        assert result['bye_in_string'] is False
        assert result['three_in_set'] is True
        assert result['ten_not_in_set'] is True
    
    def test_identity_operators(self):
        """Test identity operators."""
        result = basics.demonstrate_identity_operators()
        
        assert result['a_is_b'] is False
        assert result['a_equals_b'] is True
        assert result['a_is_c'] is True
        assert result['a_is_not_b'] is True
        assert result['none_is_none'] is True
        assert result['empty_list_is_none'] is False


class TestStringOperations:
    """Test string operation demonstrations."""
    
    def test_string_operations(self):
        """Test string operations."""
        result = basics.demonstrate_string_operations()
        
        assert result['length'] == 18
        assert result['uppercase'] == 'PYTHON PROGRAMMING'
        assert result['lowercase'] == 'python programming'
        assert result['starts_with'] is True
        assert result['ends_with'] is True
        assert result['find_index'] == 7
        assert result['replace'] == 'Java Programming'
        assert result['split'] == ['Python', 'Programming']
        assert result['join'] == 'Python-is-awesome'
        assert result['strip'] == 'spaces'
        assert result['substring'] == 'Python'
        assert result['reverse'] == 'gnimmargorP nohtyP'
        assert result['count'] == 2
        assert 'Learning Python Programming is fun!' in result['formatted']
    
    def test_string_formatting(self):
        """Test string formatting."""
        result = basics.demonstrate_string_formatting()
        
        assert 'Alice' in result['f_string']
        assert '30' in result['f_string']
        assert 'Alice' in result['format_method']
        assert 'Alice' in result['format_named']
        assert 'Alice' in result['percent_format']
        assert '3.14' in result['number_format']
        assert len(result['padding']) == 10
        assert result['zero_padding'] == '00030'


if __name__ == "__main__":
    pytest.main([__file__, "-v"])