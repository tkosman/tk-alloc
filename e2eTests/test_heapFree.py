import ctypes
import pytest
from unittest.mock import patch
import subprocess

allocator = ctypes.CDLL('./bin/heapAllocator.so')
allocator.heapAlloc.argtypes = [ctypes.c_size_t]
allocator.heapAlloc.restype = ctypes.c_void_p
allocator.heapFree.argtypes = [ctypes.c_void_p]
allocator.heapFree.restype = None

def test_free_null_block(capfd):
    allocator.heapFree(None)
    captured = capfd.readouterr()
    assert captured.out == "Freeing NULL pointer\n", "Freeing NULL block failed"

#ONlL FOR MACOS EXIT CODES
def test_free_invalid_block():
    result = subprocess.run(['./bin/seq_fault.out'], text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    assert result.returncode == -11, "Segmentation fault occurred"
