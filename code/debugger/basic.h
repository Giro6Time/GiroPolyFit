#pragma once
#include <string>
#include "common.h"
void abort(const std::string &message);

// assertion
void GIROPOLYFIT_API assertion_failed(
    const std::string &condition_string,
    const std::string &file, int line);

void GIROPOLYFIT_API range_assertion_failed(double value, double min, double max,
                                            const std::string &file, int line);

void GIROPOLYFIT_API should_not_have_reached(
    const std::string &file, int line);

#ifdef ATTIRIBUTE_CHECK
#define my_attribute_assert(x) my_assert(x)
#else
#define my_attribute_assert(x)
#endif

#ifndef NDEBUG
#define my_debug_assert(x) my_assert(x)
#define my_debug_range_assert(x, min_val, max_val) my_range_assert(x, min_val, max_val)
#else
#define my_debug_assert(x)
#define my_debug_range_assert(x, min_val, max_val)
#endif

#define my_assert(x)                                    \
    {                                                   \
        if (!(x))                                       \
        {                                               \
            ::assertion_failed(#x, __FILE__, __LINE__); \
        }                                               \
    }

#define my_range_assert(x, min_val, max_val)              \
    {                                                     \
        if (((x) < (min_val)) || ((x) > (max_val)))       \
        {                                                 \
            ::range_assertion_failed(x, min_val, max_val, \
                                     __FILE__, __LINE__); \
        }                                                 \
    }

#define my_assert_not_reached()                        \
    {                                                  \
        ::should_not_have_reached(__FILE__, __LINE__); \
    }
