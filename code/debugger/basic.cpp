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
#ifdef WIN32
    if (message.length())
        OutputDebugString(message.c_str());
    DebugBreak();
#else

#endif
}
