#pragma once
#include "color.h"
struct PointStyle
{
    PointStyle() : visible(false), color(0.0, 0.0, 0.0, 1.0), size(4) {}
    bool visible;
    Color color;
    float size;
};

struct SurfaceStyle
{
    SurfaceStyle() : visible(false), color(0.33f, 0.67f, 1.0f, 0.5f) {}
    bool visible;
    Color color;
};

struct EdgeStyle
{
    EdgeStyle() : visible(false), color(0.f, 0.f, 0.f, 1.f) {}
    bool visible;
    Color color;
    float width;
};