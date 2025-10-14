#ifndef MIZU_CARRIER_H
#define MIZU_CARRIER_H

#include <stdbool.h>

void print_info(const char *msg);
void print_warn(const char *msg);
void print_error(const char *msg, const char *errmsg);

// wraps around first 3 functions
void print_progress(const char *type, const char *info, const char *dirname);


#endif // MIZU_CARRIER_H
