import ctypes
import pytest

allocator = ctypes.CDLL('./bin/heapAllocator.so')

def test_allocate_small_block():
    size = 128
    block = allocator.heapAlloc(size)
    assert block is not None, "Allocation failed for small block"

def test_allocate_large_block():
    size = 1024 * 1024 * 1024
    block = allocator.heapAlloc(size)
    assert block is not None, "Allocation failed for large block"

def test_allocate_zero_block():
    size = 0
    block = allocator.heapAlloc(size)
    assert block is not None, "Allocation should return NULL for zero size"

def test_allocate_negative_block():
    size = -100
    block = allocator.heapAlloc(size)
    assert block is not None, "Allocation should return NULL for negative size"