#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __WIN32
#include <lmcons.h>
#include <windows.h>
#else
#include <pwd.h>
#include <unistd.h>
#endif

// meta
#include <carrier.h>
#include <config.h>
#include <main.h>


static const char *get_name_of_current_user();
#ifdef __WIN32

// these two functions are vibe coded because as much as i'm
// a win32api shill, i still don't know shi about it
static WCHAR* get_username() {
    WCHAR username_buffer[UNLEN + 1];
    DWORD username_len = UNLEN + 1;

    if (GetUserNameW(username_buffer, &username_len)) {
        WCHAR* username_heap = malloc(username_len * sizeof(WCHAR));
        if (username_heap == NULL) {
            print_warn("failed to retrieve username; falling back");
            return L"anonymous";
        }

        lstrcpyW(username_heap, username_buffer);
        return username_heap;
    }

    print_warn("failed to retrieve username; falling back");
    return L"anonymous";
}

static char* convert_wchar_to_mbs(const wchar_t* wide_str) {
    if (wide_str == NULL) {
        return NULL;
    }

    size_t required_size = wcstombs(NULL, wide_str, 0);

    if (required_size == (size_t)-1) {
        print_warn("username probably contains invalid characters");
        return NULL;
    }

    char* mb_str = (char*)malloc(required_size + 1);
    if (mb_str == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    size_t result = wcstombs(mb_str, wide_str, required_size + 1);

    if (result == (size_t)-1) {
        free(mb_str);
        return NULL;
    }

    return mb_str;
}

static const char *get_name_of_current_user() {
    char *u1, *u2;
    u1 = convert_wchar_to_mbs(get_username());
    if (u1 == NULL) {
        print_warn("failed to get name of correct user and user has not provided username. falling back");
        return "anonymous";
    }
    u2 = strdup(u1);

    free(u1);
    return u2;
}
#else
static const char *get_name_of_current_user() {
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if (pw == NULL) {
        print_error(
            "cannot get the name for the current user. falling back to `anonymous' as the default author name",
            strerror(errno)
        );
        return "anonymous";
    }
    return pw->pw_name;
}
#endif

config_t *config_init(const char *project_directory) {
    config_t *config = malloc(sizeof(config_t*) + sizeof(FILE*));
    if (config == NULL) {
        print_error("failed to create crucial project file", strerror(errno));
        return NULL;
    }

    // open file
    char config_filedir[PATH_MAX];
    snprintf(config_filedir, sizeof(config_filedir), "%s/mizu.project.json", project_directory);

    FILE *fptr = fopen(config_filedir, "w");
    if (fptr == NULL) {
        print_error("cannot open config file", strerror(errno));
        config->config_file = NULL;
        config->result += errno;
        return config;
    }

    config->config_file = fptr;
    config->result = 0;

    return config;
}

int config_write(config_t *config, opt_t *opt) {
    // write dummy shit for now
    assert(config->config_file != NULL);
    assert(config->result == 0);

    // config + project name
    json_t *json_obj = json_object();
    json_object_set_new(json_obj, "mizu_project_name", json_string(opt->name));

    // author
    const char *mizu_project_author = get_name_of_current_user();
    json_object_set_new(json_obj, "mizu_project_author", json_string(mizu_project_author));

    // version
    json_object_set_new(json_obj, "mizu_project_version", json_string(project_version));

    char *json_out = json_dumps(json_obj, JSON_INDENT(4));
    assert(json_out != NULL);

    if (fputs(json_out, config->config_file) == -1) {
        print_error("cannot write config", strerror(errno));
        free(json_out);
        json_decref(json_obj);
        free(config);
        return -1;
    }

    // newline is required for jansson to function properly
    if (fputs("\n", config->config_file) == -1) {
        print_error("cannot write config", strerror(errno));
        free(json_out);
        json_decref(json_obj);
        free(config);
        return -1;
    }

    free(json_out);
    json_decref(json_obj);
    fclose(config->config_file);
    free(config);
    return 0;
}
