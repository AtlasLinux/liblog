// file_sink.c
#include "file_sink.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

typedef struct {
    LogSink base;
    pthread_mutex_t mu;
    char* filename;
    FILE* file;
} FileSink;

static int ensure_parent_dirs(const char* path) {
    char* copy = strdup(path);
    if (!copy) return -1;

    for (char* p = copy + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(copy, 0755) < 0 && errno != EEXIST) {
                free(copy);
                return -1;
            }
            *p = '/';
        }
    }

    free(copy);
    return 0;
}

static void file_sink_write(LogSink* self, LogLevel lvl, const char* msg) {
    FileSink* sink = (FileSink*)self;
    
    pthread_mutex_lock(&sink->mu);
    
    if (!sink->file) {
        if (ensure_parent_dirs(sink->filename) < 0) {
            pthread_mutex_unlock(&sink->mu);
            return;
        }
        
        sink->file = fopen(sink->filename, "a");
        if (!sink->file) {
            pthread_mutex_unlock(&sink->mu);
            return;
        }
    }
    
    fputs(msg, sink->file);
    fflush(sink->file);
    pthread_mutex_unlock(&sink->mu);
}

static void file_sink_destroy(LogSink* self) {
    FileSink* sink = (FileSink*)self;
    
    if (sink->file) {
        fclose(sink->file);
    }
    
    free(sink->filename);
    pthread_mutex_destroy(&sink->mu);
    free(sink);
}

LogSink* file_sink_create(const char* filename) {
    FileSink* sink = malloc(sizeof(FileSink));
    if (!sink) return NULL;
    
    sink->base.write = file_sink_write;
    sink->base.destroy = file_sink_destroy;
    sink->filename = strdup(filename);
    sink->file = NULL;
    
    if (pthread_mutex_init(&sink->mu, NULL) != 0) {
        free(sink->filename);
        free(sink);
        return NULL;
    }
    
    return (LogSink*)sink;
}
