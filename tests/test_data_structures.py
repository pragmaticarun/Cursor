"""
Tests for the data_structures module.
"""

import pytest
import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))

from src import data_structures
from collections import namedtuple


class TestListOperations:
    """Test list operations."""
    
    def test_list_operations(self):
        """Test basic list operations."""
        result = data_structures.demonstrate_list_operations()
        
        assert 0 in result['after_adding']
        assert 4 in result['after_adding']
        assert 6 in result['after_adding']
        
        assert 3 not in result['after_removing']
        assert result['popped_values'] == (6, 0)
        
        assert result['count_2'] == 3
        assert result['index_of_3'] == 2
        assert result['4_in_list'] is True
        
        assert result['sorted'] == [1, 1, 2, 3, 4, 5, 9]
        assert result['reversed'][0] == 9
        
        assert result['stack_operations']['popped'] == 3
        assert result['queue_operations']['dequeued'] == 1
    
    def test_list_slicing(self):
        """Test list slicing."""
        result = data_structures.demonstrate_list_slicing()
        
        assert result['first_three'] == [0, 1, 2]
        assert result['last_three'] == [7, 8, 9]
        assert result['middle'] == [3, 4, 5, 6]
        assert result['every_second'] == [0, 2, 4, 6, 8]
        assert result['reverse'] == [9, 8, 7, 6, 5, 4, 3, 2, 1, 0]
        assert result['except_first_last'] == [1, 2, 3, 4, 5, 6, 7, 8]
        
        slice_assign = result['slice_assignment']
        assert 'a' in slice_assign['after_replace']
        assert 'x' in slice_assign['after_insert']
    
    def test_list_comprehensions(self):
        """Test list comprehensions."""
        result = data_structures.demonstrate_list_comprehensions()
        
        assert result['squares'][:5] == [0, 1, 4, 9, 16]
        assert all(x % 2 == 0 for x in result['evens'])
        assert all(x % 15 == 0 for x in result['divisible_by_15'])
        assert len(result['pairs']) == 9
        assert result['matrix'][1][1] == 4
        assert result['absolute_values'] == [3, 1, 0, 1, 3]
        assert result['flattened'] == [1, 2, 3, 4, 5, 6]
        assert result['word_lengths'] == [5, 5, 6]
        assert 'PYTHON' in result['long_words_upper']


class TestDictOperations:
    """Test dictionary operations."""
    
    def test_dict_operations(self):
        """Test basic dictionary operations."""
        result = data_structures.demonstrate_dict_operations()
        
        assert result['get_a'] == 1
        assert result['get_with_default'] == 0
        assert 'd' in result['after_update']
        assert 'e' in result['after_update']
        
        assert result['removed_values'][0] == 6
        assert 'a' in result['keys']
        assert 2 in result['values']
        
        assert result['has_key_a'] is True
        assert result['has_value_2'] is True
        
        assert result['merged']['b'] == 3  # dict2 overwrites dict1
        
        comp = result['comprehensions']
        assert comp['squares'][3] == 9
        assert all(v > 2 for v in comp['filtered'].values())
        assert 1 in comp['inverted']
    
    def test_nested_dicts(self):
        """Test nested dictionary operations."""
        result = data_structures.demonstrate_nested_dicts()
        
        assert result['johns_age'] == 30
        assert result['it_budget'] == 500000
        assert result['safe_access'] == 0
        assert 'a_b' in result['flattened']
        assert result['flattened']['a_b'] == 1


class TestSetOperations:
    """Test set operations."""
    
    def test_set_operations(self):
        """Test basic set operations."""
        result = data_structures.demonstrate_set_operations()
        
        assert 4 in result['modified_set']
        assert 5 in result['modified_set']
        assert 7 not in result['modified_set']
        
        assert result['union'] == {1, 2, 3, 4, 5, 6, 7, 8}
        assert result['intersection'] == {4, 5}
        assert result['difference'] == {1, 2, 3}
        assert result['symmetric_difference'] == {1, 2, 3, 6, 7, 8}
        
        assert result['is_subset'] is True
        assert result['is_superset'] is True
        assert result['is_disjoint'] is True
        
        assert 0 in result['comprehensions']['squares']
        assert all(x % 2 == 0 for x in result['comprehensions']['evens'])
        
        assert result['frozen_set']['type'] == 'frozenset'
        assert result['frozen_set']['hashable'] is True


class TestTupleOperations:
    """Test tuple operations."""
    
    def test_tuple_operations(self):
        """Test basic tuple operations."""
        result = data_structures.demonstrate_tuple_operations()
        
        assert result['first'] == 1
        assert result['last'] == 5
        assert result['slice'] == (2, 3, 4)
        
        unpacking = result['unpacking']
        assert unpacking['x_y'] == (10, 20)
        assert unpacking['a_rest_b'] == (1, [2, 3, 4], 5)
        
        assert result['count_2'] == 3
        assert result['index_of_3'] == 2
        
        named = result['named_tuple']
        assert named['point1'] == (11, 22)
        assert named['point2']['x'] == 33
        assert 'x' in named['fields']
        
        assert result['tuple_as_key'] == 'origin'


class TestCollections:
    """Test collections module structures."""
    
    def test_collections(self):
        """Test collections module demonstrations."""
        result = data_structures.demonstrate_collections()
        
        # Counter
        counter = result['counter']
        assert counter['counts']['apple'] == 3
        assert counter['most_common'][0][0] == 'apple'
        assert counter['total'] == 6
        
        # defaultdict
        assert 'apple' in result['defaultdict']['fruits']
        assert result['defaultdict_int']['a'] == 1
        
        # deque
        deque_result = result['deque']
        assert deque_result['popped'] == (0, 4)
        
        # OrderedDict
        od_keys = list(result['ordered_dict'].keys())
        assert od_keys[-1] == 'first'  # Moved to end
        
        # ChainMap
        chain = result['chain_map']
        assert chain['b_value'] == 2  # From first dict


class TestAdvancedStructures:
    """Test advanced data structures."""
    
    def test_heap_operations(self):
        """Test heap operations."""
        result = data_structures.demonstrate_heap_operations()
        
        assert result['smallest_popped'] == 1
        assert result['heapified'][0] == 1  # Min element at root
        assert result['largest_3'] == [9, 8, 7]
        assert result['smallest_3'] == [0, 1, 2]
        assert result['priority_queue_pop'] == 'task2'
    
    def test_bisect_operations(self):
        """Test bisect operations."""
        result = data_structures.demonstrate_bisect_operations()
        
        assert result['insertion_point'] == 2
        assert 5 in result['after_insert']
        assert result['search_for_7'] >= 0
        assert result['search_for_6'] == -1
        assert result['grade_85'] == 'B'
        assert result['grade_72'] == 'C'
    
    def test_stack_queue(self):
        """Test stack and queue implementations."""
        result = data_structures.demonstrate_stack_queue()
        
        assert result['list_stack']['popped'] == 3
        assert len(result['list_stack']['remaining']) == 2
        
        assert result['deque_queue']['dequeued'] == 1
        assert len(result['deque_queue']['remaining']) == 2
        
        assert result['custom_stack']['peek'] == 20
        assert result['custom_stack']['size'] == 2
        
        assert result['custom_queue']['front'] == 'a'
        assert result['custom_queue']['size'] == 2


if __name__ == "__main__":
    pytest.main([__file__, "-v"])