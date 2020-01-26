#ifndef QUATERNION_H
#pragma once
#define QUATERNION_H

#include <math.h>

#include "vec3.h"


struct Quaternion
{
	float x, y, z, w;

	Quaternion(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	void Normalize()
	{
		float Length = sqrtf(x * x + y * y + z * z + w * w);

		x /= Length;
		y /= Length;
		z /= Length;
		w /= Length;
	}

	Quaternion Conjugate()
	{
		Quaternion ret(-x, -y, -z, w);
		return ret;
	}
};

Quaternion operator*(const Quaternion& l, const Quaternion& r);
Quaternion operator*(const Quaternion& q, const Vec3f& v);

#endif
