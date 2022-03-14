#ifndef IO_H
#define IO_H

#include <stdbool.h>


#include "huffman_common.h"

/**
 * initializes the input buffer with the input character array
 * resets the position and usage variable
 */
extern void init_in(void);

/**
 * initializes the output buffer by
 * resetting the position and usage variable
 */
extern void init_out(void);

/**
 * tests for read_char if it still has a valid byte in the in_buffer and may also read the file again
 * in order to check for readable characters
 * @return true when there is still a valid value to be read
 */
extern bool has_next_char(void);

/**
 * returns a character from the in_buffer and increases the in_buffer_position by 1
 * @return the character at the current position of the in_buffer_position
 */
extern unsigned char read_char(void);

/**
 * writes the input into the next position of the out_buffer and increases the out_buffer_position by 1
 * if the buffer is full afterwards write the buffer to the file and resets it
 * @param c character that's going to be written in the out_buffer
 */
extern void write_char(unsigned char c);

/**
 * tests for read_bit if it still has a valid bit in the in_buffer and may also read the file again
 * in order ro check for readable bits
 * @return true when there is still a valid value in the in_buffer to be read
 */
extern bool has_next_bit(void);

/**
 * returns a bit from the in_buffer and increases the in_buffer_position by 1
 * @return the bit at the current position of the in_buffer_position
 */
extern BIT read_bit(void);

/**
 * writes the input into the next position of the out_buffer and increases the out_buffer_position by 1
 * if the buffer is full afterwards write the buffer to the file and resets it
 * @param c BIT that's going to be written to the out_buffer
 */
extern void write_bit(BIT c);

/**
 *  starts a stream for the infile
 */
extern void open_infile(void);

/**
 *  starts a stream for the outfile
 */
extern void open_outfile(void);

/**
 * ends a stream for the infile
 */
extern void close_infile(void);

/**
 * ends a stream for the ourfile
 */
extern void close_outfile(void);

/**
 * reads the infile into the in_buffer and stores the current usage into in_buffer_usage
 */
extern void read_file(void);

/**
 * writes the out_buffer into the out_file and resets the out_buffer
 */
extern void write_file(void);

/**
 * sets the file-stream back to the start of the file
 */
extern void gotostart_infile(void);

/**
 * writes a given int to the out_buffer
 * @param i
 */
extern void write_int(unsigned int i);

/**
 * reads an int value from the in_buffer;
 * @return
 */
extern unsigned int* read_int(void);

#endif //IO_H

