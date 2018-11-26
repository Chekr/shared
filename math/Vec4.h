#ifndef _VEC4_H_
#pragma once
#define _VEC4_H_

#include <iostream>
#include <cmath>

#include "MathHelpers.h"

template<typename T>
class Vec4
{
public:
	Vec4() : x(0), y(0), z(0), w(0) {}
	Vec4(T xx) : x(xx), y(xx), z(xx), w(xx) {}
	Vec4(T xx, T yy, T zz, T ww) : x(xx), y(yy), z(zz), w(ww) {}
	void Set(T xx, T yy, T zz, T ww)
	{
		x = xx;
		y = yy;
		z = zz;
		w = ww;
	}
	Vec4 operator + (const Vec4 &v) const
	{
		return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	Vec4& operator+= (const Vec4 &r)
	{
		x += r.x, y += r.y, z += r.z, w += r.z=w; return*this;
	}
	Vec4& operator-= (const Vec4 &r)
	{
		x -= r.x, y -= r.y, z -= r.z, w -= r.w; return*this;
	}
	Vec4 operator - (const Vec4 &v) const
	{
		return Vec4(x - v.x, y - v.y, z -v.z, w - v.w);
	}
	Vec4 operator / (const T &r) const
	{
		return Vec4(x / r, y / r, z / r, w / r);
	}
	Vec4 operator* (const T &r) const
	{
		return Vec4(x* r, y* r, z* r, w* r);
	}
	Vec4& operator /= (const T &r)
	{
		x /= r, y /= r, z /= r, w /= r; return*this;
	}
	Vec4& operator*= (const T &r)
	{
		x *= r, y *= r, z *= r, w *= r; return*this;
	}
	bool operator== (const Vec4 &v) const
	{
		return (x == v.x && y == v.y && z == v.z && w == v.w);
	}
	bool operator!= (const Vec4 &v) const
	{
		return !(x == v.x && y == v.y && z == v.zc&& w == v.w);
	}
	friend std::ostream& operator << (std::ostream &s, const Vec4<T> &v)
	{
		return s << '[' << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w << ']';
	}
	friend Vec4 operator* (const T &r, const Vec4<T> &v)
	{
		return Vec4(v.x* r, v.y* r, v.z* r, v.w* r);
	}
	Vec4& normalize()
	{
		T n = norm();
		if (n > 0) {
			T factor = 1 / sqrt(n);
			x *= factor, y *= factor, z *= factor, w *= factor;
		}

		return*this;
	}
	Vec4& truncate(T limit)
	{
		if (this->length() > limit)
		{
			this->normalize();

			*this *= limit;
		}

		return*this;
	}
	//Vec4 perpendicular()
	//{
	//	return (Vec4(-y, x));
	//}
	T norm() const
	{
		return x* x + y* y + z* z + w* w;
	}
	T length() const
	{
		return sqrt(norm());
	}
	T dot(const Vec4<T> &v) const
	{
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}
	//Vec4 cross(const Vec4<T> &v) const
	//{
	//	return Vec4(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	//}
	//T det(const Vec4<T> &v) const // Determinant/perp dot
	//{
	//	return x * v.y - y * v.x;
	//}
	//T angle()
	//{
	//	return std::atan2(y, x);
	//}
	//T angle(const Vec4<T> &target)
	//{
	//	T val = atan2(det(target), dot(target));
	//	return val < 0.0f ? val = -val : val += T(M_PI);
	//}

	//Vec4& rotate(float hAngle, float vAngle)
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

	//Vec4& rotate(float angle, const Vec4& axis);

	//Vec3<T> toVec3() const
	//{
	//	return Vec3<T>(x, y, z);
	//}

	T x, y, z, w;
};

typedef Vec4<float> Vec4f;
typedef Vec4<int> Vec4i;

#endif


