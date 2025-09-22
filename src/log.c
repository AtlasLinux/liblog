#define _GNU_SOURCE
#include <time.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "log.h"

static const char* level_names[] = {"DEBUG","INFO","WARN","ERROR"};
static const char* level_colours[] = {"\033[36m","\033[32m","\033[33m","\033[31m"};
static const char* colour_reset = "\033[0m";

static int logfd_console = -1;
static int logfd_file = -1;
int loglevel = 1;
char* logfile = "/log/unknown.log";

void log_init(const char* file, const log_level_t level) {
    logfile = file;
    loglevel = level;
}

static void log_timestamp(char* buf, size_t sz) {
    time_t t = time(NULL);
    struct tm tm;
    localtime_r(&t, &tm);
    strftime(buf, sz, "%Y-%m-%d %H:%M:%S", &tm);
}

static int ensure_parent_dirs(const char *path, mode_t mode) {
    char *copy = strdup(path);
    if (!copy) return -1;

    for (char *p = copy + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(copy, mode) < 0 && errno != EEXIST) {
                free(copy);
                return -1;
            }
            *p = '/';
        }
    }

    free(copy);
    return 0;
}

void log_console_level(log_level_t level, const char *fmt, ...) {
    if (level < loglevel) {
        return;
    }
    char ts[20];
    log_timestamp(ts, sizeof(ts));

    // lazy open
    if (logfd_console < 0) logfd_console = open("/dev/console", O_WRONLY | O_CLOEXEC);
    if (logfd_file < 0) {
        if (ensure_parent_dirs(logfile, 0755) < 0) {
            perror("mkdir");
            exit(1);
        }

        logfd_file = open(logfile, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0644);
        if (logfd_file < 0) {
            perror("open");
            exit(1);
        }
    }

    va_list ap;

    if (logfd_console >= 0) {
        va_start(ap, fmt);
        dprintf(logfd_console, "%s%s %s[%s]%s ", colour_reset, ts, level_colours[level], level_names[level], colour_reset);
        vdprintf(logfd_console, fmt, ap);
        va_end(ap);
    }

    if (logfd_file >= 0) {
        va_start(ap, fmt);
        dprintf(logfd_file, "%s%s %s[%s]%s ", colour_reset, ts, level_colours[level], level_names[level], colour_reset);
        vdprintf(logfd_file, fmt, ap);
        va_end(ap);
    }
}