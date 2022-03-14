#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <stdbool.h>

/**
 * Struct for function pointer of a compare function
 */
typedef int (*HEAP_ELEM_COMP) (void *, void *);

/**
 * Struct for function pointer of a print function
 */
typedef void (*HEAP_ELEM_PRINT) (void *);

/**
 * Initializes the start
 * @param comp pointer to the compare function, that is going to be used by the heap
 * @param print pointer to the print function, that is going to be used by the heap
 */
extern void heap_init(HEAP_ELEM_COMP comp,HEAP_ELEM_PRINT print);

/**
 * Deletes the heap and frees the Memory
 */
extern void heap_destroy(void);

/**
 * Inserts a new element into the heap and maintaining its correctness
 * @param element the element that is going to be inserted
 */
extern void heap_insert(void *element);

/**
 * Extracts the smallest element within the heap and stores it in the parameter "min_element"
 * while maintaining the correctness of the heap
 * deletes the extracted element and destroys the heap when the last was extracted
 * @param min_element the pointer that is going to point to the smallest element of the heap
 * @return true, if the extraction was successfully; false, otherwise
 */
extern bool heap_extract_min(void **min_element);

/**
 * Prints the current heap
 */
extern void heap_print(void);

#endif //BINARY_HEAP_H
