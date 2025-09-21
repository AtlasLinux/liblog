#pragma once
#include "log_sink.hpp"
#include <mutex>
#include <iostream>

class ConsoleSink : public LogSink {
public:
    void write(LogLevel lvl, const std::string& msg) override;

private:
    std::mutex mu_;
};
