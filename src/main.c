#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// meta
#include <main.h>
#include <project.h>

static void print_help(char *argv[]) {
    printf("Usage: %s -<options> -<flags> <name>\n", argv[0]);
    printf("Build system and package manager for C/C++\n\n");
    printf("Options:\n");
    printf("    -I  initialize project in the cwd\n");
    printf("    -N  create a new project\n");
    printf("    -R  remove all mizu related files and build remnants\n\n");
    printf("Flags: (nil)\n\n");
    printf("mizu is free and open source software under the terms and conditions of the MIT license\n");
    printf("Copyright (c) 2025-present thraciaexpelled/clangjesus, All rights reserved\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s -[I|N|R] -<options> <name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    opt_t *opt = malloc(sizeof(opt_t*) + sizeof(const char*));
    if (opt == NULL) {
        fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
        exit(EXIT_FAILURE);
    }

    int c;
    while ((c = getopt(argc, argv, "I:N:R:hV")) != -1){
        switch (c) {
            case 'I':
                opt->job = Init;
                opt->name = strdup(optarg);
                break;
            case 'N':
                opt->job = New;
                opt->name = strdup(optarg);
                break;
            case 'R':
                opt->job = Remove;
                opt->name = strdup(optarg);
                break;
            case 'h':
                print_help(argv);
                free(opt->name);
                free(opt);
                return 0;
            case 'V':
                puts("alpha-0.1");
                free(opt->name);
                free(opt);
                return 0;
            default:
                fprintf(stderr, "Try `%s -h` for a list of options\n", argv[0]);
                free(opt->name);
                free(opt);
                return EXIT_FAILURE;
        }
    }
    
    printf("\n");
    switch (opt->job) {
        case New:
            int c1 = project_new(opt);
            printf("\n");
            return c1;
        case Init:
            int c2 = project_init(opt);
            printf("\n");
            return c2;
        case Remove:
            int c3 = project_remove(opt);
            printf("\n");
            return c3;
    }


    free(opt->name);
    free(opt);
    return 0;
}
