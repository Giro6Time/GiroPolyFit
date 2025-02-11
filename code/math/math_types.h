#pragma once


#include "common.h"
#include "basic_types.h"
class GIROPOLYFIT_API GeometryTypes
{
public:
    // 2d Point
    typedef vecng<2, Numeric::int8> Point2d_int8;
    typedef vecng<2, Numeric::int16> Point2d_int16;
    typedef vecng<2, Numeric::int32> Point2d_int32;
    typedef vecng<2, Numeric::int64> Point2d_int64;
};

typedef vecng<2, Numeric::float32> vec2;
typedef vecng<3, Numeric::float32> vec3;
typedef vecng<4, Numeric::float32> vec4;
