#include "logger.hpp"

#include <array>
#include <cstdarg>
#include <cstdio>
#include <mutex>
#include <string_view>

namespace
{
    constexpr std::size_t LOG_BUFFER_SIZE = 4096;

    [[nodiscard]]
    auto levelToString(Log::Level level) -> std::string_view
    {
        switch (level)
        {
        case Log::Level::Fatal:
            return "FATAL";

        case Log::Level::Error:
            return "ERROR";

        case Log::Level::Warn:
            return "WARN";

        case Log::Level::Info:
            return "INFO";

        case Log::Level::Debug:
            return "DEBUG";

        case Log::Level::Trace:
            return "TRACE";
        }

        return "UNKNOWN";
    }

    Log::Level currentLevel = Log::Level::Info;
    std::mutex outputMutex;
}

void Log::setLevel(Log::Level level)
{
    currentLevel = level;
}

void Log::output(Log::Level level, const char* fmt, ...)
{
    if (static_cast<int>(level) > static_cast<int>(currentLevel))
    {
        return;
    }

    std::array<char, LOG_BUFFER_SIZE> message{};

    va_list args;
    va_start(args, fmt);
    std::vsnprintf(message.data(), message.size(), fmt, args);
    va_end(args);

    std::lock_guard<std::mutex> lock(outputMutex);
    std::fprintf(stderr, "[%s] %s\n", levelToString(level).data(), message.data());
}