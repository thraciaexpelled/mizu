#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>

// meta
#include <carrier.h>
#include <config.h>
#include <project.h>

#define CARRIER_BUFFER_MAX 32768

int project_new(opt_t *opt) {
    print_progress("info", "creating NEW project", opt->name);

    // pathname
    char pathbuf[PATH_MAX];
    snprintf(pathbuf, PATH_MAX, "./%s", opt->name);

    if (mkdir(pathbuf, (mode_t)0775) == -1) {
        print_error("failed to create NEW project", strerror(errno));
        return -1;
    }
    
    config_t *config = config_init(pathbuf);
    if (config == NULL) {
        return -1;
    }
    int final_result = config->result;

    if (final_result == 0) {
        print_progress(" ok ", "created NEW project", opt->name);
        free(config);
        return final_result;
    } else {
        print_error("failed to create NEW project", strerror(final_result));
        free(config);
        return -1;
    }
}

int project_init(opt_t *opt) {
    printf("todo: create new directory with the name %s\n", opt->name);
    return 67;
}

int project_remove(opt_t *opt) {
    printf("todo: create new directory with the name %s\n", opt->name);
    return 67;
}
