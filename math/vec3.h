#ifndef VEC3_H
#pragma once
#define VEC3_H

#include <iostream>
#include <cmath>

#include "math-helpers.h"

template<typename T>
class Vec3
{
public:
	Vec3() : x(0), y(0), z(0) {}
	Vec3(T xx) : x(xx), y(xx), z(xx) {}
	Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
	void Set(T xx, T yy, T zz)
	{
		x = xx;
		y = yy;
		z = zz;
	}
	Vec3 operator + (const Vec3 &v) const
	{
		return Vec3(x + v.x, y + v.y, z + v.z);
	}
	Vec3& operator+= (const Vec3 &r)
	{
		x += r.x, y += r.y, z += r.z; return*this;
	}
	Vec3& operator-= (const Vec3 &r)
	{
		x -= r.x, y -= r.y, z -= r.z; return*this;
	}
	Vec3 operator - (const Vec3 &v) const
	{
		return Vec3(x - v.x, y - v.y, z -v.z);
	}
	Vec3 operator - () const
    {
        return Vec3(-x, -y, -z);
    }
	Vec3 operator/ (const T &r) const
	{
		return Vec3(x / r, y / r, z / r);
	}
	Vec3 operator* (const T &r) const
	{
		return Vec3(x* r, y* r, z* r);
	}
	Vec3& operator /= (const T &r)
	{
		x /= r, y /= r, z /= r; return*this;
	}
	Vec3& operator*= (const T &r)
	{
		x *= r, y *= r, z *= r; return*this;
	}
	bool operator== (const Vec3 &v) const
	{
		return (x == v.x && y == v.y && z == v.z);
	}
	bool operator!= (const Vec3 &v) const
	{
		return !(x == v.x && y == v.y && z == v.z);
	}
	friend std::ostream& operator << (std::ostream &s, const Vec3<T> &v)
	{
		return s << '[' << v.x << ' ' << v.y << ' ' << v.z << ']';
	}
	friend Vec3 operator* (const T &r, const Vec3<T> &v)
	{
		return Vec3(v.x* r, v.y* r, v.z* r);
	}
	Vec3& normalize()
	{
		T n = norm();
		if (n > 0) {
			T factor = 1 / sqrt(n);
			x *= factor, y *= factor, z *= factor;
		}

		return*this;
	}
	Vec3& truncate(T limit)
	{
		if (this->length() > limit)
		{
			this->normalize();

			*this *= limit;
		}

		return*this;
	}
	//Vec3 perpendicular()
	//{
	//	return (Vec3(-y, x));
	//}
	T norm() const
	{
		return x* x + y* y + z* z;
	}
	T length() const
	{
		return sqrt(norm());
	}
	T dot(const Vec3<T> &v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	Vec3 cross(const Vec3<T> &v) const
	{
		return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	//T det(const Vec3<T> &v) const // Determinant/perp dot
	//{
	//	return x * v.y - y * v.x;
	//}
	//T angle()
	//{
	//	return std::atan2(y, x);
	//}
	//T angle(const Vec3<T> &target)
	//{
	//	T val = atan2(det(target), dot(target));
	//	return val < 0.0f ? val = -val : val += T(M_PI);
	//}

	//Vec3& rotate(float hAngle, float vAngle)
	//{
	//	float theta = angle * ((float)M_PI / 180.0f);

	//	T cs = std::cos(theta);
	//	T sn = std::sin(theta);

	//	T px = x * cs - y * sn;
	//	T py = x * sn + y * cs;

	//	x = px;
	//	y = py;

	//	return*this;
	//}

	Vec3& rotate(float angle, const Vec3& axis);


	T x, y, z;
};

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

inline bool IsZero(Vec3f vec)
{
	return IsZero(vec.x) && IsZero(vec.y) && IsZero(vec.z);
}


#endif


