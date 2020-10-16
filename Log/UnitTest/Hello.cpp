#include <iostream>
#include "Log.h"
#include "App.h"

using namespace std;

int main(int argc, char *argv[])
{
    g_app->parseAndInit(argc, argv);
    LOG_TRACE("trace");
    LOG_DEBUG("debug");
    LOG_WARN("warn");
    LOG_ERROR("error");
    LOG_FATAL("fatal");
    g_app->fini();
}

