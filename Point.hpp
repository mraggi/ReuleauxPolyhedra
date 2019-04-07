#pragma once

#include "PointUtility.hpp"

struct Point
{
public:
    real x{0};
    real y{0};

    Point() : x(0), y(0) {}

    Point(real X, real Y) : x(X), y(Y) {}

    void Zero()
    {
        x = 0.0;
        y = 0.0;
    }

    real LengthSq() const { return x*x + y*y; }
    real Length() const { return std::sqrt(LengthSq()); }

    real Distance(const Point& vec) const { return std::sqrt(DistanceSq(vec)); }

    real DistanceSq(const Point& vec) const
    {
        real dX = (vec.x - x);
        real dY = (vec.y - y);
        return dX*dX + dY*dY;
    }

    real Angle() const { return atan2(y, x); }
    real AngleTo(const Point& vec) const
    {
        return MakeValidAngle(vec.Angle() - Angle());
    }

    bool IsCloserToFirstThanSecond(const Point& A, const Point& B) const
    {
        return DistanceSq(A) < DistanceSq(B);
    }

    Point VectorWithAngle(real t) const
    {
        real r = Length();
        return Polar(r, t);
    }

    void SetAngle(real t)
    {
        real r = Length();
        x = r*cos(t);
        y = r*sin(t);
    }

    void Rotate(real t) { Rotate(sin(t), cos(t)); }

    void Rotate(real SIN, real COS)
    {
        real xprime = COS*x - SIN*y;

        y = SIN*x + COS*y;
        x = xprime;
    }

    Point Rotated(real t)
    {
        Point a = *this;
        a.Rotate(t);
        return a;
    }

    Point Rotated(real SIN, real COS) const
    {
        Point a = *this;
        a.Rotate(SIN, COS);
        return a;
    }

    void Normalize()
    {
        if (x != 0 || y != 0)
        {
            real length = Length();

            x /= length;
            y /= length;
        }
    }

    Point Normalized() const
    {
        Point normalized = *this;

        normalized.Normalize();

        return normalized;
    }

    void Scale(real factor)
    {
        x *= factor;
        y *= factor;
    }

    void Scale(real xfactor, real yfactor)
    {
        x *= xfactor;
        y *= yfactor;
    }

    void SetLength(real r)
    {
        Normalize();
        x *= r;
        y *= r;
    }

    // only takes one sqrt instead of 2
    void SetLengthSq(real r2)
    {
        real m = std::sqrt(r2/LengthSq());
        x *= m;
        y *= m;
    }

    void SetPolar(real r, real t)
    {
        x = r*sin(t);
        y = r*cos(t);
    }

    Point WithLength(real r) const
    {
        Point copy = *this;
        copy.SetLength(r);
        return copy;
    }

    Point WithLengthSq(real r2) const
    {
        Point copy = *this;
        copy.SetLengthSq(r2);
        return copy;
    }

    Point Projection(const Point& H) const;

    Point ProjectionToLine(const Point& A, const Point& B) const;

    bool IsToTheLeftOfLine(const Point& A, const Point& B) const;

    bool IsOnLine(const Point& A, const Point& B) const;

    bool IsToTheRightOfLine(const Point& A, const Point& B) const;

    // Returns the projection if it's "positive" (wrt H) and returns 0 if
    // it's negative
    Point ForwardProjection(const Point& H) const;

    void Truncate(real r)
    {
        if (LengthSq() > r*r)
            SetLength(r);
    }

    Point Truncated(real r) const
    {
        if (LengthSq() > r*r)
            return WithLength(r);
        return *this;
    }

    // Returns a point with same length that is rotated pi/2 counter-clockwise
    inline Point Perp() const { return {-y, x}; }

    /* start Operator definitions */

    inline void operator+=(const Point& vec)
    {
        x += vec.x;
        y += vec.y;
    }
    inline void operator-=(const Point& vec)
    {
        x -= vec.x;
        y -= vec.y;
    }
    inline void operator*=(real num)
    {
        x *= num;
        y *= num;
    }
    inline void operator/=(real num)
    {
        x /= num;
        y /= num;
    }
    inline Point operator-() const { return {-x, -y}; }

    bool operator==(const Point& vec) const
    {
        return ((x == vec.x) && (y == vec.y));
    }
    bool operator!=(const Point& vec) const { return !((*this) == vec); }

    ///////////////////////////////////////////////////////////////
    ///\brief Convert from Local Coordinates to Global Coordinates
    ///
    /// Assume *this is written in the basis U, V with origin at origin.
    /// This function writes it in the canonical basis (with origin at 0).
    ///
    ///\param origin is the origin
    ///\param U is first basis
    ///\param V is second basis
    ///\return The local
    ///////////////////////////////////////////////////////////////
    Point LocalToGlobal(const Point& origin, const Point& U, const Point& V) const;

    ///////////////////////////////////////////////////////////////
    ///\brief Convert from Global Coordinates to Local Coordinates
    ///
    /// Assume *this is written in the basis e_1, e_1. This function
    /// writes it in basis U, V (with origin origin).
    ///
    ///\param origin is the origin
    ///\param U is first basis std::vector
    ///\param V is second basis std::vector
    ///\return The global std::vector
    ///////////////////////////////////////////////////////////////
    Point GlobalToLocal(const Point& origin, const Point& U, const Point& V) const;

    // Get the std::vector with length r, angle theta
    static Point Polar(real r, real theta)
    {
        return {r*cos(theta), r*sin(theta)};
    }
};

// Add and Substract
inline Point operator+(const Point& A, const Point& B)
{
    return {A.x + B.x, A.y + B.y};
}

inline Point operator-(const Point& A, const Point& B)
{
    return {A.x - B.x, A.y - B.y};
}

// Multiply and divide by scalars on both sides;
inline Point operator*(real num, const Point& vec)
{
    return {num*vec.x, num*vec.y};
}

inline Point operator/(real num, const Point& vec)
{
    return {num/vec.x, num/vec.y};
}

inline Point operator*(const Point& vec, real num)
{
    return {num*vec.x, num*vec.y};
}

inline Point operator/(const Point& vec, real num)
{
    return {vec.x/num, vec.y/num};
}

// dot product
inline real operator*(const Point& A, const Point& B)
{
    return (A.x*B.x + A.y*B.y);
}

inline real distancesq(const Point& A, const Point& B)
{
    real dx = A.x - B.x;
    real dy = A.y - B.y;
    return dx*dx + dy*dy;
}

inline real distance(const Point& A, const Point& B)
{
    return std::sqrt(distancesq(A, B));
}

inline real lengthsq(const Point& A) { return A.LengthSq(); }

inline real length(const Point& A) { return A.Length(); }

inline std::ostream& operator<<(std::ostream& os, const Point& rhs)
{
    os << "(" << rhs.x << ", " << rhs.y << ")";

    return os;
}

inline Point Sum(const std::vector<Point>& SomePoints)
{
    return std::accumulate(SomePoints.begin(), SomePoints.end(), Point(0, 0));
}

Point Point::Projection(const Point& H) const
{
    Point T(x, y);
    real t = (T*H)/(H*H);
    Point proj = t*H;
    return proj;
}

Point Point::ProjectionToLine(const Point& A, const Point& B) const
{
    Point me = *this;

    Point meMinusA = me - A;
    Point BMinusA = B - A;

    Point temp = meMinusA.Projection(BMinusA);

    return temp + A;
}

bool Point::IsToTheLeftOfLine(const Point& A, const Point& B) const
{
    Point H = B - A;
    Point P = (*this) - A;
    return H.x*P.y > H.y*P.x;
}

bool Point::IsOnLine(const Point& A, const Point& B) const
{
    Point H = B - A;
    Point P = (*this) - A;
    return (H.x*P.y == H.y*P.x);
}

bool Point::IsToTheRightOfLine(const Point& A, const Point& B) const
{
    Point H = B - A;
    Point P = (*this) - A;
    return H.x*P.y < H.y*P.x;
}

Point Point::ForwardProjection(const Point& H) const
{
    if ((*this)*H < 0.)
        return {0., 0.};
    return Projection(H);
}

Point Point::LocalToGlobal(const Point& origin, const Point& U, const Point& V) const
{
    return origin + x*U + y*V;
}

Point Point::GlobalToLocal(const Point& origin, const Point& U, const Point& V) const
{
    Point L = *this - origin;
    real determinant = (U.x*V.y - U.y*V.x);

    // check to see if they U and V are linearly dependent
    if (std::abs(determinant) != 0)
        return L.Projection(U);

    real locx = (L.x*V.y - L.y*V.x)/determinant;
    real locy = -(L.x*U.y - L.y*U.x)/determinant;
    return {locx, locy};
}
