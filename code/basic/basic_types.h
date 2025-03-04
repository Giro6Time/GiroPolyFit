#pragma once
#define nil 0

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <string>
#include <vector>
#include <memory>
#include "common.h"
#include "vec.h"

enum Sign
{
    NEGATIVE = -1,
    ZERO = 0,
    POSITIVE = 1
};

namespace String
{
    void split(const std::string &in,
               char seperator,
               std::vector<std::string> &out,
               bool skip_empty_fields = true);

}

/**
 * A namespace gathering typedefs
 * corresponding to numbers. These types
 * names have the form (u)int<size> or float<size>,
 * where the (optional) u denotes an unsigned type,
 * and the size is in bits.
 */

namespace Numeric
{

    typedef char int8;
    typedef short int16;
    typedef int int32;
#ifdef WIN32
    typedef __int64 int64;
#else
    typedef long long int int64;
#endif

    typedef unsigned char uint8;
    typedef unsigned short uint16;
    typedef unsigned int uint32;

#ifdef WIN32
    typedef unsigned __int64 uint64;
#else
    typedef unsigned long long int uint64;
#endif

    typedef float float32;
    typedef double float64;

    typedef void *pointer;

    // -----------------------------------------------------------------

    extern GIROPOLYFIT_API const float big_float;
    extern GIROPOLYFIT_API const float small_float;
    extern GIROPOLYFIT_API const double big_double;
    extern GIROPOLYFIT_API const double small_double;

    bool GIROPOLYFIT_API is_nan(float32 x);
    bool GIROPOLYFIT_API is_nan(float64 x);

    int32 GIROPOLYFIT_API random_int32();
    float32 GIROPOLYFIT_API random_float32();
    float64 GIROPOLYFIT_API random_float64();

    /* Random real number in the range [min, max] */
    inline float32 random_float32(float32 min, float32 max) { return min + random_float32() * (max - min); }
    inline float64 random_float64(float64 min, float64 max) { return min + random_float64() * (max - min); }

    /** Index of maximum of 2 values. */
    template <typename T>
    size_t index_of_max(T a, T b)
    {
        return a > b ? 0 : 1;
    }

    /** Index of maximum of 2 values by magnitude. */
    template <typename T>
    size_t index_of_max_abs(T a, T b)
    {
        return index_of_max(std::fabs(a), std::fabs(b));
    }

    /** Index of minimum of 2 values. */
    template <typename T>
    size_t index_of_min(T a, T b)
    {
        return a < b ? 0 : 1;
    }

    /** Index of minimum of 2 values by magnitude. */
    template <typename T>
    size_t index_of_min_abs(T a, T b)
    {
        return index_of_min(std::fabs(a), std::fabs(b));
    }

    /** Index of maximum of 3 values. */
    template <typename T>
    size_t index_of_max(T a, T b, T c)
    {
        return a > b ? (c > a ? 2 : 0) : (b > c ? 1 : 2);
    }

    /** Index of maximum of 3 values by magnitude. */
    template <typename T>
    size_t index_of_max_abs(T a, T b, T c)
    {
        return index_of_max(std::fabs(a), std::fabs(b), std::fabs(c));
    }

    /** Index of minimum of 3 values. */
    template <typename T>
    size_t index_of_min(T a, T b, T c)
    {
        return a < b ? (c < a ? 2 : 0) : (b < c ? 1 : 2);
    }

    /** Index of minimum of 3 values by magnitude. */
    template <typename T>
    size_t index_of_min_abs(T a, T b, T c)
    {
        return index_of_min(std::fabs(a), std::fabs(b), std::fabs(c));
    }

    template <class T>
    inline T max(T x1, T x2)
    {
        return x1 > x2 ? x1 : x2;
    }

    template <class T>
    inline T min(T x1, T x2)
    {
        return x1 < x2 ? x1 : x2;
    }

    template <class T>
    inline T max(T x1, T x2, T x3)
    {
        return max(max(x1, x2), x3);
    }

    template <class T>
    inline T min(T x1, T x2, T x3)
    {
        return min(min(x1, x2), x3);
    }
}

namespace Memory
{
    typedef unsigned char byte;
    typedef unsigned char word8;
    typedef unsigned short word16;
    typedef unsigned int word32;

    typedef byte *pointer;
    inline void clear(void *addr, size_t size)
    {
        ::memset(addr, 0, size);
    }

    inline void copy(void *to, const void *from, size_t size)
    {
        ::memcpy(to, from, size);
    }

}

namespace Math
{
    typedef vecng<2, Numeric::float32> vec2;
    typedef vecng<3, Numeric::float32> vec3;
    typedef vecng<4, Numeric::float32> vec4;
}