#define _GNU_SOURCE
#include <time.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>

#define log_debug(fmt, ...) log_console_level(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...)  log_console_level(LOG_INFO, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...)  log_console_level(LOG_WARN, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) log_console_level(LOG_ERROR, fmt, ##__VA_ARGS__)

typedef enum { LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR } log_level_t;

extern int loglevel;

void log_console_level(log_level_t level, const char *fmt, ...);