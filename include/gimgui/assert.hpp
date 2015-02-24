#pragma once
#include <iostream>
#include <csignal>

#ifndef SIGTRAP
#define SIGTRAP 5
#endif

#ifndef EMSCRIPTEN
#define GIM_HALT raise(SIGTRAP)
#else
#define GIM_HALT exit(1)
#endif

#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif

#ifndef NDEBUG
#   define GIM_ASSERT(condition, message) \
    do\
    { \
        if(!(condition))\
        { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
            << " function " << __func__ << " line " << __LINE__ << ": " << message << std::endl; \
            GIM_HALT; \
        } \
    } while (false)
#else
#   define GIM_ASSERT(condition, message) do { } while (false)
#endif
