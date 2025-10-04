#include <errno.h>
#include <stdio.h>
#include <string.h>

// meta
#include <config_writer.h>
#include <log.h>

#define MIZU_INI_WRITER

int write_config_header(FILE *f, const char *data) {
	log_info("Checking if 'f' is NULL");
	if (f == NULL) {
		log_fatal("'f' is NULL. cannot continue (err: %s\n)", strerror(errno));
		return -1;
	}

	log_info("About to write %d bytes of data...", sizeof(data));
	fprintf(f, "[%s]\n", data);
	return 0;
}

int write_config_keyval(FILE *f, const char *key, const char *val) {
	log_info("Checking if 'f' is NULL");
	if (f == NULL) {
		log_fatal("'f' is NULL. cannot continue (err: %s\n)", strerror(errno));
		return -1;
	}

	log_info("About to write %d bytes of data...", (sizeof(key) + sizeof(val)));
	fprintf(f, "%s = %s\n", key, val);
	return 0;
}