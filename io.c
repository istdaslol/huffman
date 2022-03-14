#include <stdio.h>
#include <stdlib.h>

#ifdef __unix__
#include <sys/stat.h>
#else
#include <sys\stat.h>
#endif

#include "io.h"

/**
 * reads the bit at the given position in the in_buffer and returns it as a BIT type
 * @param BYTE the byte within the in_buffer that the bit is in
 * @param POS the bit position within the byte
 * @return BIT depending on the bit that has been found at the given position
 */
#define GET_BIT(BYTE, POS) \
    (in_buffer[BYTE] & (0x80 >> (POS))) == (0x80 >> (POS)) ? ONE : ZERO

/**
 * writes a bit at the given position to the out_buffer
 * shifts the bitmask to the left in order to overwrite noise in the remaining part of the byte
 * @param BYTE The byte within the out_buffer that has to be written to
 * @param BIT the type of bit that's going to be written - ONE or ZERO
 * @param POS index starting from the right of the byte not, the left
 */
#define PUT_BIT(BYTE, BIT, POS) \
    out_buffer[BYTE] = (BIT == ONE) \
    ? out_buffer[BYTE] | (0x01 << (POS)) \
    : out_buffer[BYTE] & (0xfe << (POS))


/**
* buffer storage to read from
*/
unsigned char in_buffer[BUFF_SIZE];

/**
* buffer storage to be written from
*/
unsigned char out_buffer[BUFF_SIZE];

/**
 * stores the current bit-position within the in_buffer
 */
int in_buffer_position;

/**
 * stores the current bit-position within the out_buffer
 */
int out_buffer_position;

/**
 * stores how many bytes are used of the in_buffer
 */
int in_buffer_usage;

/**
 * stores how many bytes are used of the out_buffer
 */
int out_buffer_usage;

/**
 * pointer to the file-stream of the out_file
 */
FILE* infile;

/**
 * pointer to the file-stream of the out_file
 */
FILE* outfile;

extern void init_in(void)
{
	if (verbose)
	{
		struct stat attribut;
		stat(infile_name, &attribut);

		printf(" - Groesse der EingabeDatei %s (byte): %ld\n", infile_name, attribut.st_size);
	}

	in_buffer_position = 0;
}

extern void init_out(void)
{
	out_buffer_usage = 0;
	out_buffer_position = 0;
}

extern bool has_next_char(void)
{
	bool has_next_char;

	if (in_buffer_position / 8 == in_buffer_usage)
	{
		read_file();
		has_next_char = in_buffer_usage > 0;
	}
	else
	{
		has_next_char = (in_buffer_position / 8) < in_buffer_usage;
	}

	return has_next_char;
}

extern unsigned char read_char(void)
{
	unsigned char output;
	int byte = in_buffer_position / 8;

	output = in_buffer[byte];
	in_buffer_position += 8;

	return output;
}

extern void write_char(unsigned char c)
{
	int byte = out_buffer_position / 8;

	out_buffer[byte] = c;
	out_buffer_position += 8;
	out_buffer_usage = byte + 1;

	if (out_buffer_usage == BUFF_SIZE)
	{
		write_file();
		init_out();
	}
}

extern bool has_next_bit(void)
{
	bool has_next_bit;

	if (in_buffer_position / 8 == in_buffer_usage)
	{
		read_file();
		has_next_bit = in_buffer_usage > 0;
	}
	else
	{
		has_next_bit = (in_buffer_position / 8) < in_buffer_usage;
	}

	return has_next_bit;
}

extern BIT read_bit(void)
{
	BIT output;
	int byte = in_buffer_position / 8;
	int bit_in_byte = in_buffer_position % 8;

	output = GET_BIT(byte, bit_in_byte);
	in_buffer_position++;

	return output;
}

extern void write_bit(BIT c)
{
	int byte = out_buffer_position / 8;
	int bit_in_byte = 7 - (out_buffer_position % 8);// 7 - in order to get the position right to left not left to right

	PUT_BIT(byte, c, bit_in_byte);
	out_buffer_position++;
	out_buffer_usage = byte + 1;

	if (out_buffer_position == BUFF_SIZE * 8)
	{
		write_file();
		init_out();
	}
}

extern void open_infile(void)
{
	init_in();
	infile = fopen(infile_name, "rb");
	if (infile == NULL)
	{
		fprintf(stderr,"FehlerCode %d: EingabeDatei konnte nicht geoeffnet werden", FILE_OR_IO_ERROR);
		exit(FILE_OR_IO_ERROR);
	}
}

extern void open_outfile(void)
{
	init_out();

	outfile = fopen(outfile_name, "wb");
	if (outfile == NULL)
	{
		fprintf(stderr, "FehlerCode %d: AusgabeDatei konnte nicht geoeffnet werden", FILE_OR_IO_ERROR);
		exit(FILE_OR_IO_ERROR);
	}

}

extern void close_infile(void)
{
	fclose(infile);
}

extern void close_outfile(void)
{
	write_file();
	fclose(outfile);

	if (verbose)
	{
		struct stat attribut;
		stat(outfile_name, &attribut);

		printf(" - Groesse der AusgabeDatei %s (byte): %ld\n", outfile_name, attribut.st_size);
	}
}

extern void read_file(void)
{
	in_buffer_usage = fread(in_buffer, sizeof(char), BUFF_SIZE, infile);
	in_buffer_position = 0;
}

extern void write_file(void)
{
	fwrite(out_buffer, sizeof(char), out_buffer_usage, outfile);
	init_out();
}

extern void gotostart_infile(void)
{
	fseek(infile, 0, SEEK_SET);
}


extern void write_int(unsigned int i)
{

	unsigned char* pointer = &i;
	write_char(pointer[0]);
	write_char(pointer[1]);
	write_char(pointer[2]);
	write_char(pointer[3]);

}

extern unsigned int* read_int(void)
{
	unsigned char* pointer = malloc(sizeof(int));

	if (has_next_char())
	{
		pointer[0] = read_char();
	}
	else
	{
		fprintf(stderr, "File empty or locked\n");
		exit(FILE_OR_IO_ERROR);
	}
	if (has_next_char())
	{
		pointer[1] = read_char();
	}
	else
	{
		fprintf(stderr, "File empty or locked\n");
		exit(FILE_OR_IO_ERROR);
	}
	if (has_next_char())
	{
		pointer[2] = read_char();
	}
	else
	{
		fprintf(stderr, "File empty or locked\n");
		exit(FILE_OR_IO_ERROR);
	}
	if (has_next_char())
	{
		pointer[3] = read_char();
	}
	else
	{
		fprintf(stderr, "File empty or locked\n");
		exit(FILE_OR_IO_ERROR);
	}
	return (unsigned int*) pointer;
}