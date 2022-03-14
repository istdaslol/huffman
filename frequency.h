#ifndef FREQUENCY_H
#define FREQUENCY_H

typedef struct frequency
{
	int ascii_code;
	unsigned int count;
	struct frequency* left_child;
	struct frequency* right_child;
} FREQUENCY;

/**
 * compares two frequency's
 * @param f1
 * @param f2
 * @return 1 if c1 is bigger 0 if both are equal -1 if c2 is bigger
 */
extern int cmp_frequency(FREQUENCY* f1, FREQUENCY* f2);

/**
 * prints the ascii-code of a frequency f
 * @param f frequency of wich the ascii-code has to be printed
 */
extern void print_frequency(FREQUENCY* f);

#endif //FREQUENCY_H
