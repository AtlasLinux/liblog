#pragma once
#include <string>

enum class LogLevel;

class LogSink {
public:
    virtual ~LogSink() = default;
    virtual void write(LogLevel lvl, const std::string& msg) = 0;
};
