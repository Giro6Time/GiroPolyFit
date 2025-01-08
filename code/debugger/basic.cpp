#include "basic.h"
#include "logger.h"
#include <iostream>

#include <cstdlib>
#include <windows.h>
// #ifndef WIN32
// #include <unistd.h>
// #endif

void abort(const std::string &message)
{
    if (message.length())
        Logger::error("Abort") << message << std::endl;
#ifdef WIN32
    DebugBreak();
#else

#endif
}

void assertion_failed(
    const std::string &condition_string,
    const std::string &file, int line)
{
    Logger::error("Assert") << "Assertion failed: " << condition_string << std::endl;
    Logger::error("Assert") << "File: " << file << std::endl;
    Logger::error("Assert") << "Line: " << line << std::endl;
    abort();
}

void range_assertion_failed(double value, double min, double max,
                            const std::string &file, int line)
{
    Logger::error("Assert") << "Range assertion failed: "
                            << value << " in "
                            << "[ " << min << " ... " << max << " ]"
                            << std::endl;
    Logger::error("Assert") << "File: " << file << std::endl;
    Logger::error("Assert") << "Line: " << line << std::endl;
    abort();
}

void should_not_have_reached(
    const std::string &file, int line)
{
    Logger::error("Assert") << "Control should not have reached this point:" << std::endl;
    Logger::error("Assert") << "File: " << file << std::endl;
    Logger::error("Assert") << "Line: " << line << std::endl;
    abort();
}
