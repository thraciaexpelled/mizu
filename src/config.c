#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// meta
#include <carrier.h>
#include <config.h>

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
        // free(config);
        // return NULL;
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
    fprintf(config->config_file, "omaigaa hiiiiiI!!!!!!!!!!!!! :3333\n");
    free(config);
    return 0;
}
