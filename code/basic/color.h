#pragma once
#include "basic_types.h"
#include "assert.h"
#include <iostream>

class Color
{
private:
    typedef Numeric::float32 float32;

public:
    Color(float32 r = 0, float32 g = 0, float32 b = 0, float32 a = 1.0f)
    {
        components_[0] = r;
        components_[1] = g;
        components_[2] = b;
        components_[3] = a;
    }

    float32 r() const { return components_[0]; }
    float32 g() const { return components_[1]; }
    float32 b() const { return components_[2]; }
    float32 a() const { return components_[3]; }

    void set_r(float32 r) { components_[0] = r; }
    void set_g(float32 g) { components_[1] = g; }
    void set_b(float32 b) { components_[2] = b; }
    void set_a(float32 a) { components_[3] = a; }

    void set(float32 r, float32 g, float32 b, float32 a)
    {
        components_[0] = r;
        components_[1] = g;
        components_[2] = b;
        components_[3] = a;
    }

    float32 &operator[](int i)
    {
        assert(i >= 0 && i <= 3);
        return components_[i];
    }
    

    const float32 &operator[](int i) const
    {
        assert(i >= 0 && i <= 3);
        return components_[i];
    }

    bool operator<(const Color &rhs) const
    {
        if (components_[0] < rhs.components_[0])
            return true;
        if (components_[0] > rhs.components_[0])
            return false;
        if (components_[1] < rhs.components_[1])
            return true;
        if (components_[1] > rhs.components_[1])
            return false;
        if (components_[2] < rhs.components_[2])
            return true;
        if (components_[2] > rhs.components_[2])
            return false;
        return (components_[3] < rhs.components_[3]);
    }
    static Color random_color(bool allow_dark);
    static Color fused_color(const Color &c1, float w1, const Color &c2, float w2);

private:
    float32 components_[4];
};

/// @brief 随机颜色
/// @param allow_dark 允许颜色为全黑
inline Color Color::random_color(bool allow_dark = false)
{
    float min_rgb = 0.3f;
    if (allow_dark)
        min_rgb = 0.0f;

    return Color(Numeric::random_float32(min_rgb, 1.0f), Numeric::random_float32(min_rgb, 1.0f), Numeric::random_float32(min_rgb, 1.0f));
}

/// @brief 融合两个颜色
inline Color Color::fused_color(const Color &c1, float w1, const Color &c2, float w2)
{
    float r = (c1.r() * w1 + c2.r() * w2) / (w1 + w2);
    float g = (c1.g() * w1 + c2.g() * w2) / (w1 + w2);
    float b = (c1.b() * w1 + c2.b() * w2) / (w1 + w2);
    float a = (c1.a() * w1 + c2.a() * w2) / (w1 + w2);
    return Color(r, g, b, a);
}

inline std::ostream &operator<<(std::ostream &output, const Color &color)
{
    return output << color[0] << " " << color[1] << " " << color[2] << " " << color[3];
}

inline std::istream &operator>>(std::istream &input, Color &color)
{
    return input >> color[0] >> color[1] >> color[2] >> color[3];
}
