#pragma once

#include "math_types.h"
#include "vec.h"

template <int DIM, class FT>
class GenericLine
{
public:
    typedef vecng<DIM, FT> Point;
    typedef vecng<DIM, FT> Vector;

public:
    static GenericLine from_point_and_direction(const Point &p, const Vector &dir) { return GenericLine(p, dir); }
    static GenericLine from_two_points(const Point &p, const Point &q) { return GenericLine(p, q - p); }

    GenericLine() {}
    void set_point(const Point &p) { p_ = p; }
    void set_dir(const Vector &dir) { dir_ = normalize(dir); }

    const Vector &direction() const { return dir_; }

    const Point &point() const { return p_; }

    GenericLine opposite() const { return GenericLine(p_, -dir_); }

    /// @brief 点p在这条线上的投影
    Point projection(const Point &p) const { return p_ + dir_ * dot(p - p_, dir_); }

    FT squared_ditance(const Point &p) const { length2(projection(p) - p); }

private:
    template <int DIM, class FT>
    inline GenericLine(const Point &p, const Vector &dir) : p_(p)
    {
        dir_ = normalize(dir);

#ifndef NDEBUG // degenerate case
        if (length(dir_) < 1e-15)
        {
            std::cerr << "degenerate line constructed from point ("
                      << p << ") and direction (" << dir << ")" << std::endl;
        }
#endif
    }

    Point p_;
    Vector dir_;
};

/// @brief 2D射线
typedef GenericLine<2, Numeric::float32> Line2d;

/// @brief 3D射线
typedef GenericLine<3, Numeric::float32> Line3d;
