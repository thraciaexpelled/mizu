#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>

static int get_terminal_width() {
    struct winsize win;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &win) == -1) {
        fprintf(stderr, "mizu: ioctl: %s\n", strerror(errno));
        return -1;
    }

    return (int)win.ws_col;
}

enum Colors {
    Black = 0,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta
};

const char *colors[] = {
    "\x1b[30m", "\x1b[31m", "\x1b[32m",
    "\x1b[33m", "\x1b[34m", "\x1b[35m",
    "\x1b[0m"
};

#define ARRAY_SIZE(x) sizeof(x) / sizeof(x[0])
#define LAST_INDEX(x) ARRAY_SIZE(x) - 1

static char *paint(const char *str, const char *color) {
    size_t str_len = strlen(str);
    size_t color_len = strlen(color);
    size_t reset_len = strlen(colors[LAST_INDEX(colors)]);
    
    char *buffer = malloc(str_len + color_len + reset_len + 1);
    assert(buffer != NULL);
    snprintf(buffer, str_len + color_len + reset_len + 1, "%s%s%s", color, str, colors[LAST_INDEX(colors)]);
    return buffer;
}

void print_info(const char *msg) {
    char *prefix = paint("info", colors[Blue]);
    fprintf(stdout, "\t%s\t%s\n", prefix, msg);
    free(prefix);
}

void print_warn(const char *msg) {
    char *prefix = paint("warn", colors[Yellow]);
    fprintf(stdout, "\t%s\t%s\n", prefix, msg);
    free(prefix);
}

void print_error(const char *msg, const char *errmsg) {
    char *prefix = paint("fail", colors[Red]);
    fprintf(stderr, "\t%s\t%s: %s\n", prefix,  msg, errmsg);
    free(prefix);
}

void print_progress(const char *type, const char *info, const char *dirname) {
    char *painted_type = paint(type, colors[Magenta]);

    if (strcmp(type, " ok ") == 0) {
        free(painted_type);
        painted_type = paint(type, colors[Green]);
    }

    if (dirname == NULL) {
        fprintf(stdout, "\t%s\t%s\n", painted_type, info);
        free(painted_type);
        return;
    }
    fprintf(stdout, "\t%s\t%s (name: %s)\n", painted_type, info, dirname);
    free(painted_type);
}
