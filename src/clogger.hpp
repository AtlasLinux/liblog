#ifndef CLOGGER_HPP
#define CLOGGER_HPP

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} clog_level_t;

typedef struct clog_logger clog_logger_t;

typedef enum {
    CLOG_SINK_CONSOLE,
    CLOG_SINK_FILE
} clog_sink_t;

clog_logger_t* clog_create(void);
void clog_destroy(clog_logger_t* logger);

void clog_set_level(clog_logger_t* logger, clog_level_t level);
void clog_add_sink_console(clog_logger_t* logger);
void clog_add_sink_file(clog_logger_t* logger, const char* path);

void clog_log(clog_logger_t* logger, clog_level_t level, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
