#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "huffman_common.h"
#include "huffman.h"

void print_help(void)
{
	printf("huffman <options> <filename>\n"
		   "wobei folgende Optionen beim Aufruf angegeben werden koennen:\n"
		   " -c Die Eingabedatei wird komprimiert.\n"
		   " -d Die Eingabedatei wird dekomprimiert.\n"
		   "	Sind im Aufruf beide Optionen -c und -d angegeben, bestimmt\n"
		   "	die letzte Angabe, ob komprimiert oder dekomprimiert wird.\n"
		   " -v Gibt Informationen ueber die Komprimierung bzw. Dekomprimierung aus,\n"
		   "	mind. die Groesse der Ein- und der Ausgabedatei\n"
		   "	sowie die Programmlaufzeit in Sekunden. (s. Hinweise 2 und 3)\n"
		   " -o <outfile_name> Legt den Namen der Ausgabedatei fest.\n"
		   "	Wird die Option weggelassen, wird der Name der Ausgabedatei standardmaessig festgelegt.\n"
		   "	Dazu wird bei einer Komprimierung der Dateiname um\n"
		   "	die Endung .hc und bei einer Dekomprimierung um die Endung .hd erweitert.\n"
		   " -h Zeigt eine Hilfe an, die die Benutzung des Programms erklaert.\n"
		   " <filename> Name der Eingabedatei");
}

static void read_arguments(int argc, char* argv[])
{
	verbose = false;
	help = false;
	has_outfile = false;

	int i;
	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			if (argv[i][1] == 'h')
			{
				help = true;
			}
			else if (argv[i][1] == 'v')
			{
				verbose = true;
			}
			else if (argv[i][1] == 'c')
			{
				selected_mode = 'c';
			}
			else if (argv[i][1] == 'd')
			{
				selected_mode = 'd';
			}
			else if (argv[i][1] == 'o')
			{
				if ((i + 1) < argc && argv[i + 1][0] != '-')
				{
					i++;
					outfile_name = argv[i];
					has_outfile = true;
				}
				else
				{
					fprintf(stderr, "FehlerCode %d: Kein oder ungueltiger Name fuer die AusgabeDatei", PARAMETER_ERROR);
					exit(PARAMETER_ERROR);
				}
			}
			else
			{
				fprintf(stderr, "FehlerCode %d: Ungueltiger Parameter: %s", PARAMETER_ERROR, argv[i]);
				exit(PARAMETER_ERROR);
			}
		}
		else if (i == argc - 1)
		{
			infile_name = argv[i];
		}
			// shouldn't be able to be entered but is still there as a default "fallback"
		else
		{
			fprintf(stderr, "FehlerCode %d: Ungueltiger Parameter %s", PARAMETER_ERROR, argv[i]);
			exit(PARAMETER_ERROR);
		}
	}
	if (selected_mode != 'c' && selected_mode != 'd')
	{
		fprintf(stderr, "FehlerCode %d: Kein Modus gewaehlt", PARAMETER_ERROR);
		exit(PARAMETER_ERROR);
	}

	if (i <= 1)
	{
		fprintf(stderr, "FehlerCode %d: Zu wenig Parameter", PARAMETER_ERROR);
		exit(PARAMETER_ERROR);
	}

}

int main(int argc, char* argv[])
{
	read_arguments(argc, argv);

	if (!has_outfile)
	{
		char* created_outfile_name = calloc(strlen(infile_name) + 4, sizeof(char));
		strcat(created_outfile_name, infile_name);
		strcat(created_outfile_name, (selected_mode == 'c') ? ".hc" : ".hd");
		outfile_name = created_outfile_name;
		has_outfile = true;
	}
	if (strcmp(infile_name, outfile_name) == 0)
	{
		fprintf(stderr, "FehlerCode %d: Ein- und AusgabeDatei heissen identisch", PARAMETER_ERROR);
		exit(PARAMETER_ERROR);
	}

	if (help)
	{
		print_help();
		exit(NO_ERROR);
	}

	clock_t prg_start = clock();
	if (selected_mode == 'd')
	{
		decompress();
	}
	else
	{
		compress();
	}

	if (verbose)
	{
		clock_t prg_end = clock();
		printf(" - total time to %s: %.2f Seconds\n",
				(selected_mode == 'd' ? "decompress" : "compress"),
				(float)(prg_end - prg_start) / CLOCKS_PER_SEC);
	}

	return EXIT_SUCCESS;
}
