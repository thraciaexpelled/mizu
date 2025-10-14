#ifndef MIZU_CONFIG_H
#define MIZU_CONFIG_H

#include <stdio.h>

// meta
#include <main.h>

typedef struct config_t {
    FILE *config_file;
    int result; // just the errno copied
} config_t;

config_t *config_init(const char *project_directory);
int config_write(config_t *config, opt_t *opt);

opt_t *get_config_from_file(FILE *config_file);

#endif // MIZU_CONFIG_H
