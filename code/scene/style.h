#pragma once
#include "color.h"
struct PointStyle
{
    PointStyle() : visible(false), color(0.0, 0.0, 0.0, 1.0), size(4) {}
    bool visible;
    Color color;
    float size;
};