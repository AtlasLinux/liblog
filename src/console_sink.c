// console_sink.c
#include "console_sink.h"
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    LogSink base;
    pthread_mutex_t mu;
    int fd;
} ConsoleSink;

static void console_sink_write(LogSink* self, LogLevel lvl, const char* msg) {
    ConsoleSink* sink = (ConsoleSink*)self;
    
    pthread_mutex_lock(&sink->mu);
    
    if (sink->fd < 0) {
        sink->fd = open("/dev/console", O_WRONLY | O_CLOEXEC);
        if (sink->fd < 0) {
            sink->fd = STDERR_FILENO;
        }
    }
    
    const char* color = "";
    switch (lvl) {
        case LOG_DEBUG: color = "\033[36m"; break;
        case LOG_INFO:  color = "\033[32m"; break;
        case LOG_WARN:  color = "\033[33m"; break;
        case LOG_ERROR: color = "\033[31m"; break;
        default: break;
    }
    
    dprintf(sink->fd, "%s%s\033[0m", color, msg);
    pthread_mutex_unlock(&sink->mu);
}

static void console_sink_destroy(LogSink* self) {
    ConsoleSink* sink = (ConsoleSink*)self;
    
    if (sink->fd >= 0 && sink->fd != STDERR_FILENO) {
        close(sink->fd);
    }
    
    pthread_mutex_destroy(&sink->mu);
    free(sink);
}

LogSink* console_sink_create(void) {
    ConsoleSink* sink = malloc(sizeof(ConsoleSink));
    if (!sink) return NULL;
    
    sink->base.write = console_sink_write;
    sink->base.destroy = console_sink_destroy;
    sink->fd = -1;
    
    if (pthread_mutex_init(&sink->mu, NULL) != 0) {
        free(sink);
        return NULL;
    }
    
    return (LogSink*)sink;
}
