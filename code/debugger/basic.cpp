#include "basic.h"
#include "logger.h"
#include <iostream>

#include <cstdlib>
#include <windows.h>
// #ifndef WIN32
// #include <unistd.h>
// #endif

static void abort()
{
#ifdef WIN32
    DebugBreak();
#else

#endif
}
