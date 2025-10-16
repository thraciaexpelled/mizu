#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <jansson.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>

// meta
#include <carrier.h>
#include <config.h>
#include <project.h>

#define CARRIER_BUFFER_MAX 32768

// taken from my other (unfinished) project
static char *read_file(const char *filepath) {
    FILE *fptr = fopen(filepath, "r");
	assert(fptr != NULL);

	char *buffer = 0;
	long buflen;

	fseek(fptr, 0, SEEK_END);
	buflen = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);

	buffer = malloc(buflen);
	assert(buffer != NULL);

	fread(buffer, 1, buflen, fptr);
	fclose(fptr);

	return buffer;
}

static void invalid_syntax(int line, const char *text) {
    char b[CARRIER_BUFFER_MAX];
    snprintf(b, sizeof(b), "line %d: %s", line, text);
    print_error("invalid syntax", b);
}

// check if existing (valid) mizu project exists
static bool file_exists(const char *filepath) {
    #ifdef __WIN32
        print_warn("this function is not implemented for windows platforms. expect bugs");
        return true;
    #endif

    if (access(filepath, F_OK) == 0)
        return true;
    return false;
}

static bool project_is_valid(const char *filepath) {
    char *project_data = read_file(filepath);

    json_error_t err;
    json_t *json_obj = json_loads(project_data, 0, &err);
    if (json_obj == NULL) {
        invalid_syntax(err.line, err.text);
        free(project_data);
        return false;
    }

    json_t *a, *b, *c;
    a = json_object_get(json_obj, "mizu_project_name");
    b = json_object_get(json_obj, "mizu_project_author");
    c = json_object_get(json_obj, "mizu_project_version");

    // for now, confirm if all are valid
    if (json_is_string(a) && json_is_string(b) && json_is_string(c)) {
        json_decref(json_obj);
        free(project_data);
        return true;
    }

    json_decref(json_obj);
    free(project_data);
    return false;
}

static bool project_exists(const char *project_filepath) {
    if (!file_exists(project_filepath)) {
        return false;
    }

    if (!project_is_valid(project_filepath)) {
        print_warn("file is not in correct format");
        return false;
    }

    return true;
}

int project_new(opt_t *opt) {
    // pathname
    char pathbuf[PATH_MAX];
    snprintf(pathbuf, PATH_MAX, "./%s", opt->name);

    print_progress("info", "creating new project", opt->name);

    // concatename pathbuf + ./mizu.project.json
    char mizu_project_file[PATH_MAX];
    snprintf(mizu_project_file, sizeof(mizu_project_file), "%s/%s", pathbuf, "mizu.project.json");

    if (project_exists(mizu_project_file)) {
        print_error("project by the same name already exists in the specified directory", mizu_project_file);
        return -1;
    }

    #ifdef __WIN32
        if (mkdir(pathbuf) == -1) {
            print_error("failed to create new project", strerror(errno));
            return -1;
        }
    #else
        if (mkdir(pathbuf, (mode_t)0775) == -1) {
            print_error("failed to create new project", strerror(errno));
            return -1;
        }
    #endif

    config_t *config = config_init(pathbuf);
    if (config == NULL)
        return -1;

    int final_result = config->result;
    if (final_result == 0) {
        assert(config_write(config, opt) == 0);
        print_progress(" ok ", "created new project", opt->name);
        //free(config);
        fclose(config->config_file);
        return final_result;
    } else {
        print_error("failed to create new project", strerror(final_result));
        fclose(config->config_file);
        free(config);
        return -1;
    }
}

int project_init(opt_t *opt) {
    if (project_exists("./mizu.project.json")) {
        print_error("project by the same name already exists in the specified directory", "./mizu.project.json");
        return -1;
    }

    print_progress("info", "initializing project", opt->name);

    // have to do nothing else, just create config
    config_t *config = config_init(".");
    if (config == NULL)
        return -1;

    int final_result = config->result;
    if (final_result == 0) {
        assert(config_write(config, opt) == 0);
        print_progress(" ok ", "initialized project", opt->name);
        //free(config);
        // fclose(config->config_file);
        return final_result;
    } else {
        print_error("failed to initialize project", strerror(final_result));
        // fclose(config->config_file);
        free(config);
        return -1;
    }
}

int project_remove(opt_t *opt) {
    printf("todo: create new directory with the name %s\n", opt->name);
    return 67;
}
