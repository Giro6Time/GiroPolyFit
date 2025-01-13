#include "common.h"

/// @brief n维向量模板
/// @tparam DIM 维度
/// @tparam T 数据类型
template <int DIM, class T>
class vecng
{
public:
    typedef vecng<DIM, T> thisclass;

    vecng()
    {
        for (unsigned i = 0; i < DIM; i++)
        {
            data_[i] = T(0);
        }
    }
    template <class T2>
    explicit vecng(const vecng<DIM, T2> rhs)
    {
        for (unsigned i = 0; i < DIM; i++)
        {
            data_[i] = rhs[i];
        }
    }

    template <class T2, int DIM2>
    explicit vecng(const vecng<DIM2, T2> &rhs)
    {
        my_assert(DIM2 == DIM);
        for (unsigned i = 0; i < DIM; i++)
        {
            data_[i] = T(rhs[i]);
        }
    }

    thisclass &operator=(const thisclass &rhs)
    {
        memcpy(data_, rhs.data(), DIM * sizeof(T));
        return *this;
    }

    inline T &operator[](unsigned index)
    {
        my_assert(index < DIM);
        return data()[index];
    }

    inline const T &operator[](unsigned index) const
    {
        my_assert(index < DIM);
        return data()[index];
    }

    /// @return 模的平方
    inline T length2() const
    {
        T result = T(0);
        for (unsigned i = 0; i < DIM; i++)
        {
            result += data_[i] * data_[i];
        }
        return result;
    }
    inline T length() const
    {
        return sqrt(length2());
    }

    /// @return 此向量与rhs的距离
    inline T distance2(const thisclass &rhs) const
    {
        T result = T(0);
        for (unsigned i = 0; i < DIM; i++)
        {
            T val = rhs.data_[i] - data[i];
            result += val * val;
        }
        return result;
    }

    inline T distance(const thisclass &rhs) const
    {
        return sqrt(distance2(rhs));
    }

    // operators

    thisclass &operator+(const thisclass &v)
    {
        thisclass result(*this);
        for (unsigned i = 0; i < DIM; i++)
        {
            result.data_[i] += v.data_[i];
        }
        return result;
    }
    thisclass &operator+=(const thisclass &v)
    {
        for (unsigned i = 0; i < DIM; i++)
        {
            data_[i] += v.data_[i];
        }
        return *this;
    }

    thisclass &operator-(const thisclass &v)
    {
        thisclass result(*this);
        for (unsigned i = 0; i < DIM; i++)
        {
            result.data_[i] -= v.data_[i];
        }
        return result;
    }
    thisclass &operator-=(const thisclass &v)
    {
        for (unsigned i = 0; i < DIM; i++)
        {
            data_[i] -= v.data_[i];
        }
        return *this;
    }

    thisclass &operator*(const thisclass &v)
    {
        thisclass result(*this);
        for (unsigned i = 0; i < DIM; i++)
        {
            result.data_[i] *= v.data_[i];
        }
        return result;
    }
    thisclass &operator*=(const thisclass &v)
    {
        for (unsigned i = 0; i < DIM; i++)
        {
            data_[i] *= v.data_[i];
        }
        return *this;
    }

    thisclass &operator/(const thisclass &v)
    {
        thisclass result(*this);
        for (unsigned i = 0; i < DIM; i++)
        {
            result.data_[i] /= v.data_[i];
        }
        return result;
    }
    thisclass &operator/=(const thisclass &v)
    {
        for (unsigned i = 0; i < DIM; i++)
        {
            data_[i] /= v.data_[i];
        }
        return *this;
    }
    // 对标量的运算符
    template <class T2>
    inline thisclass &operator*=(T2 s)
    {
        for (unsigned i = 0; i < DIM; i++)
        {
            data_[i] *= T(s);
        }
        return *this;
    }

    template <class T2>
    inline thisclass &operator/=(T2 s)
    {
        for (unsigned i = 0; i < DIM; i++)
        {
            data_[i] /= T(s);
        }
        return *this;
    }

    template <class T2>
    inline thisclass operator*(T2 s) const
    {
        thisclass result(*this);
        for (unsigned i = 0; i < DIM; i++)
        {
            result.data_[i] *= T(s);
        }
        return result;
    }

    template <class T2>
    inline thisclass operator/(T2 s) const
    {
        thisclass result(*this);
        for (unsigned i = 0; i < DIM; i++)
        {
            result.data_[i] /= T(s);
        }
        return result;
    }

    inline thisclass operator-() const
    {
        thisclass result;
        for (unsigned i = 0; i < DIM; i++)
        {
            result.data_[i] = -data_[i];
        }
        return result;
    }

    inline thisclass dot(thisclass &v2)
    {
        T result = 0;
        for (unsigned i = 0; i < DIM; i++)
        {
            result += data_[i] * v2[i];
        }
        return result;
    }

    unsigned dimension() const { return (unsigned)DIM; }

    T *data() { return data_; }

    const T *data() const { return data_; }

protected:
    T data_[];
};

// 适配GLSL
template <int DIM, class T>
inline T length(const vecng<DIM, T> &v) { return v.length(); }
template <int DIM, class T>
inline T length2(const vecng<DIM, T> &v) { return v.length2(); }
template <int DIM, class T>
inline T distance(const vecng<DIM, T> &v) { return v.distance(); }
template <int DIM, class T>
inline T distance2(const vecng<DIM, T> &v) { return v.distance2(); }
template <int DIM, class T>
inline vecng<DIM, T> normalize(const vecng<DIM, T> &v)
{
    T s = length(v);
    if (s > 1e-30) // 防止除零
    {
        s = T(1) / s;
    }
    return s * v;
}
template <int DIM, class T>
inline vecng<DIM, T> mix(const vecng<DIM, T> &v1, const vecng<DIM, T> &v2, T s)
{
    return (T(1) - s) * v1 + s * v2;
}

/*-------------------------------------------vec2-------------------------------------------*/

template <class T>
class vecng<2, T>
{
public:
    typedef vecng<2, T> thisclass;

    vecng() : x(0), y(0) {}
    vecng(T x_in, T y_in) : x(x_in), y(y_in) {}
    template <class T2>
    explicit vecng(const vecng<2, T2> &v)
        : x(v.x), y(v.y) {}

    template <class T2>
    explicit vecng(const T2 *v)
        : x(v[0]), y(v[1]) {}

    inline T length2() const { return x * x + y * y; }
    inline T length() const { return sqrt(x * x + y * y); }
    inline T distance2(const thisclass &rhs) const
    {
        T dx = rhs.x - x;
        T dy = rhs.y - y;
        return dx * dx + dy * dy;
    }

    // operators
    inline thisclass &operator+=(const thisclass &v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }
    inline thisclass &operator-=(const thisclass &v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    inline thisclass &operator*=(const thisclass &v)
    {
        x *= v.x;
        y *= v.y;
        return *this;
    }
    inline thisclass &operator/=(const thisclass &v)
    {
        x /= v.x;
        y /= v.y;
        return *this;
    }
    template <class T2>
    inline thisclass &operator*=(T2 s)
    {
        x *= T(s);
        y *= T(s);
        return *this;
    }
    template <class T2>
    inline thisclass &operator/=(T2 s)
    {
        x /= T(s);
        y /= T(s);
        return *this;
    }

    inline thisclass operator+(const thisclass &v) const { return thisclass(x + v.x, y + v.y); }
    inline thisclass operator-(const thisclass &v) const { return thisclass(x - v.x, y - v.y); }
    template <class T2>
    inline thisclass operator*(T2 s) const { return thisclass(x * T(s), y * T(s)); }
    template <class T2>
    inline thisclass operator/(T2 s) const { return thisclass(x / T(s), y / T(s)); }
    inline thisclass operator-() const { return thisclass(-x, -y); }

    unsigned dimension() const { return (unsigned)2; }

    // x,y连续存储
    T *data() { return &x; }
    const T *data() const { return &x; }

    inline T &operator[](unsigned idx)
    {
        my_assert(idx < 2);
        return data()[idx];
    }

    inline const T &operator[](unsigned idx) const
    {
        my_assert(idx < 2);
        return data()[idx];
    }

    T x;
    T y;
};

template <class T>
inline T dot(const vecng<2, T> &v1, const vecng<2, T> &v2) { return v1.x * v2.x + v1.y * v2.y; }

template <class T>
inline T det(const vecng<2, T> &v1, const vecng<2, T> &v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

/*-------------------------------------------vec3-------------------------------------------*/

template <class T>
class vecng<3, T>
{
public:
    typedef vecng<3, T> thisclass;

    vecng() : x(0), y(0), z(0) {}
    vecng(T x_in, T y_in, T z_in) : x(x_in), y(y_in), z(z_in) {}
    template <class T2>
    explicit vecng(const vecng<3, T2> &v) : x(v.x), y(v.y), z(v.z) {}
    template <class T2>
    explicit vecng(const T2 *v)
        : x(v[0]), y(v[1]), z(v[2]) {}

    inline T length2() const { return x * x + y * y + z * z; }
    inline T length() const { return sqrt(x * x + y * y + z * z); }
    inline T distance2(const thisclass &rhs) const
    {
        T dx = rhs.x - x;
        T dy = rhs.y - y;
        T dz = rhs.z - z;
        return dx * dx + dy * dy + dz * dz;
    }

    // operators
    inline thisclass &operator+=(const thisclass &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    inline thisclass &operator-=(const thisclass &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    inline thisclass &operator*=(const thisclass &v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    inline thisclass &operator/=(const thisclass &v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }
    template <class T2>
    inline thisclass &operator*=(T2 s)
    {
        x *= T(s);
        y *= T(s);
        z *= T(s);
        return *this;
    }
    template <class T2>
    inline thisclass &operator/=(T2 s)
    {
        x /= T(s);
        y /= T(s);
        z /= T(s);
        return *this;
    }

    inline thisclass operator+(const thisclass &v) const { return thisclass(x + v.x, y + v.y, z + v.z); }
    inline thisclass operator-(const thisclass &v) const { return thisclass(x - v.x, y - v.y, z - v.z); }
    template <class T2>
    inline thisclass operator*(T2 s) const { return thisclass(x * T(s), y * T(s), z * T(s)); }
    template <class T2>
    inline thisclass operator/(T2 s) const { return thisclass(x / T(s), y / T(s), z / T(s)); }

    inline thisclass operator-() const { return thisclass(-x, -y, -z); }
    unsigned dimension() const { return (unsigned)3; }

    // xyz连续存储
    T *data() { return &x; }
    const T *data() const { return &x; }

    inline T &operator[](unsigned idx)
    {
        my_assert(idx < 3);
        return data()[idx];
    }

    inline const T &operator[](unsigned idx) const
    {
        my_assert(idx < 3);
        return data()[idx];
    }

    T x;
    T y;
    T z;
};

template <class T>
inline T dot(const vecng<3, T> &v1, const vecng<3, T> &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <class T>
inline vecng<3, T> cross(const vecng<3, T> &v1, const vecng<3, T> &v2)
{
    return vecng<3, T>(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x);
}

/*-------------------------------------------vec4-------------------------------------------*/

template <class T>
class vecng<4, T>
{
public:
    typedef vecng<4, T> thisclass;

    vecng() : x(0), y(0), z(0), w(0) {}
    vecng(T x_in, T y_in, T z_in, T w_in) : x(x_in), y(y_in), z(z_in), w(w_in) {}
    template <class T2>
    explicit vecng(const vecng<4, T2> &v)
        : x(v.x), y(v.y), z(v.z), w(v.w) {}
    template <class T2>
    explicit vecng(const T2 *v)
        : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

    inline T length2() const { return x * x + y * y + z * z + w * w; }
    inline T length() const { return sqrt(x * x + y * y + z * z + w * w); }
    inline T distance2(const thisclass &rhs) const
    {
        T dx = rhs.x - x;
        T dy = rhs.y - y;
        T dz = rhs.z - z;
        T dw = rhs.w - w;
        return dx * dx + dy * dy + dz * dz + dw * dw;
    }

    unsigned dimension() const { return (unsigned)4; }

    // operators
    inline thisclass &operator+=(const thisclass &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }
    inline thisclass &operator-=(const thisclass &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }
    inline thisclass &operator*=(const thisclass &v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        w *= v.w;
        return *this;
    }
    inline thisclass &operator/=(const thisclass &v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        w /= v.w;
        return *this;
    }
    template <class T2>
    inline thisclass &operator*=(T2 s)
    {
        x *= T(s);
        y *= T(s);
        z *= T(s);
        w *= T(s);
        return *this;
    }
    template <class T2>
    inline thisclass &operator/=(T2 s)
    {
        x /= T(s);
        y /= T(s);
        z /= T(s);
        w /= T(s);
        return *this;
    }
    inline thisclass operator+(const thisclass &v) const { return thisclass(x + v.x, y + v.y, z + v.z, w + v.w); }
    inline thisclass operator-(const thisclass &v) const { return thisclass(x - v.x, y - v.y, z - v.z, w - v.w); }
    template <class T2>
    inline thisclass operator*(T2 s) const { return thisclass(x * T(s), y * T(s), z * T(s), w * T(s)); }
    template <class T2>
    inline thisclass operator/(T2 s) const { return thisclass(x / T(s), y / T(s), z / T(s), w / T(s)); }
    inline thisclass operator-() const { return thisclass(-x, -y, -z, -w); }

    T *data() { return &x; }
    const T *data() const { return &x; }

    inline T &operator[](unsigned idx)
    {
        my_assert(idx < 4);
        return data()[idx];
    }

    inline const T &operator[](unsigned idx) const
    {
        my_assert(idx < 4);
        return data()[idx];
    }

    T x;
    T y;
    T z;
    T w;
};

template <class T>
inline T dot(const vecng<4, T> &v1, const vecng<4, T> &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

template <class T>
inline vecng<4, T> operator-(const vecng<4, T> &v1) { return vecng<4, T>(-v1.x, -v1.y, -v1.z, -v1.w); }
template <class T2, class T>
inline vecng<4, T> operator*(T2 s, const vecng<4, T> &v)
{
    return vecng<4, T>(T(s) * v.x, T(s) * v.y, T(s) * v.z, T(s) * v.w);
}

template <class T>
inline vecng<4, T> operator+(const vecng<4, T> &v1, const vecng<4, T> &v2)
{
    return vecng<4, T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

template <class T>
inline vecng<4, T> operator-(const vecng<4, T> &v1, const vecng<4, T> &v2)
{
    return vecng<4, T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

template <int DIM, class T>
inline std::ostream &operator<<(std::ostream &out, const ::vecng<DIM, T> &v)
{
    for (unsigned i = 0; i < DIM; i++)
    {
        out << v[i] << " ";
    }
    return out;
}

template <int DIM, class T>
inline std::istream &operator>>(std::istream &in, ::vecng<DIM, T> &v)
{
    for (unsigned i = 0; i < DIM; i++)
    {
        in >> v[i];
    }
    return in;
}

template <class T>
inline std::ostream &operator<<(std::ostream &out, const ::vecng<2, T> &v)
{
    return out << v.x << " " << v.y;
}

template <class T>
inline std::istream &operator>>(std::istream &in, ::vecng<2, T> &v)
{
    return in >> v.x >> v.y;
}

template <class T>
inline std::ostream &operator<<(std::ostream &out, const ::vecng<3, T> &v)
{
    return out << v.x << " " << v.y << " " << v.z;
}

template <class T>
inline std::istream &operator>>(std::istream &in, ::vecng<3, T> &v)
{
    return in >> v.x >> v.y >> v.z;
}

template <class T>
inline std::ostream &operator<<(std::ostream &out, const ::vecng<4, T> &v)
{
    return out << v.x << " " << v.y << " " << v.z << " " << v.w;
}

template <class T>
inline std::istream &operator>>(std::istream &in, ::vecng<4, T> &v)
{
    return in >> v.x >> v.y >> v.z >> v.w;
}
