"""
Tests for the file_operations module.
"""

import pytest
import sys
import os
import json
import tempfile
from pathlib import Path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))

from src import file_operations


class TestTextFileOperations:
    """Test text file operations."""
    
    def test_text_file_operations(self):
        """Test basic text file operations."""
        result = file_operations.demonstrate_text_file_operations()
        
        assert result['file_created'] is True
        assert 'Hello, World!' in result['full_content']
        assert 'Python is awesome!' in result['full_content']
        
        assert len(result['lines']) == 3
        assert result['lines'][0] == 'Hello, World!'
        
        assert 'Appended line 1' in result['after_append']
        
        position_ops = result['position_ops']
        assert position_ops['first_5_chars'] == 'Hello'
        assert position_ops['position_after_5'] == 5
        assert position_ops['file_size'] > 0
    
    def test_file_modes(self):
        """Test different file opening modes."""
        result = file_operations.demonstrate_file_modes()
        
        assert 'modes' in result
        assert result['modes']['r'] == 'Read (default)'
        assert result['modes']['w'] == 'Write (overwrites existing)'
        
        assert 'Initial content' in result['after_write']
        assert 'Added with r+' in result['final_content']


class TestBinaryOperations:
    """Test binary file operations."""
    
    def test_binary_operations(self):
        """Test binary file operations."""
        result = file_operations.demonstrate_binary_operations()
        
        assert result['binary_content'] == b'Hello\nWorld'
        assert result['decoded'] == 'Hello\nWorld'
        
        pickled = result['pickled_data']
        assert pickled['list'] == [1, 2, 3]
        assert pickled['dict'] == {'a': 1, 'b': 2}
        assert pickled['tuple'] == (4, 5, 6)


class TestJSONOperations:
    """Test JSON operations."""
    
    def test_json_operations(self):
        """Test JSON file operations."""
        result = file_operations.demonstrate_json_operations()
        
        loaded = result['loaded_data']
        assert loaded['name'] == 'Alice'
        assert loaded['age'] == 30
        assert loaded['hobbies'] == ['reading', 'coding', 'hiking']
        assert loaded['active'] is True
        assert loaded['nothing'] is None
        
        assert 'Alice' in result['json_string']
        assert result['parsed_from_string'] == 'Alice'
        
        custom = json.loads(result['custom_encoded'])
        assert custom['items'] == [1, 2, 3]  # Set converted to list
        assert custom['data'] == 'bytes'  # Bytes converted to string


class TestCSVOperations:
    """Test CSV operations."""
    
    def test_csv_operations(self):
        """Test CSV file operations."""
        result = file_operations.demonstrate_csv_operations()
        
        basic = result['basic_csv']
        assert basic[0] == ['Name', 'Age', 'City']
        assert basic[1] == ['Alice', '30', 'New York']
        
        dict_csv = result['dict_csv']
        assert dict_csv[0]['Name'] == 'David'
        assert dict_csv[0]['Age'] == '28'
        assert dict_csv[0]['City'] == 'Tokyo'
        
        tsv = result['tsv_data']
        assert len(tsv) == 2
        assert tsv[0] == ['Name', 'Age', 'City']
        
        assert result['dialect_registered'] is True


class TestPathOperations:
    """Test pathlib operations."""
    
    def test_path_operations(self):
        """Test Path operations."""
        result = file_operations.demonstrate_path_operations()
        
        assert 'current_dir' in result
        assert result['constructed_path'] == str(Path('folder/subfolder/file.txt'))
        
        props = result['path_properties']
        assert props['name'] == 'file_operations.py'
        assert props['stem'] == 'file_operations'
        assert props['suffix'] == '.py'
        assert props['exists'] is True
        assert props['is_file'] is True
        assert props['is_dir'] is False
        
        assert 'subdir1' in result['dir_contents']
        assert 'file1.txt' in result['dir_contents']
        
        assert 'file1.txt' in result['txt_files']
        assert 'file3.txt' in result['txt_files']
        
        assert result['read_content'] == 'Content 1'
        
        stats = result['file_stats']
        assert stats['size'] == 9  # "Content 1" is 9 bytes
        assert stats['modified'] > 0


class TestFileUtilities:
    """Test file utility operations."""
    
    def test_file_utilities(self):
        """Test file utilities."""
        result = file_operations.demonstrate_file_utilities()
        
        assert result['file_copied'] is True
        assert result['dir_copied'] is True
        assert result['file_moved'] is True
        assert result['file_size'] == 14  # "Source content" is 14 bytes
        
        exists = result['exists_check']
        assert exists['source_exists'] is True
        assert exists['is_file'] is True
        assert exists['is_dir'] is True
        
        assert result['nested_dirs_created'] is True
        assert 'source.txt' in result['directory_listing']
        
        walk = result['walk_tree']
        assert len(walk) > 0
        assert 'root' in walk[0]


class TestContextManagers:
    """Test context managers."""
    
    def test_context_managers(self):
        """Test context managers for file operations."""
        result = file_operations.demonstrate_context_managers()
        
        assert result['file_closed_in_with'] is False
        assert result['file_closed_after_with'] is True
        assert result['multiple_files_written'] is True
        assert 'Content managed' in result['custom_context_manager']


class TestInMemoryFiles:
    """Test in-memory file operations."""
    
    def test_in_memory_files(self):
        """Test StringIO and BytesIO."""
        result = file_operations.demonstrate_in_memory_files()
        
        assert result['stringio_content'] == "Hello, World!\nPython StringIO"
        assert len(result['stringio_lines']) == 2
        assert result['stringio_lines'][0] == "Hello, World!\n"
        
        assert result['bytesio_content'] == b'Binary Data\x00\xffB'
        
        assert 'Name,Age' in result['csv_in_memory']
        assert 'Alice,30' in result['csv_in_memory']


class TestFileEncoding:
    """Test file encoding operations."""
    
    def test_file_encoding(self):
        """Test file encoding."""
        result = file_operations.demonstrate_file_encoding()
        
        assert result['utf8_content'] == "Hello, 世界! 🐍 Python"
        assert result['latin1_content'] == "Café"
        assert result['decoded_as_utf-8'] is True


if __name__ == "__main__":
    pytest.main([__file__, "-v"])