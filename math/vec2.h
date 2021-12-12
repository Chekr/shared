#ifndef VEC2_H
#pragma once
#define VEC2_H

#include <iostream>
#include <cmath>

#include "math-helpers.h"

template<typename T>
class Vec2
{
public:
	Vec2() : x(0), y(0) {}
	Vec2(T xx) : x(xx), y(xx) {}
	Vec2(T xx, T yy) : x(xx), y(yy) {}
	void Set(T xx, T yy)
	{
		x = xx;
		y = yy;
	}
	Vec2 operator + (const Vec2 &v) const
	{
		return Vec2(x + v.x, y + v.y);
	}
	Vec2& operator += (const Vec2 &r)
	{
		x += r.x, y += r.y; return*this;
	}
	Vec2& operator -= (const Vec2 &r)
	{
		x -= r.x, y -= r.y; return*this;
	}
	Vec2 operator - (const Vec2 &v) const
	{
		return Vec2(x - v.x, y - v.y);
	}
	Vec2 operator - () const
	{
		return Vec2(-x, -y);
	}
	Vec2 operator / (const T &r) const
	{
		return IsZero(r) ? 
			Vec2(0, 0) : Vec2(x / r, y / r);
	}
	Vec2 operator * (const T &r) const
	{
		return Vec2(x* r, y* r);
	}
	Vec2 operator * (const Vec2 &v) const
	{ 
		return Vec2(x * v.x, y * v.y);
	}
	Vec2& operator /= (const T &r)
	{
		x /= r, y /= r; return*this;
	}
	Vec2& operator *= (const T &r)
	{
		x *= r, y *= r; return*this;
	}
	bool operator == (const Vec2 &v) const
	{
		return (x == v.x && y == v.y);
	}
	bool operator != (const Vec2 &v) const
	{
		return !(x == v.x && y == v.y);
	}
	friend std::ostream& operator << (std::ostream &s, const Vec2<T> &v)
	{
		return s << '[' << v.x << ' ' << v.y << ']';
	}
	friend Vec2 operator* (const T &r, const Vec2<T> &v)
	{
		return Vec2(v.x* r, v.y* r);
	}
	friend Vec2 operator / (const T &r, const Vec2 &v) 
	{
		return Vec2<T>(r / v.x, r / v.y);
	} 
	Vec2& normalize()
	{
		T n = norm();
		if (!IsZero(n) && n > 0) {
			T factor = 1 / sqrt(n);
			x *= factor, y *= factor;
		}

		return *this;
	}
	Vec2& truncate(T limit)
	{
		if (this->length() > limit)
		{
			this->normalize();

			*this *= limit;
		}

		return *this;
	}
	Vec2 perpendicular()
	{
		return (Vec2(-y, x));
	}
	T norm() const
	{
		return x* x + y* y;
	}
	T length() const
	{
		return sqrt(norm());
	}
	T dot(const Vec2<T> &v) const
	{
		return x * v.x + y * v.y;
	}
	T det(const Vec2<T> &v) const // Determinant/perp dot
	{
		return x * v.y - y * v.x;
	}
	T angle()
	{
		return std::atan2(y, x);
	}
	T angle(const Vec2<T> &target)
	{
		T val = atan2(det(target), dot(target));
		return val < 0.0f ? val = -val : val += T(g_pi);
	}
	Vec2& rotate(float theta)
	{
		T cs = std::cos(theta);
		T sn = std::sin(theta);

		T px = x * cs - y * sn;
		T py = x * sn + y * cs;

		x = px;
		y = py;

		return*this;
	}
	int sign(const Vec2<T> &v2)const
 	{
		if (y*v2.x > x*v2.y)
		{
			return -1;
		}
        if(y*v2.x < x*v2.y)
		{
			return 1;
		}
        return 0;
	}

	T x, y;
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

inline bool IsZero(Vec2f vec)
{
	return IsZero(vec.x) && IsZero(vec.y);
}




#endif
