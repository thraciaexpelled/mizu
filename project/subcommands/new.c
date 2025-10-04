#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <linux/limits.h>
#include <sys/stat.h>

// meta
#include <structs.h>
#include <path-join/path-join.h>
#include <log.h>
#include <config_writer.h>
#include <strdup/strdup.h>

static const char *get_symbolic_name(ProjectType p) {
	switch (p) {
		case Program: return "program";
		case Library: return "library";
	}

	log_error("Invalid project_type: %d", p);
	return (const char*)NULL;
}

subc_new_t *new_init(int argc, char **argv) {
	subc_new_t *options = malloc(sizeof(subc_new_t*) + sizeof(const char*) * 3);
	if (options == NULL) {
		log_fatal("errno %d: %s\n", errno, strerror(errno));
		return (subc_new_t*)-1;
	}

	int c;
	optind = 1;
	while ((c = getopt(argc, argv, "n:t:l:h")) != -1) {
		switch (c) {
			case 'n': {
				options->name = strdup(optarg);
				break;
			}
			case 't': {
				// there are only two types of project mizu supports
				// safe to use else-if statements
				if (strcmp(optarg, "program") == 0) {
					options->project_type = Program;
				} else if (strcmp(optarg, "library") == 0){
					options->project_type = Library;
				} else {
					fprintf(stderr, "mizu: invalid project_type: %s\n", optarg);
					fprintf(stderr, "Note: project_type's are case-sensitive. Check if you are misspelling something\n");
					free(options);
					return (subc_new_t*)NULL;
				}
				break;
			}
			case 'l': {
				// TODO: check if this is in a valid SPDX format
				options->license = strdup(optarg);
				break;
			}
			case 'h': {
				fprintf(stderr, "mizu: -h: not implemented yet\n");
				break;
			}
			default: {
				free(options);
				return (subc_new_t*)-1;
			}
		}
	}

	if (options->license == NULL)
		options->license = strdup("MIT");

	return options;
}

#define MIZU_INI_WRITER

int start(subc_new_t *options) {
	// get cwd first
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) == NULL) {
		log_fatal("errno %d: %s\n", errno, strerror(errno));
		free(options);
		return -1;
	}

	log_info("About to create directory with name '%s'", options->name);
	
	// build project filepath
	const char *project_filepath = path_join(cwd, options->name);
	assert(project_filepath != NULL);

	if (mkdir(project_filepath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
		log_fatal("errno %d: %s\n", errno, strerror(errno));
		free(options);
		free((void *)project_filepath);
		return -1;
	}

	log_info("Succesfully created directory in '%s'\n", project_filepath);

	// write config
	log_info("About to write configuration for project '%s'\n", options->name);

	const char *project_config_path = path_join(project_filepath, "mizu.ini");
	assert(project_config_path != NULL);

	FILE *f = fopen(project_config_path, "a");
	if (f == NULL) {
		log_fatal("errno %d: %s\n", errno, strerror(errno));
		free(options);
		free((void *)project_filepath);
		free((void *)project_config_path);
		return -1;
	}

	assert(0 == write_config_header(f, "mizu-project"));											// write header
	assert(0 == write_config_keyval(f, "name", options->name));										// write data (project name)
	assert(0 == write_config_keyval(f, "project-type", get_symbolic_name(options->project_type)));	// write data (project type)
	assert(0 == write_config_keyval(f, "license", options->license));									// write data (project license)
	
	// close file
	fclose(f);

	// clean up
	free(options);
	free((void *)project_filepath);
	free((void *)project_config_path);

	return 0;
}