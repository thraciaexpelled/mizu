#ifndef MIZU_MAIN_H
#define MIZU_MAIN_H

#define MIZU_VERSION_META "alpha-0.2"
#define MIZU_DEFAULT_DUMMY_VERSION "alpha-0.1"

extern char *project_version;

enum Job {
    New,
    Init,
    Remove,
};

typedef struct opt_t {
    enum Job job;
    char *name;
} opt_t;

#endif // MIZU_MAIN_H
