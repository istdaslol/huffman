cmake_minimum_required(VERSION 3.21)
project(Huffman C)

set(CMAKE_C_STANDARD 99)

add_executable(Huffman main.c huffman_common.h io.c io.h huffman.c huffman.h frequency.c frequency.h binary_heap.c binary_heap.h )
