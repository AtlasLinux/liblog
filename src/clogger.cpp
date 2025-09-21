#include "clogger.hpp"
#include "logger.hpp"
#include "console_sink.hpp"
#include "file_sink.hpp"

#include <cstdarg>
#include <cstdio>

struct clog_logger {
    Logger logger;
};

extern "C" {

clog_logger_t* clog_create(void) {
    return new clog_logger{ Logger() };
}

void clog_destroy(clog_logger_t* logger) {
    delete logger;
}

void clog_set_level(clog_logger_t* logger, clog_level_t level) {
    logger->logger.set_level(static_cast<LogLevel>(level));
}

void clog_add_sink_console(clog_logger_t* logger) {
    logger->logger.add_sink(std::make_unique<ConsoleSink>());
}

void clog_add_sink_file(clog_logger_t* logger, const char* path) {
    logger->logger.add_sink(std::make_unique<FileSink>(path));
}

void clog_log(clog_logger_t* logger, clog_level_t level, const char* fmt, ...) {
    if (!logger) return;
    char buf[2048];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    logger->logger.log(static_cast<LogLevel>(level), buf);
}

}
