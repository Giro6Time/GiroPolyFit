#pragma once
#include "common.h"
#include "vec.h"
#include "basic.h"
#include "basic_types.h"
class Box2d
{
    Box2d() : initialized_(false), x_min_(1e30f), y_min_(1e30f), x_max_(-1e30f), y_max_(-1e30f) {}

    bool initialized() const { return initialized_; }
    void clear() { initialized_ = false; }

    Numeric::float32 x_min() const
    {
        my_debug_assert(initialized_);
        return x_min_;
    }
    Numeric::float32 y_min() const
    {
        my_debug_assert(initialized_);
        return y_min_;
    }
    Numeric::float32 x_max() const
    {
        my_debug_assert(initialized_);
        return x_max_;
    }
    Numeric::float32 y_max() const
    {
        my_debug_assert(initialized_);
        return y_max_;
    }

    Numeric::float32 min(unsigned axis) const { return (axis == 0) ? x_min_ : y_min_; }
    Numeric::float32 max(unsigned axis) const { return (axis == 0) ? x_max_ : y_max_; }

    Numeric::float32 width() const { return x_max() - x_min(); }
    Numeric::float32 height() const { return y_max() - y_min(); }

    Numeric::float32 area() const { return width() * height(); }

    vecng<2, Numeric::float32> center() const
    {
        return vecng<2, Numeric::float32>((x_max() + x_min()) / 2, (y_max() + y_min()) / 2);
    }
    Numeric::float32 radius() const
    {
        return Numeric::float32(0.5) * ::sqrt(Numeric::sqr(x_max() - x_min()) + Numeric::sqr(y_max() - y_min()));
    }

    void add_point(const vecng<2, Numeric::float32> &p)
    {
        if (!initialized_)
        {
            x_min_ = p.x;
            y_min_ = p.y;
            x_max_ = p.x;
            y_max_ = p.y;
            initialized_ = true;
        }
        else
        {
            x_min_ = Numeric::min(x_min_, p.x);
            y_min_ = Numeric::min(y_min_, p.y);
            x_max_ = Numeric::max(x_max_, p.x);
            y_max_ = Numeric::max(y_max_, p.y);
        }
    }

    void add_box(const Box2d &b)
    {
        if (b.initialized())
        {
            add_point(vecng<2, Numeric::float32>(b.x_min(), b.y_min()));
            add_point(vecng<2, Numeric::float32>(b.x_max(), b.y_max()));
        }
    }

private:
    bool initialized_;
    Numeric::float32 x_min_;
    Numeric::float32 y_min_;
    Numeric::float32 x_max_;
    Numeric::float32 y_max_;
};

class Box3d
{
public:
    Box3d() : initialized_(false),
              x_min_(1e30f), y_min_(1e30f), z_min_(1e30f),
              x_max_(-1e30f), y_max_(-1e30f), z_max_(-1e30f)
    {
    }

    bool initialized() const { return initialized_; }
    void clear() { initialized_ = false; }

    Numeric::float32 x_min() const
    {
        my_debug_assert(initialized_);
        return x_min_;
    }
    Numeric::float32 y_min() const
    {
        my_debug_assert(initialized_);
        return y_min_;
    }
    Numeric::float32 z_min() const
    {
        my_debug_assert(initialized_);
        return z_min_;
    }
    Numeric::float32 x_max() const
    {
        my_debug_assert(initialized_);
        return x_max_;
    }
    Numeric::float32 y_max() const
    {
        my_debug_assert(initialized_);
        return y_max_;
    }
    Numeric::float32 z_max() const
    {
        my_debug_assert(initialized_);
        return z_max_;
    }

    Numeric::float32 min(unsigned axis) const { return (axis == 0) ? x_min_ : ((axis == 1) ? y_min_ : z_min_); }
    Numeric::float32 max(unsigned axis) const { return (axis == 0) ? x_max_ : ((axis == 1) ? y_max_ : z_max_); }

    Numeric::float32 width() const { return x_max() - x_min(); }
    Numeric::float32 height() const { return y_max() - y_min(); }
    Numeric::float32 depth() const { return z_max() - z_min(); }

    Numeric::float32 area() const { return Numeric::float32(2.0) * (width() * height() + height() * depth() + depth() * width()); }

    vecng<3, Numeric::float32> center() const
    {
        return vecng<3, Numeric::float32>(
            Numeric::float32(0.5) * (x_max() + x_min()),
            Numeric::float32(0.5) * (y_max() + y_min()),
            Numeric::float32(0.5) * (z_max() + z_min()));
    }

    Numeric::float32 radius() const
    {
        return Numeric::float32(0.5) * ::sqrt(
                                           Numeric::sqr(x_max() - x_min()) + Numeric::sqr(y_max() - y_min()) + Numeric::sqr(z_max() - z_min()));
    }

    void add_point(const vecng<3, Numeric::float32> &p)
    {
        if (!initialized_)
        {
            x_min_ = p.x;
            y_min_ = p.y;
            z_min_ = p.z;
            x_max_ = p.x;
            y_max_ = p.y;
            z_max_ = p.z;
            initialized_ = true;
        }
        else
        {
            x_min_ = Numeric::min(x_min_, p.x);
            y_min_ = Numeric::min(y_min_, p.y);
            z_min_ = Numeric::min(z_min_, p.z);
            x_max_ = Numeric::max(x_max_, p.x);
            y_max_ = Numeric::max(y_max_, p.y);
            z_max_ = Numeric::max(z_max_, p.z);
        }
    }

    void add_box(const Box3d &b)
    {
        add_point(vecng<3, Numeric::float32>(b.x_min(), b.y_min(), b.z_min()));
        add_point(vecng<3, Numeric::float32>(b.x_max(), b.y_max(), b.z_max()));
    }

private:
    bool initialized_;
    Numeric::float32 x_min_;
    Numeric::float32 y_min_;
    Numeric::float32 z_min_;
    Numeric::float32 x_max_;
    Numeric::float32 y_max_;
    Numeric::float32 z_max_;
};
