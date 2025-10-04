// logger.c
#include "logger.h"
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Logger {
    LogLevel level;
    LogSink** sinks;
    size_t sink_count;
    size_t sink_capacity;
    pthread_mutex_t mu;
};

static void log_timestamp(char* buf, size_t sz) {
    time_t t = time(NULL);
    struct tm tm;
    localtime_r(&t, &tm);
    strftime(buf, sz, "%Y-%m-%d %H:%M:%S", &tm);
}

Logger* logger_create(LogLevel level) {
    Logger* logger = malloc(sizeof(Logger));
    if (!logger) return NULL;
    
    logger->level = level;
    logger->sink_count = 0;
    logger->sink_capacity = 4;
    logger->sinks = malloc(sizeof(LogSink*) * logger->sink_capacity);
    
    if (pthread_mutex_init(&logger->mu, NULL) != 0) {
        free(logger->sinks);
        free(logger);
        return NULL;
    }
    
    return logger;
}

void logger_destroy(Logger* logger) {
    if (!logger) return;
    
    for (size_t i = 0; i < logger->sink_count; i++) {
        logger->sinks[i]->destroy(logger->sinks[i]);
    }
    free(logger->sinks);
    pthread_mutex_destroy(&logger->mu);
    free(logger);
}

void logger_add_sink(Logger* logger, LogSink* sink) {
    if (!logger || !sink) return;
    
    pthread_mutex_lock(&logger->mu);
    
    if (logger->sink_count >= logger->sink_capacity) {
        logger->sink_capacity *= 2;
        LogSink** new_sinks = realloc(logger->sinks, sizeof(LogSink*) * logger->sink_capacity);
        if (new_sinks) {
            logger->sinks = new_sinks;
        }
    }
    
    if (logger->sink_count < logger->sink_capacity) {
        logger->sinks[logger->sink_count++] = sink;
    }
    
    pthread_mutex_unlock(&logger->mu);
}

void logger_set_level(Logger* logger, LogLevel level) {
    if (!logger) return;
    
    pthread_mutex_lock(&logger->mu);
    logger->level = level;
    pthread_mutex_unlock(&logger->mu);
}

const char* log_level_to_string(LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO:  return "INFO";
        case LOG_WARN:  return "WARN";
        case LOG_ERROR: return "ERROR";
    }
    return "UNKNOWN";
}

void logger_log(Logger* logger, LogLevel level, const char* fmt, ...) {
    if (!logger || level < logger->level) return;
    
    char timestamp[20];
    log_timestamp(timestamp, sizeof(timestamp));
    
    va_list args;
    va_start(args, fmt);
    
    char message[1024];
    vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);
    
    char line[2048];
    snprintf(line, sizeof(line), "%s [%s] %s", timestamp, log_level_to_string(level), message);
    
    pthread_mutex_lock(&logger->mu);
    for (size_t i = 0; i < logger->sink_count; i++) {
        logger->sinks[i]->write(logger->sinks[i], level, line);
    }
    pthread_mutex_unlock(&logger->mu);
}
