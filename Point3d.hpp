#pragma once

#include "PointUtility.hpp"

struct Point3d
{
public:
    real x{0.};
    real y{0.};
    real z{0.};

    Point3d() {}

    Point3d(real X, real Y, real Z) : x(X), y(Y), z(Z) {}

    real LengthSq() const { return x*x + y*y + z*z; }
    real Length() const { return std::sqrt(LengthSq()); }

    real Distance(const Point3d& vec) const { return std::sqrt(DistanceSq(vec)); }

    real DistanceSq(const Point3d& vec) const
    {
        real dX = (vec.x - x);
        real dY = (vec.y - y);
        real dZ = (vec.z - z);
        return dX*dX + dY*dY + dZ*dZ;
    }

    bool IsCloserToFirstThanSecond(const Point3d& A, const Point3d& B) const
    {
        return DistanceSq(A) < DistanceSq(B);
    }

    void Normalize()
    {
        if (x != 0 || y != 0 || z != 0)
        {
            real length = Length();

            x /= length;
            y /= length;
            z /= length;
        }
    }

    Point3d Normalized() const
    {
        Point3d normalized = *this;

        normalized.Normalize();

        return normalized;
    }

    void Scale(real factor)
    {
        x *= factor;
        y *= factor;
        z *= factor;
    }

    void Scale(real xfactor, real yfactor, real zfactor)
    {
        x *= xfactor;
        y *= yfactor;
        z *= zfactor;
    }

    void SetLength(real r)
    {
        Normalize();
        x *= r;
        y *= r;
        z *= r;
    }

    // only takes one sqrt
    void SetLengthSq(real r2)
    {
        real m = std::sqrt(r2/LengthSq());
        x *= m;
        y *= m;
        z *= m;
    }

    Point3d WithLength(real r) const
    {
        Point3d copy = *this;
        copy.SetLength(r);
        return copy;
    }

    Point3d WithLengthSq(real r2) const
    {
        Point3d copy = *this;
        copy.SetLengthSq(r2);
        return copy;
    }


    /* start Operator definitions */

    inline void operator+=(const Point3d& vec)
    {
        x += vec.x;
        y += vec.y;
        z += vec.z;
    }
    inline void operator-=(const Point3d& vec)
    {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;
    }
    inline void operator*=(real num)
    {
        x *= num;
        y *= num;
        z *= num;
    }
    inline void operator/=(real num)
    {
        x /= num;
        y /= num;
        z /= num;
    }
    inline Point3d operator-() const { return {-x, -y, -z}; }

    bool operator==(const Point3d& vec) const
    {
        return ((x == vec.x) && (y == vec.y) && (z == vec.z));
    }
    bool operator!=(const Point3d& vec) const { return !((*this) == vec); }
};

// Add and Substract
inline Point3d operator+(const Point3d& A, const Point3d& B)
{
    return {A.x + B.x, A.y + B.y, A.z + B.z};
}

inline Point3d operator-(const Point3d& A, const Point3d& B)
{
    return {A.x - B.x, A.y - B.y, A.z - B.z};
}

// Multiply and divide by scalars on both sides;
inline Point3d operator*(real num, const Point3d& vec)
{
    return {num*vec.x, num*vec.y, num*vec.z};
}

inline Point3d operator/(real num, const Point3d& vec)
{
    return {num/vec.x, num/vec.y, num/vec.z};
}

inline Point3d operator*(const Point3d& vec, real num)
{
    return {num*vec.x, num*vec.y, num*vec.z};
}

inline Point3d operator/(const Point3d& vec, real num)
{
    return {vec.x/num, vec.y/num, vec.z/num};
}

// dot product
inline real operator*(const Point3d& A, const Point3d& B)
{
    return (A.x*B.x + A.y*B.y + A.z*B.z);
}

inline real distancesq(const Point3d& A, const Point3d& B)
{
    real dx = A.x - B.x;
    real dy = A.y - B.y;
    real dz = A.z - B.z;
    return dx*dx + dy*dy + dz*dz;
}

inline real distance(const Point3d& A, const Point3d& B)
{
    return std::sqrt(distancesq(A, B));
}

inline real lengthsq(const Point3d& A) { return A.LengthSq(); }

inline real length(const Point3d& A) { return A.Length(); }

inline std::ostream& operator<<(std::ostream& os, const Point3d& P)
{
    os << "(" << P.x << ", " << P.y << ", " << P.z <<  ")";

    return os;
}

inline Point3d Sum(const std::vector<Point3d>& S)
{
    return std::accumulate(S.begin(), S.end(), Point3d(0, 0, 0));
}
