#include <stdlib.h>
#include <stdio.h>
#include "binary_heap.h"

#define HEAP_INCREMENT 5

/**
 * Variable to store the compare function used by the heap
 */
HEAP_ELEM_COMP compare_function;

/**
 * Variable to store the print function used by the heap
 */
HEAP_ELEM_PRINT print_function;

/**
 * Pointer to the start of the array
 */
void** heap;

/**
 * counter of the current elements within the heap
 */
int heap_elements;

/**
 * counter of the current free slots of reserved storage
 */
int heap_free_elements;

/**
 * Variable to keep track of the level within the heap while printing the heap
 */
int level;

/**
 * swaps two elements within the heap
 * @param a index of the first element
 * @param b index of the second element
 */
void swap(int a, int b);

/**
 * restores the heap starting at a given root
 * @param start the root at wich to start restoring the heap
 */
void restore_heap(int start);

/**
 * prints the children of an element recursively
 * @param parent the parent at wich to start printing it's children
 */
void print_children(int parent);

extern void heap_init(HEAP_ELEM_COMP comp, HEAP_ELEM_PRINT print)
{
	compare_function = comp;
	print_function = print;

	heap = malloc(HEAP_INCREMENT * sizeof(void*));
	heap_elements = 0;
	heap_free_elements = HEAP_INCREMENT;
}

extern void heap_destroy(void)
{
	free(heap);
}

extern void heap_insert(void* element)
{
	// extends array if needed
	if (heap_free_elements == 0)
	{
		heap = realloc(heap, (heap_elements + HEAP_INCREMENT) * sizeof(void*));
		heap_free_elements = HEAP_INCREMENT;
	}
	// if heap couldn't be created or extended
	if (heap == NULL)
	{
		fprintf(stderr, "Zu wenig freier Arbeitsspeicher");
		exit(EXIT_FAILURE);
	}

	// add new element to end and set counters
	heap[heap_elements] = element;
	heap_free_elements--;
	heap_elements++;

	// if there's more than one element in the heap, correct the heap
	if (heap_elements > 1)
	{
		int child = heap_elements - 1;
		int parent = (child - 1) / 2;
		int result;
		while (parent >= 0)
		{
			result = compare_function(heap[child], heap[parent]);
			if (result == -1) // child is smaller than parent and has to be swapped
			{
				swap(child, parent);
			}
			else // terminate loop because the child is in the correct position
			{
				break;
			}
			// get next pair
			child = parent;
			parent = (child - 1) / 2;
		}
	}
}

void swap(int a, int b)
{
	void* tmp = heap[a];
	heap[a] = heap[b];
	heap[b] = tmp;
}

extern bool heap_extract_min(void** min_element)
{
	bool has_extracted = false;
	if (heap_elements > 0)
	{
		*min_element = heap[0];
		heap_elements--;
		heap_free_elements++;
		has_extracted = true;
		if (heap_elements > 0)
		{
			swap(0, heap_elements);
		}
		restore_heap(0);
		// if enough elements have been deleted resize the array to free storage
		if (heap_free_elements >= HEAP_INCREMENT)
		{
			heap = realloc(heap, (heap_elements) * sizeof(void*));
			heap_free_elements = 0;
		}
	}
	return has_extracted;
}

void restore_heap(int start)
{
	// set start as the current smallest
	int smallest = start;
	// calculate children
	int left_child = start * 2 + 1;
	int right_child = start * 2 + 2;

	// if a left child exists, and it's smaller than the current smallest set it as the smallest
	smallest = (left_child < heap_elements && (compare_function(heap[left_child], heap[smallest]) == -1))
			   ? left_child
			   : smallest;
	// if a right child exists, and it's smaller than the current smallest set it as the smallest
	smallest = (right_child < heap_elements && (compare_function(heap[right_child], heap[smallest]) == -1))
			   ? right_child
			   : smallest;

	// if smallest isn't the start swap and restore heap for the subtree
	if (smallest != start)
	{
		swap(smallest, start);
		restore_heap(smallest);
	}
}

extern void heap_print(void)
{
	if (heap_elements > 0)
	{
		printf("\nDer Heap sieht wie folgt aus:\n");
		// print the root
		print_function(heap[0]);
		printf("\n");

		// sets the level to -1 so that the first print_children will increase it to 0
		level = -1;
		// start recursion for the children
		print_children(0);

		printf("\n");
	}
}

void print_children(int parent)
{
	// calculate children
	int left_child = parent * 2 + 1;
	int right_child = parent * 2 + 2;

	if (left_child < heap_elements)
	{
		level++;
		// print spaces according to the level
		for (int i = level; i > 0; i--)
		{
			printf("     ");
		}
		printf("|--->");
		print_function(heap[left_child]);
		printf("\n");
		print_children(left_child);
	}
	if (right_child < heap_elements)
	{
		level++;
		// print spaces according to the level
		for (int i = level; i > 0; i--)
		{
			printf("     ");
		}
		printf("|--->");
		print_function(heap[right_child]);
		printf("\n");
		print_children(right_child);
	}
	level--;
}