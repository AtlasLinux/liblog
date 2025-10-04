// log_sink.h
#ifndef LOG_SINK_H
#define LOG_SINK_H

#include <stddef.h>

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

typedef struct LogSink LogSink;

struct LogSink {
    void (*write)(LogSink* self, LogLevel lvl, const char* msg);
    void (*destroy)(LogSink* self);
};

#endif
