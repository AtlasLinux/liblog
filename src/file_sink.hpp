#pragma once
#include "log_sink.hpp"
#include <fstream>
#include <mutex>
#include <string>

class FileSink : public LogSink {
public:
    explicit FileSink(const std::string& filename);
    void write(LogLevel lvl, const std::string& msg) override;

private:
    std::ofstream out_;
    std::mutex mu_;
};

