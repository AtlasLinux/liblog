// logger.h
#ifndef LOGGER_H
#define LOGGER_H

#include "log_sink.h"
#include <stdarg.h>

typedef struct Logger Logger;

Logger* logger_create(LogLevel level);
void logger_destroy(Logger* logger);
void logger_add_sink(Logger* logger, LogSink* sink);
void logger_set_level(Logger* logger, LogLevel level);
void logger_log(Logger* logger, LogLevel level, const char* fmt, ...);

const char* log_level_to_string(LogLevel level);

#endif
