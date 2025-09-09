"""
Tests for the control_flow module.
"""

import pytest
import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))

from src import control_flow


class TestConditionals:
    """Test conditional demonstrations."""
    
    def test_if_else(self):
        """Test if-else statements."""
        assert control_flow.demonstrate_if_else(5)['category'] == 'Child'
        assert control_flow.demonstrate_if_else(15)['category'] == 'Teenager'
        assert control_flow.demonstrate_if_else(25)['category'] == 'Adult'
        assert control_flow.demonstrate_if_else(65)['category'] == 'Senior'
        assert control_flow.demonstrate_if_else(-5)['category'] == 'Invalid age'
        
        assert control_flow.demonstrate_if_else(17)['legal_status'] == 'Minor'
        assert control_flow.demonstrate_if_else(18)['legal_status'] == 'Adult'
    
    def test_nested_conditions(self):
        """Test nested conditions."""
        result = control_flow.demonstrate_nested_conditions(95)
        assert result['grade'] == 'A'
        assert result['message'] == 'Excellent!'
        
        result = control_flow.demonstrate_nested_conditions(85)
        assert result['grade'] == 'B'
        
        result = control_flow.demonstrate_nested_conditions(75)
        assert result['grade'] == 'C'
        
        result = control_flow.demonstrate_nested_conditions(65)
        assert result['grade'] == 'D'
        
        result = control_flow.demonstrate_nested_conditions(55)
        assert result['grade'] == 'F'
        
        result = control_flow.demonstrate_nested_conditions(105)
        assert result['grade'] == 'Invalid'


class TestLoops:
    """Test loop demonstrations."""
    
    def test_for_loop(self):
        """Test for loop patterns."""
        result = control_flow.demonstrate_for_loop()
        
        assert result['fruits'] == ['APPLE', 'BANANA', 'ORANGE']
        assert result['range_5'] == [0, 1, 2, 3, 4]
        assert result['range_2_10_2'] == [2, 4, 6, 8]
        assert result['enumerated'] == [(0, 'apple'), (1, 'banana'), (2, 'orange')]
        assert result['zipped'] == [('apple', 'red'), ('banana', 'yellow'), ('orange', 'orange')]
        assert len(result['matrix']) == 3
        assert len(result['matrix'][0]) == 3
        assert result['matrix'][1][1] == 4
    
    def test_while_loop(self):
        """Test while loop patterns."""
        result = control_flow.demonstrate_while_loop()
        
        assert result['basic_count'] == [0, 1, 2, 3, 4]
        assert result['with_break'] == [0, 2, 4, 6, 8]
        assert result['fibonacci'][:5] == [0, 1, 1, 2, 3]
        assert all(n < 100 for n in result['fibonacci'])
    
    def test_loop_control(self):
        """Test loop control statements."""
        result = control_flow.demonstrate_loop_control()
        
        assert result['odds_only'] == [1, 3, 5, 7, 9]
        assert result['first_multiple_of_7_after_10'] == 14
        assert result['found_6'] is False
        assert result['with_pass'] == [0, 1, 3, 4]


class TestComprehensions:
    """Test comprehension demonstrations."""
    
    def test_list_comprehension(self):
        """Test list comprehensions."""
        result = control_flow.demonstrate_list_comprehension()
        
        assert result['squares'][:5] == [0, 1, 4, 9, 16]
        assert all(x % 2 == 0 for x in result['evens'])
        assert all(x % 15 == 0 for x in result['divisible_by_15'])
        assert len(result['matrix']) == 3
        assert result['absolute_values'] == [3, 1, 0, 1, 3]
        assert result['uppercase'] == ['HELLO', 'WORLD', 'PYTHON']
    
    def test_dict_set_comprehension(self):
        """Test dictionary and set comprehensions."""
        result = control_flow.demonstrate_dict_set_comprehension()
        
        assert result['squares_dict'] == {0: 0, 1: 1, 2: 4, 3: 9, 4: 16}
        assert all(k % 2 == 0 for k in result['even_squares'].keys())
        assert 'apple' in result['discounted_prices']
        assert result['discounted_prices']['apple'] == pytest.approx(0.4)
        assert isinstance(result['unique_lengths'], set)
        assert result['unique_vowels'] == {'e', 'o'}


class TestExceptionHandling:
    """Test exception handling demonstrations."""
    
    def test_exception_handling(self):
        """Test basic exception handling."""
        result = control_flow.demonstrate_exception_handling(10, 2)
        assert result['division'] == 5.0
        
        result = control_flow.demonstrate_exception_handling(10, 0)
        assert result['division'] == "Cannot divide by zero"
        
        assert 'conversions' in result
        assert result['file_processed'] is True
        assert result['cleanup_done'] is True
    
    def test_raising_exceptions(self):
        """Test raising exceptions."""
        result = control_flow.demonstrate_raising_exceptions()
        
        assert result['25'] == 'Valid'
        assert result['50.5'] == 'Valid'
        assert 'ValueError' in result['-5']
        assert 'CustomError' in result['200']
        assert 'TypeError' in result['twenty']
    
    def test_context_managers(self):
        """Test context managers."""
        result = control_flow.demonstrate_context_managers()
        
        assert result['file_opened'] == 'test.txt'
        assert result['file_closed'] is True
        assert result['data_written'] == 'Hello, World!'
        assert result['multiple_files'] is True


class TestMatchStatement:
    """Test match statement simulation."""
    
    def test_match_statement(self):
        """Test match statement functionality."""
        result = control_flow.demonstrate_match_statement()
        
        assert result['quit'] == 'Exiting program'
        assert result['help'] == 'Available commands: quit, help, status'
        assert 'Hello World' in result['echo Hello World']
        assert result['calc 10 + 5'] == 'Result: 15.0'
        assert result['calc 20 / 4'] == 'Result: 5.0'
        assert result['unknown'] == 'Unknown command'


if __name__ == "__main__":
    pytest.main([__file__, "-v"])