#include "file_sink.hpp"
#include <stdexcept>

FileSink::FileSink(const std::string& filename)
    : out_(filename, std::ios::app) {
    if (!out_) throw std::runtime_error("Failed to open log file");
}

void FileSink::write(LogLevel, const std::string& msg) {
    std::lock_guard<std::mutex> lock(mu_);
    out_ << msg;
    out_.flush();
}
