#include <stdlib.h>
#include <stdio.h>

#include "huffman.h"
#include "io.h"
#include "frequency.h"
#include "binary_heap.h"

#define HUFF_CODE_SIZE (ASCII_MAX - 1 * sizeof(char))

unsigned long bits = 0;

/**
 * function to read the huff_tree and store the corresponding code for a found char into the huff_table
 * @param huff_tree tree, to be searched
 * @param huff_table table, were the codes will be stored
 */
void get_huffcode(FREQUENCY* huff_tree, unsigned char huff_table[ASCII_MAX][HUFF_CODE_SIZE]);

extern void compress(void)
{
	// start
	open_infile();

	// count chars
	unsigned int working_set[ASCII_MAX] = { 0 };
	while (has_next_char())
	{
		unsigned char c = read_char();
		working_set[c]++;
	}

	// sort found chars
	heap_init((HEAP_ELEM_COMP)cmp_frequency, (HEAP_ELEM_PRINT)print_frequency);

	unsigned int symbols = 0;
	unsigned int diff_chars = 0;
	int i;
	for (i = 0; i < ASCII_MAX; i++)
	{
		if (working_set[i] > 0)
		{
			symbols += working_set[i];
			diff_chars++;

			FREQUENCY* new_element = malloc(sizeof(struct frequency));
			new_element->ascii_code = i;
			new_element->count = working_set[i];
			new_element->left_child = NULL;
			new_element->right_child = NULL;

			heap_insert(new_element);
		}
	}

	if (symbols < 1)
	{
		// create empty outfile
		open_outfile();
		close_outfile();
		fprintf(stderr, "File empty no need to compress\n");
		exit(EXIT_SUCCESS);
	}

	// create huffman tree
	FREQUENCY* huff_tree;
	while (true)
	{
		FREQUENCY* min1;
		FREQUENCY* min2;
		heap_extract_min(&min1);
		if (min1->count == symbols)
		{
			huff_tree = min1;
			break;
		}
		heap_extract_min(&min2);
		FREQUENCY* new_node = malloc(sizeof(struct frequency));
		new_node->count = min1->count + min2->count;
		new_node->left_child = min1;
		new_node->right_child = min2;
		new_node->ascii_code = -1;

		heap_insert(new_node);
	}

	// get translation-table
	unsigned char huff_code[ASCII_MAX][HUFF_CODE_SIZE];

	get_huffcode(huff_tree, huff_code);

	// export different chars
	open_outfile();
	write_char(diff_chars - 1);

	// export results
	for (i = 0; i < ASCII_MAX; i++)
	{
		if (working_set[i] > 0)
		{
			write_char((unsigned char)i);
			write_int(working_set[i]);
		}
	}

	// export overhead bits
	unsigned char offset_bits = (8 - (bits % 8) == 8) ? 0 : 8 - (bits % 8);
	write_char(offset_bits);

	// write overhead bits
	int ofs_count;
	for (ofs_count = 0; ofs_count < offset_bits; ofs_count++)
	{
		write_bit(ONE);
	}

	// translate file
	gotostart_infile();

	while (has_next_char())
	{
		unsigned char* trans_code = &huff_code[read_char()][0];
		int position;
		for (position = 0; trans_code[position] != '\0'; position++)
		{
			write_bit(trans_code[position]);
		}
	}

	// finish
	close_outfile();
	close_infile();
}

extern void decompress(void)
{
	// start
	open_infile();
	unsigned int sum = 0;

	// import translation-tree
	// import char-counter
	int diff_chars = 0;
	if (has_next_char())
	{
		diff_chars = read_char() + 1;
	}

	// check if there are any chars
	if (diff_chars < 1)
	{
		close_infile();
		open_outfile();
		close_outfile();
		fprintf(stderr, "File empty no need to decompress\n");
		exit(EXIT_SUCCESS);
	}

	// import results
	heap_init((HEAP_ELEM_COMP)cmp_frequency, (HEAP_ELEM_PRINT)print_frequency);
	for (int i = 0; i < diff_chars; i++)
	{
		FREQUENCY* result = malloc(sizeof(struct frequency));
		if (has_next_char())
		{
			result->ascii_code = read_char();
		}
		else
		{
			fprintf(stderr, "File empty or locked\n");
			exit(FILE_OR_IO_ERROR);
		}
		result->count = *read_int();
		result->left_child = NULL;
		result->right_child = NULL;
		sum += result->count;

		heap_insert(result);
	}

	// create huffman tree
	FREQUENCY* huff_tree;
	while (true)
	{
		FREQUENCY* min1;
		FREQUENCY* min2;
		heap_extract_min(&min1);
		if (min1->count == sum)
		{
			huff_tree = min1;
			break;
		}
		heap_extract_min(&min2);
		FREQUENCY* new_node = malloc(sizeof(struct frequency));
		new_node->count = min1->count + min2->count;
		new_node->left_child = min1;
		new_node->right_child = min2;
		new_node->ascii_code = -1;

		heap_insert(new_node);
	}

	// import offset-bits
	unsigned char offset_bits = read_char();

	// move by offset bits
	int ofs_counter;
	for (ofs_counter = 0; ofs_counter < offset_bits && has_next_bit(); ofs_counter++)
	{
		read_bit();
	}

	// translate file
	open_outfile();

	// if there is only one char, write this for every bit found
	if (huff_tree->left_child == NULL && huff_tree->right_child == NULL)
	{
		while (has_next_bit() && sum > 0)
		{
			read_bit();
			write_char(huff_tree->ascii_code);
			sum--;
		}
	}
	else
	{
		while (has_next_bit())
		{
			FREQUENCY* curr_elem = huff_tree;
			while (curr_elem->ascii_code == -1)
			{
				BIT next;
				if (has_next_bit())
				{
					next = read_bit();
				}
				else
				{
					fprintf(stderr, "File empty or locked\n");
					exit(FILE_OR_IO_ERROR);
				}

				if (next == ZERO)
				{
					curr_elem = curr_elem->left_child;
				}
				else
				{
					curr_elem = curr_elem->right_child;
				}
			}
			write_char(curr_elem->ascii_code);
		}
	}
	close_outfile();

	// finish
	close_infile();
}

// help function to do the recursion
void get_huffcode_help(FREQUENCY* huff_tree, unsigned char huff_table[ASCII_MAX][HUFF_CODE_SIZE], char* curr)
{
	if (huff_tree->ascii_code == -1)
	{
		if (huff_tree->left_child != NULL)
		{
			char* left_subtree = malloc(HUFF_CODE_SIZE);
			int i;
			for (i = 0; curr[i] != '\0'; i++)
			{
				left_subtree[i] = curr[i];
			}
			left_subtree[i] = '0';
			left_subtree[i + 1] = '\0';
			get_huffcode_help(huff_tree->left_child, huff_table, left_subtree);

		}
		if (huff_tree->right_child != NULL)
		{
			char* right_subtree = malloc(HUFF_CODE_SIZE);
			int i;
			for (i = 0; curr[i] != '\0'; i++)
			{
				right_subtree[i] = curr[i];
			}
			right_subtree[i] = '1';
			right_subtree[i + 1] = '\0';
			get_huffcode_help(huff_tree->right_child, huff_table, right_subtree);
		}
	}

	else
	{
		int i;
		for (i = 0; curr[i] != '\0'; i++)
		{
			huff_table[huff_tree->ascii_code][i] = curr[i];
		}
		huff_table[huff_tree->ascii_code][i] = '\0';
		bits += i * huff_tree->count;
	}

}

void get_huffcode(FREQUENCY* huff_tree, unsigned char huff_table[ASCII_MAX][HUFF_CODE_SIZE])
{
	// only one char in the tree, code it with 1
	if (huff_tree->left_child == NULL && huff_tree->right_child == NULL)
	{
		huff_table[huff_tree->ascii_code][0] = '1';
		huff_table[huff_tree->ascii_code][1] = '\0';
		bits = huff_tree->count;
	}
	else
	{
		get_huffcode_help(huff_tree, huff_table, "");
	}
}
