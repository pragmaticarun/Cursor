"""
File Operations in Python

This module demonstrates file I/O operations including text files, binary files,
JSON, CSV, and more.
"""

import os
import json
import csv
import pickle
import tempfile
import shutil
from pathlib import Path
from typing import List, Dict, Any
import io


# ============= TEXT FILE OPERATIONS =============

def demonstrate_text_file_operations():
    """Demonstrate basic text file operations."""
    results = {}
    
    # Create a temporary file for demonstration
    temp_file = tempfile.NamedTemporaryFile(mode='w+', delete=False, suffix='.txt')
    temp_file_path = temp_file.name
    
    try:
        # Writing to file
        with open(temp_file_path, 'w') as f:
            f.write("Hello, World!\n")
            f.write("Python is awesome!\n")
            f.write("File operations are important.\n")
        
        results['file_created'] = True
        
        # Reading entire file
        with open(temp_file_path, 'r') as f:
            content = f.read()
        results['full_content'] = content
        
        # Reading line by line
        with open(temp_file_path, 'r') as f:
            lines = []
            for line in f:
                lines.append(line.strip())
        results['lines'] = lines
        
        # Reading all lines at once
        with open(temp_file_path, 'r') as f:
            all_lines = f.readlines()
        results['all_lines'] = [line.strip() for line in all_lines]
        
        # Appending to file
        with open(temp_file_path, 'a') as f:
            f.write("Appended line 1\n")
            f.write("Appended line 2\n")
        
        # Read after append
        with open(temp_file_path, 'r') as f:
            updated_content = f.read()
        results['after_append'] = updated_content
        
        # File position manipulation
        with open(temp_file_path, 'r') as f:
            f.seek(0)  # Go to beginning
            first_5_chars = f.read(5)
            current_position = f.tell()
            f.seek(0, 2)  # Go to end
            file_size = f.tell()
        
        results['position_ops'] = {
            'first_5_chars': first_5_chars,
            'position_after_5': current_position,
            'file_size': file_size
        }
        
    finally:
        # Clean up
        os.unlink(temp_file_path)
    
    return results


def demonstrate_file_modes():
    """Demonstrate different file opening modes."""
    modes = {
        'r': 'Read (default)',
        'w': 'Write (overwrites existing)',
        'a': 'Append',
        'x': 'Exclusive creation (fails if exists)',
        'r+': 'Read and write',
        'w+': 'Write and read (overwrites)',
        'a+': 'Append and read',
        'rb': 'Read binary',
        'wb': 'Write binary',
        'rt': 'Read text (default)',
        'wt': 'Write text'
    }
    
    results = {'modes': modes}
    
    # Demonstrate mode usage
    temp_file = tempfile.NamedTemporaryFile(delete=False)
    temp_file_path = temp_file.name
    temp_file.close()
    
    try:
        # Write mode
        with open(temp_file_path, 'w') as f:
            f.write("Initial content")
        
        # Read mode
        with open(temp_file_path, 'r') as f:
            results['after_write'] = f.read()
        
        # Append mode
        with open(temp_file_path, 'a') as f:
            f.write("\nAppended content")
        
        # Read+ mode
        with open(temp_file_path, 'r+') as f:
            content = f.read()
            f.write("\nAdded with r+")
            f.seek(0)
            final = f.read()
        
        results['final_content'] = final
        
    finally:
        os.unlink(temp_file_path)
    
    return results


# ============= BINARY FILE OPERATIONS =============

def demonstrate_binary_operations():
    """Demonstrate binary file operations."""
    results = {}
    
    temp_file = tempfile.NamedTemporaryFile(delete=False, suffix='.bin')
    temp_file_path = temp_file.name
    temp_file.close()
    
    try:
        # Writing binary data
        data = bytes([0x48, 0x65, 0x6C, 0x6C, 0x6F])  # "Hello" in bytes
        with open(temp_file_path, 'wb') as f:
            f.write(data)
            f.write(b'\n')
            f.write("World".encode('utf-8'))
        
        # Reading binary data
        with open(temp_file_path, 'rb') as f:
            binary_content = f.read()
        
        results['binary_content'] = binary_content
        results['decoded'] = binary_content.decode('utf-8')
        
        # Using pickle for object serialization
        data_to_pickle = {
            'list': [1, 2, 3],
            'dict': {'a': 1, 'b': 2},
            'tuple': (4, 5, 6)
        }
        
        pickle_file = temp_file_path + '.pickle'
        
        # Pickle dump
        with open(pickle_file, 'wb') as f:
            pickle.dump(data_to_pickle, f)
        
        # Pickle load
        with open(pickle_file, 'rb') as f:
            loaded_data = pickle.load(f)
        
        results['pickled_data'] = loaded_data
        
        os.unlink(pickle_file)
        
    finally:
        os.unlink(temp_file_path)
    
    return results


# ============= JSON OPERATIONS =============

def demonstrate_json_operations():
    """Demonstrate JSON file operations."""
    results = {}
    
    # Sample data
    data = {
        'name': 'Alice',
        'age': 30,
        'city': 'New York',
        'hobbies': ['reading', 'coding', 'hiking'],
        'address': {
            'street': '123 Main St',
            'zip': '10001'
        },
        'active': True,
        'balance': 1234.56,
        'nothing': None
    }
    
    temp_file = tempfile.NamedTemporaryFile(mode='w+', delete=False, suffix='.json')
    temp_file_path = temp_file.name
    temp_file.close()
    
    try:
        # Write JSON to file
        with open(temp_file_path, 'w') as f:
            json.dump(data, f, indent=2)
        
        # Read JSON from file
        with open(temp_file_path, 'r') as f:
            loaded_data = json.load(f)
        
        results['loaded_data'] = loaded_data
        
        # JSON string operations
        json_string = json.dumps(data, indent=2)
        results['json_string'] = json_string[:100] + '...'  # First 100 chars
        
        # Parse JSON string
        parsed = json.loads(json_string)
        results['parsed_from_string'] = parsed['name']
        
        # Custom JSON encoder
        class CustomEncoder(json.JSONEncoder):
            def default(self, obj):
                if isinstance(obj, set):
                    return list(obj)
                if isinstance(obj, bytes):
                    return obj.decode('utf-8')
                return super().default(obj)
        
        custom_data = {'items': {1, 2, 3}, 'data': b'bytes'}
        custom_json = json.dumps(custom_data, cls=CustomEncoder)
        results['custom_encoded'] = custom_json
        
    finally:
        os.unlink(temp_file_path)
    
    return results


# ============= CSV OPERATIONS =============

def demonstrate_csv_operations():
    """Demonstrate CSV file operations."""
    results = {}
    
    temp_file = tempfile.NamedTemporaryFile(mode='w+', delete=False, suffix='.csv', newline='')
    temp_file_path = temp_file.name
    temp_file.close()
    
    try:
        # Writing CSV with writer
        data = [
            ['Name', 'Age', 'City'],
            ['Alice', 30, 'New York'],
            ['Bob', 25, 'London'],
            ['Charlie', 35, 'Paris']
        ]
        
        with open(temp_file_path, 'w', newline='') as f:
            writer = csv.writer(f)
            writer.writerows(data)
        
        # Reading CSV with reader
        with open(temp_file_path, 'r') as f:
            reader = csv.reader(f)
            rows = list(reader)
        results['basic_csv'] = rows
        
        # Using DictWriter
        dict_data = [
            {'Name': 'David', 'Age': 28, 'City': 'Tokyo'},
            {'Name': 'Eve', 'Age': 32, 'City': 'Sydney'},
            {'Name': 'Frank', 'Age': 29, 'City': 'Berlin'}
        ]
        
        dict_file = temp_file_path + '_dict.csv'
        
        with open(dict_file, 'w', newline='') as f:
            fieldnames = ['Name', 'Age', 'City']
            writer = csv.DictWriter(f, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerows(dict_data)
        
        # Using DictReader
        with open(dict_file, 'r') as f:
            reader = csv.DictReader(f)
            dict_rows = list(reader)
        results['dict_csv'] = dict_rows
        
        # CSV with different delimiter
        tsv_file = temp_file_path + '.tsv'
        with open(tsv_file, 'w', newline='') as f:
            writer = csv.writer(f, delimiter='\t')
            writer.writerows(data)
        
        with open(tsv_file, 'r') as f:
            reader = csv.reader(f, delimiter='\t')
            tsv_rows = list(reader)
        results['tsv_data'] = tsv_rows[:2]  # First 2 rows
        
        # CSV dialect
        csv.register_dialect('pipes', delimiter='|', quoting=csv.QUOTE_MINIMAL)
        
        pipe_file = temp_file_path + '.pipe'
        with open(pipe_file, 'w', newline='') as f:
            writer = csv.writer(f, dialect='pipes')
            writer.writerows(data)
        
        results['dialect_registered'] = True
        
        # Clean up additional files
        os.unlink(dict_file)
        os.unlink(tsv_file)
        os.unlink(pipe_file)
        
    finally:
        os.unlink(temp_file_path)
    
    return results


# ============= PATH OPERATIONS =============

def demonstrate_path_operations():
    """Demonstrate pathlib operations."""
    results = {}
    
    # Current directory
    current_path = Path.cwd()
    results['current_dir'] = str(current_path)
    
    # Path construction
    path = Path('folder') / 'subfolder' / 'file.txt'
    results['constructed_path'] = str(path)
    
    # Path properties
    test_path = Path(__file__)  # This file
    results['path_properties'] = {
        'name': test_path.name,
        'stem': test_path.stem,
        'suffix': test_path.suffix,
        'parent': str(test_path.parent),
        'absolute': str(test_path.absolute()),
        'exists': test_path.exists(),
        'is_file': test_path.is_file(),
        'is_dir': test_path.is_dir()
    }
    
    # Create temporary directory for testing
    with tempfile.TemporaryDirectory() as temp_dir:
        temp_path = Path(temp_dir)
        
        # Create directory structure
        (temp_path / 'subdir1').mkdir()
        (temp_path / 'subdir2').mkdir()
        (temp_path / 'subdir1' / 'nested').mkdir(parents=True)
        
        # Create some files
        (temp_path / 'file1.txt').write_text('Content 1')
        (temp_path / 'file2.py').write_text('print("Hello")')
        (temp_path / 'subdir1' / 'file3.txt').write_text('Content 3')
        
        # List directory contents
        contents = list(temp_path.iterdir())
        results['dir_contents'] = [p.name for p in contents]
        
        # Glob patterns
        txt_files = list(temp_path.glob('**/*.txt'))
        results['txt_files'] = [p.name for p in txt_files]
        
        # Read file
        content = (temp_path / 'file1.txt').read_text()
        results['read_content'] = content
        
        # File stats
        stats = (temp_path / 'file1.txt').stat()
        results['file_stats'] = {
            'size': stats.st_size,
            'modified': stats.st_mtime
        }
    
    return results


# ============= FILE UTILITIES =============

def demonstrate_file_utilities():
    """Demonstrate file utility operations."""
    results = {}
    
    with tempfile.TemporaryDirectory() as temp_dir:
        temp_path = Path(temp_dir)
        
        # Create test files and directories
        source_file = temp_path / 'source.txt'
        source_file.write_text('Source content')
        
        source_dir = temp_path / 'source_dir'
        source_dir.mkdir()
        (source_dir / 'file1.txt').write_text('File 1')
        (source_dir / 'file2.txt').write_text('File 2')
        
        # Copy file
        dest_file = temp_path / 'dest.txt'
        shutil.copy2(source_file, dest_file)
        results['file_copied'] = dest_file.exists()
        
        # Copy directory
        dest_dir = temp_path / 'dest_dir'
        shutil.copytree(source_dir, dest_dir)
        results['dir_copied'] = dest_dir.exists()
        
        # Move file
        moved_file = temp_path / 'moved.txt'
        shutil.move(str(dest_file), str(moved_file))
        results['file_moved'] = moved_file.exists()
        
        # Get file size
        file_size = os.path.getsize(source_file)
        results['file_size'] = file_size
        
        # Check file existence
        results['exists_check'] = {
            'source_exists': os.path.exists(source_file),
            'is_file': os.path.isfile(source_file),
            'is_dir': os.path.isdir(source_dir)
        }
        
        # Directory operations
        new_dir = temp_path / 'new_directory'
        os.makedirs(new_dir / 'nested' / 'deep', exist_ok=True)
        results['nested_dirs_created'] = (new_dir / 'nested' / 'deep').exists()
        
        # List directory
        dir_contents = os.listdir(temp_path)
        results['directory_listing'] = dir_contents[:5]  # First 5 items
        
        # Walk directory tree
        walk_results = []
        for root, dirs, files in os.walk(temp_path):
            walk_results.append({
                'root': Path(root).name,
                'dirs': dirs[:2],  # First 2 dirs
                'files': files[:2]  # First 2 files
            })
        results['walk_tree'] = walk_results[:3]  # First 3 levels
    
    return results


# ============= CONTEXT MANAGERS =============

def demonstrate_context_managers():
    """Demonstrate context managers for file operations."""
    results = {}
    
    # Basic with statement
    temp_file = tempfile.NamedTemporaryFile(mode='w+', delete=False)
    temp_file_path = temp_file.name
    temp_file.close()
    
    try:
        # Automatic file closing with 'with'
        with open(temp_file_path, 'w') as f:
            f.write("Content managed by context manager")
            results['file_closed_in_with'] = f.closed  # False
        results['file_closed_after_with'] = f.closed  # True
        
        # Multiple files in one with statement
        file1 = tempfile.NamedTemporaryFile(mode='w+', delete=False)
        file2 = tempfile.NamedTemporaryFile(mode='w+', delete=False)
        file1_path, file2_path = file1.name, file2.name
        file1.close()
        file2.close()
        
        with open(file1_path, 'w') as f1, open(file2_path, 'w') as f2:
            f1.write("File 1 content")
            f2.write("File 2 content")
        
        results['multiple_files_written'] = True
        
        # Custom context manager
        class ManagedFile:
            def __init__(self, filename, mode):
                self.filename = filename
                self.mode = mode
                self.file = None
            
            def __enter__(self):
                self.file = open(self.filename, self.mode)
                return self.file
            
            def __exit__(self, exc_type, exc_val, exc_tb):
                if self.file:
                    self.file.close()
                if exc_type is not None:
                    # Handle exception if needed
                    pass
                return False  # Don't suppress exceptions
        
        with ManagedFile(temp_file_path, 'r') as f:
            content = f.read()
        results['custom_context_manager'] = content[:20]  # First 20 chars
        
        # Clean up
        os.unlink(file1_path)
        os.unlink(file2_path)
        
    finally:
        os.unlink(temp_file_path)
    
    return results


# ============= IN-MEMORY FILES =============

def demonstrate_in_memory_files():
    """Demonstrate in-memory file operations with StringIO and BytesIO."""
    results = {}
    
    # StringIO for text
    text_buffer = io.StringIO()
    text_buffer.write("Hello, ")
    text_buffer.write("World!")
    text_buffer.write("\nPython StringIO")
    
    # Get content
    text_buffer.seek(0)
    content = text_buffer.read()
    results['stringio_content'] = content
    
    # Read line by line
    text_buffer.seek(0)
    lines = text_buffer.readlines()
    results['stringio_lines'] = lines
    
    # BytesIO for binary data
    binary_buffer = io.BytesIO()
    binary_buffer.write(b"Binary ")
    binary_buffer.write(b"Data")
    binary_buffer.write(bytes([0x00, 0xFF, 0x42]))
    
    # Get binary content
    binary_buffer.seek(0)
    binary_content = binary_buffer.read()
    results['bytesio_content'] = binary_content
    
    # Using StringIO as a file
    csv_buffer = io.StringIO()
    writer = csv.writer(csv_buffer)
    writer.writerow(['Name', 'Age'])
    writer.writerow(['Alice', 30])
    writer.writerow(['Bob', 25])
    
    csv_buffer.seek(0)
    csv_content = csv_buffer.read()
    results['csv_in_memory'] = csv_content
    
    # Clean up
    text_buffer.close()
    binary_buffer.close()
    csv_buffer.close()
    
    return results


# ============= FILE ENCODING =============

def demonstrate_file_encoding():
    """Demonstrate file encoding operations."""
    results = {}
    
    temp_file = tempfile.NamedTemporaryFile(mode='w+', delete=False, encoding='utf-8')
    temp_file_path = temp_file.name
    temp_file.close()
    
    try:
        # Write with UTF-8 encoding
        text_with_unicode = "Hello, 世界! 🐍 Python"
        with open(temp_file_path, 'w', encoding='utf-8') as f:
            f.write(text_with_unicode)
        
        # Read with UTF-8
        with open(temp_file_path, 'r', encoding='utf-8') as f:
            utf8_content = f.read()
        results['utf8_content'] = utf8_content
        
        # Write with different encoding
        latin1_file = temp_file_path + '.latin1'
        with open(latin1_file, 'w', encoding='latin-1', errors='replace') as f:
            f.write("Café")  # é is supported in Latin-1
        
        # Read with correct encoding
        with open(latin1_file, 'r', encoding='latin-1') as f:
            latin1_content = f.read()
        results['latin1_content'] = latin1_content
        
        # Detect encoding (simplified example)
        with open(temp_file_path, 'rb') as f:
            raw_bytes = f.read()
        
        # Try different encodings
        encodings = ['utf-8', 'latin-1', 'ascii']
        for enc in encodings:
            try:
                decoded = raw_bytes.decode(enc)
                results[f'decoded_as_{enc}'] = True
                break
            except UnicodeDecodeError:
                results[f'decoded_as_{enc}'] = False
        
        # Clean up
        os.unlink(latin1_file)
        
    finally:
        os.unlink(temp_file_path)
    
    return results


if __name__ == "__main__":
    # Demonstrate all features when run directly
    print("=== Text File Operations ===")
    print(demonstrate_text_file_operations())
    
    print("\n=== File Modes ===")
    print(demonstrate_file_modes())
    
    print("\n=== Binary Operations ===")
    print(demonstrate_binary_operations())
    
    print("\n=== JSON Operations ===")
    print(demonstrate_json_operations())
    
    print("\n=== CSV Operations ===")
    print(demonstrate_csv_operations())
    
    print("\n=== Path Operations ===")
    print(demonstrate_path_operations())
    
    print("\n=== File Utilities ===")
    print(demonstrate_file_utilities())
    
    print("\n=== Context Managers ===")
    print(demonstrate_context_managers())
    
    print("\n=== In-Memory Files ===")
    print(demonstrate_in_memory_files())
    
    print("\n=== File Encoding ===")
    print(demonstrate_file_encoding())