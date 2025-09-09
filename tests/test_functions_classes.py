"""
Tests for the functions_classes module.
"""

import pytest
import sys
import os
import time
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))

from src import functions_classes


class TestFunctions:
    """Test function demonstrations."""
    
    def test_basic_functions(self):
        """Test basic function features."""
        assert functions_classes.basic_function("World") == "Hello, World!"
        assert functions_classes.function_with_defaults(3) == 9
        assert functions_classes.function_with_defaults(2, 4) == 16
        assert functions_classes.function_with_args(1, 2, 3, 4) == 10
        
        kwargs_result = functions_classes.function_with_kwargs(name="Alice", age=30)
        assert kwargs_result['name'] == "Alice"
        assert kwargs_result['age'] == 30
        
        all_params = functions_classes.function_with_all("test", "custom", 1, 2, x=10)
        assert all_params['required'] == "test"
        assert all_params['default'] == "custom"
        assert all_params['args'] == (1, 2)
        assert all_params['kwargs'] == {'x': 10}
    
    def test_lambda_functions(self):
        """Test lambda functions."""
        assert functions_classes.square(5) == 25
        assert functions_classes.add(3, 7) == 10
        assert functions_classes.is_even(4) is True
        assert functions_classes.is_even(5) is False
    
    def test_demonstrate_functions(self):
        """Test function demonstration."""
        result = functions_classes.demonstrate_functions()
        
        assert result['basic'] == "Hello, Python!"
        assert result['default_used'] == 9
        assert result['default_overridden'] == 16
        assert result['var_args'] == 15
        assert 'name' in result['kwargs']
        assert result['lambda_square'] == 25
        assert result['lambda_add'] == 10
        assert result['lambda_filter'] == [0, 2, 4, 6, 8]


class TestDecorators:
    """Test decorator demonstrations."""
    
    def test_timer_decorator(self):
        """Test timer decorator."""
        result = functions_classes.slow_function(0.01)
        assert result == "Completed"
        assert hasattr(functions_classes.slow_function, 'execution_time')
        assert functions_classes.slow_function.execution_time >= 0.01
    
    def test_memoize_decorator(self):
        """Test memoization decorator."""
        # Clear cache
        functions_classes.fibonacci.cache.clear()
        
        # First call
        result1 = functions_classes.fibonacci(10)
        assert result1 == 55
        
        # Second call (should be cached)
        result2 = functions_classes.fibonacci(10)
        assert result2 == 55
        assert functions_classes.fibonacci.cache_hits > 0
    
    def test_validate_positive_decorator(self):
        """Test validation decorator."""
        assert functions_classes.calculate_area(5, 3) == 15
        
        with pytest.raises(ValueError):
            functions_classes.calculate_area(-5, 3)
    
    def test_repeat_decorator(self):
        """Test repeat decorator."""
        results = functions_classes.get_random_number()
        assert len(results) == 3
        assert all(isinstance(n, int) for n in results)
        assert all(1 <= n <= 100 for n in results)


class TestClosures:
    """Test closure demonstrations."""
    
    def test_multiplier_closure(self):
        """Test multiplier closure."""
        double = functions_classes.create_multiplier(2)
        triple = functions_classes.create_multiplier(3)
        
        assert double(5) == 10
        assert triple(5) == 15
        assert double(10) == 20
    
    def test_counter_closure(self):
        """Test counter closure."""
        counter = functions_classes.create_counter(0)
        
        assert counter() == 1
        assert counter() == 2
        assert counter() == 3
        assert counter.get_count() == 3


class TestGenerators:
    """Test generator demonstrations."""
    
    def test_fibonacci_generator(self):
        """Test fibonacci generator."""
        fib_gen = functions_classes.fibonacci_generator(20)
        fib_list = list(fib_gen)
        
        assert fib_list == [0, 1, 1, 2, 3, 5, 8, 13]
        assert all(n < 20 for n in fib_list)
    
    def test_infinite_counter(self):
        """Test infinite counter generator."""
        counter = functions_classes.infinite_counter(10, 5)
        
        assert next(counter) == 10
        assert next(counter) == 15
        assert next(counter) == 20


class TestClasses:
    """Test class demonstrations."""
    
    def test_person_class(self):
        """Test Person class."""
        person = functions_classes.Person("Alice", 30)
        
        assert str(person) == "Alice (30 years old)"
        assert repr(person) == "Person(name='Alice', age=30)"
        assert person.introduce() == "Hi, I'm Alice, 30 years old"
        assert person.age_group == "Adult"
        assert functions_classes.Person.is_adult(30) is True
        assert functions_classes.Person.is_adult(16) is False
        
        # Test equality
        person2 = functions_classes.Person("Alice", 30)
        person3 = functions_classes.Person("Bob", 25)
        assert person == person2
        assert person != person3
        
        # Test class method
        person4 = functions_classes.Person.from_birth_year("Bob", 1990)
        assert person4.age >= 33  # Depends on current year
        
        # Test info property
        info = person.info
        assert info['name'] == "Alice"
        assert info['age'] == 30
        assert info['is_adult'] is True
    
    def test_student_class(self):
        """Test Student class (inheritance)."""
        student = functions_classes.Student("Charlie", 20, "S12345", 85.5)
        
        assert student.study("Math") == "Charlie is studying Math"
        student.enroll("Physics")
        student.enroll("Chemistry")
        assert "Physics" in student.courses
        assert len(student.courses) == 2
        
        info = student.info
        assert info['student_id'] == "S12345"
        assert info['grade'] == 85.5
        assert info['age_group'] == "Adult"
    
    def test_bank_account_class(self):
        """Test BankAccount class (encapsulation)."""
        account = functions_classes.BankAccount("1234567890", 1000)
        
        assert account.balance == 1000
        assert account.account_number == "***7890"
        
        account.deposit(500)
        assert account.balance == 1500
        
        account.withdraw(200)
        assert account.balance == 1300
        
        with pytest.raises(ValueError):
            account.deposit(-100)
        
        with pytest.raises(ValueError):
            account.withdraw(2000)
        
        statement = account.get_statement()
        assert "***7890" in statement
        assert "$1300.00" in statement
    
    def test_shape_classes(self):
        """Test Shape hierarchy (polymorphism)."""
        rectangle = functions_classes.Rectangle(5, 3, "red")
        circle = functions_classes.Circle(4, "blue")
        
        assert rectangle.area() == 15
        assert rectangle.perimeter() == 16
        assert "red Rectangle" in rectangle.describe()
        
        assert circle.area() == pytest.approx(50.265, rel=0.01)
        assert circle.perimeter() == pytest.approx(25.132, rel=0.01)
        assert "blue Circle" in circle.describe()
    
    def test_data_class(self):
        """Test DataClass with special methods."""
        dc = functions_classes.DataClass([1, 2, 3, 4, 5])
        
        assert len(dc) == 5
        assert dc[0] == 1
        assert dc[-1] == 5
        assert 3 in dc
        assert 10 not in dc
        
        dc[0] = 10
        assert dc[0] == 10
        
        # Test iteration
        items = list(dc)
        assert items == [10, 2, 3, 4, 5]
        
        # Test addition
        dc2 = functions_classes.DataClass([6, 7])
        dc3 = dc + dc2
        assert len(dc3) == 7
    
    def test_singleton_class(self):
        """Test Singleton metaclass."""
        db1 = functions_classes.DatabaseConnection()
        db2 = functions_classes.DatabaseConnection()
        
        assert db1 is db2
        assert id(db1) == id(db2)
        
        db1.connect()
        assert db1.connected is True
        assert db2.connected is True  # Same instance
        
        db2.disconnect()
        assert db1.connected is False  # Same instance


class TestDemonstrateClasses:
    """Test class demonstration function."""
    
    def test_demonstrate_classes(self):
        """Test the demonstrate_classes function."""
        result = functions_classes.demonstrate_classes()
        
        # Test person results
        assert 'person' in result
        assert 'Alice' in result['person']['str']
        
        # Test student results
        assert 'student' in result
        assert 'Charlie' in result['student']['study']
        
        # Test bank account results
        assert 'bank_account' in result
        assert result['bank_account']['balance'] == 1300
        
        # Test shapes results
        assert 'shapes' in result
        assert len(result['shapes']) == 3
        
        # Test singleton results
        assert 'singleton' in result
        assert result['singleton']['same_instance'] is True


if __name__ == "__main__":
    pytest.main([__file__, "-v"])