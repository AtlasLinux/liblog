#pragma once
#include <chrono>
#include <ctime>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "log_sink.hpp"

enum class LogLevel { DEBUG, INFO, WARN, ERROR };

const char* to_string(LogLevel lvl);
const char* to_colour(LogLevel lvl);

class Logger {
public:
    explicit Logger(LogLevel lvl = LogLevel::INFO);

    void add_sink(std::unique_ptr<LogSink> sink);
    void set_level(LogLevel lvl);

    template <typename... Args>
    void log(LogLevel lvl, const std::string& fmt, Args&&... args) {
        if (lvl < level_) return;

        std::ostringstream oss;
        oss << timestamp() << " [" << to_string(lvl) << "] "
            << format(fmt, std::forward<Args>(args)...) << "\n";

        const std::string line = oss.str();
        for (auto& sink : sinks_) {
            sink->write(lvl, line);
        }
    }

private:
    LogLevel level_;
    std::vector<std::unique_ptr<LogSink>> sinks_;

    static std::string timestamp();

    template <typename... Args>
    static std::string format(const std::string& fmt, Args&&... args) {
        size_t size = snprintf(nullptr, 0, fmt.c_str(), args...) + 1;
        std::unique_ptr<char[]> buf(new char[size]);
        snprintf(buf.get(), size, fmt.c_str(), args...);
        return std::string(buf.get(), buf.get() + size - 1);
    }
};

