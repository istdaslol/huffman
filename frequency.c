#include <stdio.h>
#include "frequency.h"

extern int cmp_frequency(FREQUENCY* f1, FREQUENCY* f2)
{
	return (f1->count == f2->count)
		   ? 0
		   : (f1->count > f2->count) ? 1 : -1;
}
extern void print_frequency(FREQUENCY* f)
{
	printf("%d", f->ascii_code);
}

