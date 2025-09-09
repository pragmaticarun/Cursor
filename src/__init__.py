"""
Python Programming Tutorial Package

This package contains modules demonstrating Python's basic language features
and standard library facilities.
"""

__version__ = "1.0.0"
__author__ = "Python Tutorial"

# Import all modules for easy access
from . import basics
from . import control_flow
from . import functions_classes
from . import data_structures
from . import file_operations
from . import stdlib_examples

__all__ = [
    'basics',
    'control_flow',
    'functions_classes',
    'data_structures',
    'file_operations',
    'stdlib_examples'
]