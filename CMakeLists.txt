cmake_minimum_required(VERSION 3.21)
project(Aufgabe9 C)

set(CMAKE_C_STANDARD 99)

add_executable(Aufgabe9 main.c huffman_common.h io.c io.h huffman.c huffman.h frequency.c frequency.h binary_heap.c binary_heap.h )
