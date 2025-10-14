#ifndef MIZU_MAIN_H
#define MIZU_MAIN_H

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
