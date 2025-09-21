#include "logger.hpp"

Logger::Logger(LogLevel lvl) : level_(lvl) {}

void Logger::add_sink(std::unique_ptr<LogSink> sink) {
    sinks_.push_back(std::move(sink));
}

void Logger::set_level(LogLevel lvl) {
    level_ = lvl;
}

std::string Logger::timestamp() {
    auto now = std::chrono::system_clock::now();
    auto t   = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
    localtime_r(&t, &tm);
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

const char* to_string(LogLevel lvl) {
    switch (lvl) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
    }
    return "UNKNOWN";
}

const char* to_colour(LogLevel lvl) {
    switch (lvl) {
        case LogLevel::DEBUG: return "\033[36m";
        case LogLevel::INFO:  return "\033[32m";
        case LogLevel::WARN:  return "\033[33m";
        case LogLevel::ERROR: return "\033[31m";
    }
    return "\033[0m";
}
