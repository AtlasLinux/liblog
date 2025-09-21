#include "console_sink.hpp"
#include "logger.hpp"

void ConsoleSink::write(LogLevel lvl, const std::string& msg) {
    std::lock_guard<std::mutex> lock(mu_);
    std::cerr << to_colour(lvl) << msg << "\033[0m";
}

