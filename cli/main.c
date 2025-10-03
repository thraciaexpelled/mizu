#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// meta
#include <log.h>
#include <structs.h>

#define VERSION "alpha-0.1"

static void print_help_meta() {
	printf("Usage: mizu [COMMAND] <SUBCOMMAND>\n");
	printf("Package manager and build system for C\n\n");
	printf("Commands:\n");
	printf("	new\tCreates a new project.\n");
	printf("	init\tInitializes a project.\n");
	printf("Run `[COMMAND] -h` to get information about that command.\n\n");
	printf("Options:\n");
	printf("	-h\tPrints this message and exit\n");
	printf("	-v\tPrint usually hidden messages\n");
	printf("	-V\tPrint version and exit\n\n");
	printf("mizu version '%s'; (c) mizu authors 2025, all rights reserved\n", VERSION);
	exit(0);
}

// a good way to not create else-if nests is to
// create an enum representation of our 
// subcommands, create an string array containing
// our desired subcommands, iterate through them
// and then return that enum.

#define MIZU_SUBCOMMANDS

static Subcommand retrieve_current_subcommand(char *subcommand) {
	assert(subcommand != NULL);
	for (int i = 0; i < subcommand_index; ++i) {
		if (strcmp(subcommand, subcommands[i]) == 0) {
			assert(i < 2);
			Subcommand current_subcommand = i;
			return current_subcommand;
		}
	}

	fprintf(stderr, "mizu: invalid command: %s\n", subcommand);
	return (Subcommand)NULL;
}

int main(int argc, char *argv[]) {
	log_set_level(LOG_ERROR);

	if (argc < 2)
		print_help_meta();

	// global options stay before the subcommmands
	// assuming so, shift 'subcommand_location' by
	// '1' if 'getopt_set' is indeed true
	int subcommand_location = 1;
	bool getopt_set = false;

	// we should parse global option first i suppose
	int c;
	while ((c = getopt(argc, argv, "hvV")) != -1) {
		// if we match with any of these, we should set 'getopt_set' to true
		switch (c) {
			case 'h': {
				getopt_set = true;
				print_help_meta();
				break;
			}

			case 'v': {
				getopt_set = true;
				log_set_level(LOG_TRACE);
				break;
			}

			case 'V': {
				getopt_set = true;
				printf("%s\n", VERSION);
				return 0;
			}
		}
	}

	// let's handle subcommands too :)
	
	// (check line 30)
	if (getopt_set)
		subcommand_location += 1;

	char *subcommand = argv[subcommand_location];

	if (subcommand == NULL) {
		fprintf(stderr, "mizu: no command entered\n\n");
		print_help_meta();
		return -1;
	}

	// match through the subcommand that we got
	Subcommand sb = retrieve_current_subcommand(subcommand);
	switch (sb) {
		case New: /* new */
			fprintf(stderr, "mizu: new: not implemented yet\n");
			break;
		case Init: /* init */
			fprintf(stderr, "mizu: init: not implemented yet\n");
	}

	return 0;
}