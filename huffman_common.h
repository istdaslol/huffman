#ifndef HUFFMAN_COMMON_H
#define HUFFMAN_COMMON_H

#include <stdbool.h>

#define BUFF_SIZE 4096
#define ASCII_MAX 256

typedef enum
{
	ZERO = '0',
	ONE = '1'
} BIT;

typedef enum
{
	NO_ERROR = 0,
	UNKNOWN_ERROR = 1,
	PARAMETER_ERROR = 2,
	FILE_OR_IO_ERROR = 3,
	HUFFMAN_ERROR = 4,
} ERROR_CODES;

unsigned char selected_mode;

bool verbose;
bool help;
bool has_outfile;

char* outfile_name;
char* infile_name;

#endif //HUFFMAN_COMMON_H