#include <structs.h>

#ifndef MIZU_SUBCOMMAND_NEW
#define MIZU_SUBCOMMAND_NEW

// this function uses getopt to parse command line arguments
subc_new_t *new_init(int argc, char **argv);

// this function actually implements functionality
int start(subc_new_t *options);

#endif // MIZU_SUBCOMMAND_NEW