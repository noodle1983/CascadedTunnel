#include <iostream>
#include "Log.h"

using namespace std;

int main()
{
    LOG_TRACE("trace");
    LOG_DEBUG("debug");
    LOG_WARN("warn");
    LOG_ERROR("error");
    LOG_FATAL("fatal");
}

