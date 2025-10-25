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

#ifdef __WIN32
// #include <io.h>
#include <windows.h>
#endif

#define CARRIER_BUFFER_MAX 32768

// taken from my other (unfinished) project
static char *read_file(const char *filepath) {
    FILE *fptr = fopen(filepath, "r");
	if (fptr == NULL) {
	    print_error("fptr on line 27", strerror(errno));
		abort();
	}

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
    DWORD attr = GetFileAttributes(filepath);
    if (attr == INVALID_FILE_ATTRIBUTES)
        return false;
    return true;
#endif

    if (access(filepath, F_OK) == 0)
        return true;
    return false;
}

static enum ProjectStanding {
    ValidJSON, InvalidJSON, JSONHasSyntaxError,
};

static enum ProjectStanding project_is_valid(const char *filepath) {
    char *project_data = read_file(filepath);

    json_error_t err;
    json_t *json_obj = json_loads(project_data, 0, &err);
    if (json_obj == NULL) {
        invalid_syntax(err.line, err.text);
        free(project_data);
        return JSONHasSyntaxError;
    }

    json_t *a, *b, *c;
    a = json_object_get(json_obj, "mizu_project_name");
    b = json_object_get(json_obj, "mizu_project_author");
    c = json_object_get(json_obj, "mizu_project_version");

    // for now, confirm if all are valid
    if (json_is_string(a) && json_is_string(b) && json_is_string(c)) {
        json_decref(json_obj);
        free(project_data);
        return ValidJSON;
    }

    json_decref(json_obj);
    free(project_data);
    return InvalidJSON;
}

static json_t read_json_file(const char *json_filepath) {
    const char *json_file_contents = read_file(json_filepath);

    json_t *error;
    json_t *json_root = json_loads(json_file_contents, 0, &error);

    if (json_root != NULL) {
        return ;
    }
}

// Ok: Exists
// Bad: Doesn't exist
// BadSyntax: Exists but invalid syntax
static enum ProjectIntegrity {
    Ok, Bad, BadSyntax,
};

static enum ProjectIntegrity project_exists(const char *project_filepath) {
    if (!file_exists(project_filepath)) {
        return Bad;
    }

    switch (project_is_valid(project_filepath)) {
        case ValidJSON: return Ok;
        case InvalidJSON: return Ok;
        case JSONHasSyntaxError: return BadSyntax;
    }

    return Ok;
}


static bool project_name_is_valid(const char *project_name) {
    if (strlen(project_name) < 3)
        return false;
    return true;
}

int project_new(opt_t *opt) {
    if (!project_name_is_valid(opt->name)) {
        print_error("invalid project name", opt->name);
        return -1;
    }

    // pathname
    char pathbuf[PATH_MAX];
    snprintf(pathbuf, PATH_MAX, "./%s", opt->name);

    // concatename pathbuf + ./mizu.project.json
    char mizu_project_file[strlen(pathbuf) + strlen("mizu.project.json") + 2];
    snprintf(mizu_project_file, sizeof(mizu_project_file), "%s/%s", pathbuf, "mizu.project.json");

    switch (project_exists(mizu_project_file)) {
        case Ok:
            print_error("project already exists in", mizu_project_file);
            return -1;
        case Bad:
            break;
        case BadSyntax:
            print_warn("fix your project file");
            return -1;
    }

     print_progress("info", "creating new project", opt->name);

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
        return final_result;
    } else {
        print_error("failed to create new project", strerror(final_result));
        fclose(config->config_file);
        free(config);
        return -1;
    }
}

int project_init(opt_t *opt) {
    if (!project_name_is_valid(opt->name)) {
        print_error("invalid project name", optarg);
        return -1;
    }

    switch (project_exists("./mizu.project.json")) {
        case Ok:
            print_error("project by the same name already exists in the specified directory", "./mizu.project.json");
            return -1;
        case Bad:
            break;
        case BadSyntax:
            print_warn("fix your project file");
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
        return final_result;
    } else {
        print_error("failed to initialize project", strerror(final_result));
        free(config);
        return -1;
    }
}

int project_remove(opt_t *opt) {
    char mizu_project_file[PATH_MAX];

    if (strcmp(opt->name, ".") == 0) {
        snprintf(mizu_project_file, sizeof(mizu_project_file), "./%s", "mizu.project.json");
    } else {
        snprintf(mizu_project_file, sizeof(mizu_project_file), "./%s/%s", opt->name, "mizu.project.json");
    }

    switch (project_exists(mizu_project_file)) {
        case Ok:
            break;
        case Bad:
            print_error("project does not exist", opt->name);
            return -1;
        case BadSyntax:
            print_error("project file is corrupted, cannot remove", opt->name);
            return -1;
    }

    if (remove(mizu_project_file) != 0) {
        print_error("cannot remove project", strerror(errno));
        return -1;
    }

    print_progress(" ok ", "removed project", mizu_project_file);

    return 0; // Success
}

int project_info(opt_t *opt) {
    if (!project_name_is_valid(opt->name)) {
        print_error("project does not exist", opt->name);
        return -1;
    }

    // pathname
    char pathbuf[PATH_MAX];
    snprintf(pathbuf, PATH_MAX, "./%s", opt->name);

    // concatename pathbuf + ./mizu.project.json
    char mizu_project_file[strlen(pathbuf) + strlen("mizu.project.json") + 2];
    snprintf(mizu_project_file, sizeof(mizu_project_file), "%s/%s", pathbuf, "mizu.project.json");

    switch (project_exists(mizu_project_file)) {
        case Ok:
            print_error("project already exists in", mizu_project_file);
            return -1;
        case Bad:
            break;
        case BadSyntax:
            print_warn("fix your project file");
            return -1;
    }

    
}