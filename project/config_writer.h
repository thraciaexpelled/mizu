#include <stdio.h>

#ifndef MIZU_INI_WRITER
#define MIZU_INI_WRITER

// there are no generics in C. we will use this function to write data line by line
int write_config_header(FILE *f, const char *data);
int write_config_keyval(FILE *f, const char *key, const char *val);

#endif // MIZU_INI_WRITER