#pragma once
#include <iostream>
#include "basic.h"
#include "vec.h"
#include "basic_types.h"
#include "line.h"

/// @brief 表示了一个由 a*x + b*y + c*z + d = 0 决定的三维平面
class Plane
{
public:
    typedef Numeric::float32 float32;
    typedef vecng<2, float32> Point2D;
    typedef vecng<3, float32> Point3D;
    typedef vecng<3, float32> Vector3;

public:
    inline Plane(const Point3D &p1, const Point3D &p2, const Point3D &p3);
    inline Plane(const Point3D &p, const Vector3 &n);
    inline Plane(float32 a, float32 b, float32 c, float32 d)
    {
        a_ = a;
        b_ = b;
        c_ = c;
        d_ = d;
    }
    inline Plane() {}

    inline float32 a() { return a_; }
    inline float32 b() { return b_; }
    inline float32 c() { return c_; }
    inline float32 d() { return d_; }

    inline float32 &operator[](ptrdiff_t i)
    {
        my_assert(i < 4);
        return (&a_)[i];
    }
    inline const float32 &operator[](ptrdiff_t i) const
    {
        my_assert(i < 4);
        return (&a_)[i];
    }

    inline Vector3 normal() const;
    /// @brief 返回平面上的一个点
    inline Point3D point() const;
    /// @return 平面的其中一个正交基准
    inline Vector3 base1() const;
    /// @return 平面的其中一个正交基准
    inline Vector3 base2() const;
    /// @brief 将世界3D坐标投影到平面2D坐标上（以base1、base2为基准）
    inline Point2D to_2d(const Point3D &p) const;
    /// @brief 将平面2D坐标转换为世界3D坐标
    inline Point3D to_3d(const Point2D &p) const;
    /// @brief 将点坐标投射到平面上
    inline Point3D projection(const Point3D &p) const;
    /// @brief 计算a*x + b*y + c*z + d的结果
    inline float32 equation_value(const Point3D &p) const { return (a_ * p.x + b_ * p.y + c_ * p.z + d_); }
    /// @brief 计算点到平面的距离的平方
    float32 distance2(const Point3D &p) const;

    /// @brief 检查线面是否相交
    /// @param p 返回线面交点
    inline bool intersection(const Line3D &line, Point3D &p) const;
    /// @brief 检查线面是否相交
    inline bool intersection(const Line3D &line) const;

    /// @brief 检查点p在平面的正面或反面
    /// @return POSITIVE: 正面  NEGETIVE: 反面  ZERO: 平面内
    inline Sign orient(const Point3D &p) const;

private:
    float32 a_;
    float32 b_;
    float32 c_;
    float32 d_;
};

inline Plane::Plane(const Point3D &p1, const Point3D &p2, const Point3D &p3)
{
    Vector3 n = cross(p2 - p1, p3 - p1);
    normalize(n);

    a_ = n.x;
    b_ = n.y;
    c_ = n.z;
    d_ = -(a_ * p1.x + b_ * p1.y + c_ * p1.z);

#ifndef NDEBUG // 三点共线时无法确认唯一平面
    if (length(n) < 1e-15)
    {
        std::cerr << "degenerate plane constructed from 3 points:" << std::endl
                  << "\t(" << p1 << ")" << std::endl
                  << "\t(" << p2 << ")" << std::endl
                  << "\t(" << p3 << ")" << std::endl;
    }
#endif
}

inline Plane::Plane(const Point3D &p, const Vector3 &n)
{
    Vector3 nn = normalize(n);
    a_ = nn.x;
    b_ = nn.y;
    c_ = nn.z;
    d_ = -(a_ * p.x + b_ * p.y + c_ * p.z);

#ifndef NDEBUG // 当n为0时无法确认唯一平面
    if (length(nn) < 1e-15)
    {
        std::cerr << "degenerate plane constructed from point ("
                  << p << ") and normal (" << n << ")" << std::endl;
    }
#endif
}

inline std::ostream &operator<<(std::ostream &stream, const Plane &plane)
{
    return stream << plane[0] << ' ' << plane[1] << ' ' << plane[2] << ' ' << plane[3];
}

inline std::istream &operator>>(std::istream &stream, Plane &plane)
{
    return stream >> plane[0] >> plane[1] >> plane[2] >> plane[3];
}

inline Plane::Vector3 Plane::normal() const
{
    Vector3 n = normalize(Vector3(a_, b_, c_));

#ifndef NDEBUG // degenerate case
    if (length(n) < 1e-15)
    {
        std::cerr << "degenerate plane with normal: (" << n << ")" << std::endl;
    }
#endif
    return n;
}

inline Plane::Point3D Plane::point() const
{
    // 假设d为0时，原点在平面上
    if (d_ == 0)
    {
        return Point3D(0, 0, 0);
    }
    else
    {
        // 可以选择令x=0, y=0，求z = (-d)/c（假设c != 0）
        if (c_ != 0)
        {
            return Point3D(0, 0, -d_ / c_);
        }
        else if (b_ != 0)
        {
            // 如果c为0，令y=0，解x
            return Point3D(-d_ / a_, 0, 0);
        }
        else if (a_ != 0)
        {
            return Point3D(0, -d_ / b_, 0);
        }
    }
    my_assert_not_reached();
    return Point3D(0, 0, 0); // 这行不应被运行到
}

// 实现base1()函数
inline Plane::Vector3 Plane::base1() const
{
    // 找一个非零的向量，使其在平面上
    Vector3 base;
    if (c_ != 0)
    {
        base = Vector3(1, 0, -(a_ / c_));
    }
    else if (b_ != 0)
    {
        base = Vector3(1, 0, 0);
    }
    else
    {
        base = Vector3(1, -(a_ / b_), 0);
    }
    return base;
}

// 实现base2()函数
inline Plane::Vector3 Plane::base2() const
{
    // 计算与base1正交且在平面上的向量
    Vector3 b1 = base1();
    Vector3 n = normal();
    Vector3 b2 = cross(b1, n);
    return b2;
}

inline Plane::Point2D Plane::to_2d(const Point3D &p) const
{
    // 使用base1和base2作为基底，构造变换矩阵
    Vector3 b1 = base1();
    Vector3 b2 = base2();
    Vector3 v = p - point();

    float32 x = dot(v, b1);
    float32 y = dot(v, b2);

    return Point2D(x, y);
}

inline Plane::Point3D Plane::to_3d(const Point2D &p) const
{
    // 逆变换
    Vector3 b1 = base1();
    Vector3 b2 = base2();

    Vector3 v = b1 * p.x + b2 * p.y;
    return point() + v;
}

inline Plane::Point3D Plane::projection(const Point3D &p) const
{
    // 计算点到平面的投影点
    // 投影点p通过参数方程表达
    // x = p.x - A*t; yz同理，通过平面表达式可以求得t的值
    float32 t = (a_ * p.x + b_ * p.y + c_ * p.z + d_) / (a_ * a_ + b_ * b_ + c_ * c_);
    float32 x = p.x - a_ * t;
    float32 y = p.y - b_ * t;
    float32 z = p.z - c_ * t;
    return Point3D(x, y, z);
}

inline Plane::float32 Plane::distance2(const Point3D &p) const
{
    float32 v = equation_value(p);
    return v * v / (a_ * a_ + b_ * b_ + c_ * c_);
}

inline bool Plane::intersection(const Line3D &line, Point3D &p) const
{
    Vector3 dir = line.direction();

    float32 tmp = dot(dir, normal());
    if (std::abs(tmp) < 1e-15)
    {
        return false;
    }

    // 使用线面交点计算方法，求解t
    Point3D p0 = line.point();
    float32 t = -equation_value(p0) / tmp;
    p = p0 + dir * t;
    return true;
}

inline bool Plane::intersection(const Line3D &line) const
{
    Vector3 dir = line.direction();
    float32 tmp = dot(dir, normal());
    if (std::abs(tmp) < 1e-15)
    {
        return false;
    }

    return true;
}

// 实现orient函数
inline Sign Plane::orient(const Point3D &p) const
{
    double val = equation_value(p);

    if (val > 0)
    {
        return Sign::POSITIVE;
    }
    else if (val < 0)
    {
        return Sign::NEGATIVE;
    }
    else
    {
        return Sign::ZERO;
    }
}