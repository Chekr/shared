#ifndef QUATERNION_H
#pragma once
#define QUATERNION_H

#include <math.h>

#include "vec3.h"
#include "matrix3x3.h"
#include "matrix4x4.h"

struct Quaternion
{
	float x, y, z, w;

	Quaternion() {}
	Quaternion(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	Quaternion(const Quaternion& q)
	{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
	}


	bool operator==(const Quaternion &q) const
	{
		return IsZero(w - q.w) 
			&& IsZero(x - q.x)
        	&& IsZero(y - q.y) 
			&& IsZero(z - q.z);
	}
    bool operator!=(const Quaternion &q) const
	{
		return !(*this == q);
	}

	Quaternion &operator+=(const Quaternion &q)
	{
		w += q.w, x += q.x, y += q.y, z += q.z; return *this;
	}
	Quaternion &operator-=(const Quaternion &q)
	{
		w -= q.w, x -= q.x, y -= q.y, z -= q.z; return *this;
	}
	Quaternion &operator*=(const Quaternion &q)
	{
		Quaternion temp( 
			(w * q.w) - (x * q.x) - (y * q.y) - (z * q.z),
			(w * q.x) + (x * q.w) - (y * q.z) + (z * q.y),
			(w * q.y) + (x * q.z) + (y * q.w) - (z * q.x),
			(w * q.z) - (x * q.y) + (y * q.x) + (z * q.w));

		*this = temp;
		return *this;
	}
	Quaternion &operator*=(float scalar)
	{
		w *= scalar, x *= scalar, y *= scalar, z *= scalar; return *this;
	}
	Quaternion &operator/=(float scalar)
	{
		w /= scalar, x /= scalar, y /= scalar, z /= scalar; return *this;
	}

	Quaternion operator+(const Quaternion &q) const
	{
		Quaternion tmp(*this);
		tmp += q;
		return tmp;
	}
	Quaternion operator-(const Quaternion &q) const
	{
		Quaternion tmp(*this);
		tmp -= q;
		return tmp;
	}
	Quaternion operator*(const Quaternion &q) const
	{
		Quaternion tmp(*this);
		tmp *= q;
		return tmp;
	}
	Quaternion operator*(float scalar) const
	{
		Quaternion tmp(*this);
		tmp *= scalar;
		return tmp;
	}

	Quaternion operator/(float scalar) const
	{
		Quaternion tmp(*this);
		tmp /= scalar;
		return tmp;
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
		return Quaternion(-x, -y, -z, w);
	}

	void Identity()
	{
		x = 0, y = 0, z = 0, w = 1;
	}


	void FromAxisAngle(const Vec3f &axis, float radians)
	{
		float halfTheta = radians * 0.5f;
		float s = sinf(halfTheta);
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
		w = cosf(halfTheta);
	}

	void Rotation(float yaw, float pitch, float roll)
	{
		Matrix3x3f m;
		m.Rotation(yaw, pitch, roll);
		FromMatrix(m);
	}

	void FromMatrix(const Matrix3x3f &m)
	{
		// "Advanced Animation and Rendering Techniques"
		float s = 0.0f;
		float q[4] = {0.0f};
		float trace = m[0][0] + m[1][1] + m[2][2];

		if (trace > 0.0f)
		{
			s = sqrtf(trace + 1.0f);
			q[3] = s * 0.5f;
			s = 0.5f / s;
			q[0] = (m[1][2] - m[2][1]) * s;
			q[1] = (m[2][0] - m[0][2]) * s;
			q[2] = (m[0][1] - m[1][0]) * s;
		}
		else
		{
			int nxt[3] = {1, 2, 0};
			int i = 0, j = 0, k = 0;

			if (m[1][1] > m[0][0])
				i = 1;

			if (m[2][2] > m[i][i])
				i = 2;

			j = nxt[i];
			k = nxt[j];
			s = sqrtf((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);

			q[i] = s * 0.5f;
			s = 0.5f / s;
			q[3] = (m[j][k] - m[k][j]) * s;
			q[j] = (m[i][j] + m[j][i]) * s;
			q[k] = (m[i][k] + m[k][i]) * s;
		}

		x = q[0], y = q[1], z = q[2], w = q[3];
	}

	void FromMatrix(const Matrix4x4f &m)
	{
		float s = 0.0f;
		float q[4] = {0.0f};
		float trace = m[0][0] + m[1][1] + m[2][2];

		if (trace > 0.0f)
		{
			s = sqrtf(trace + 1.0f);
			q[3] = s * 0.5f;
			s = 0.5f / s;
			q[0] = (m[1][2] - m[2][1]) * s;
			q[1] = (m[2][0] - m[0][2]) * s;
			q[2] = (m[0][1] - m[1][0]) * s;
		}
		else
		{
			int nxt[3] = {1, 2, 0};
			int i = 0, j = 0, k = 0;

			if (m[1][1] > m[0][0])
				i = 1;

			if (m[2][2] > m[i][i])
				i = 2;

			j = nxt[i];
			k = nxt[j];
			s = sqrtf((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);

			q[i] = s * 0.5f;
			s = 0.5f / s;
			q[3] = (m[j][k] - m[k][j]) * s;
			q[j] = (m[i][j] + m[j][i]) * s;
			q[k] = (m[i][k] + m[k][i]) * s;
		}

		x = q[0], y = q[1], z = q[2], w = q[3];
	}

	Matrix3x3f ToMatrix3x3() const
	{
		float x2 = x + x;
		float y2 = y + y;
		float z2 = z + z;
		float xx = x * x2;
		float xy = x * y2;
		float xz = x * z2;
		float yy = y * y2;
		float yz = y * z2;
		float zz = z * z2;
		float wx = w * x2;
		float wy = w * y2;
		float wz = w * z2;

		Matrix3x3f m;

		m[0][0] = 1.0f - (yy + zz);
		m[0][1] = xy + wz;
		m[0][2] = xz - wy;

		m[1][0] = xy - wz;
		m[1][1] = 1.0f - (xx + zz);
		m[1][2] = yz + wx;

		m[2][0] = xz + wy;
		m[2][1] = yz - wx;
		m[2][2] = 1.0f - (xx + yy);

		return m;
	}

	Matrix4x4f ToMatrix4x4() const
	{
		float x2 = x + x;
		float y2 = y + y;
		float z2 = z + z;
		float xx = x * x2;
		float xy = x * y2;
		float xz = x * z2;
		float yy = y * y2;
		float yz = y * z2;
		float zz = z * z2;
		float wx = w * x2;
		float wy = w * y2;
		float wz = w * z2;

		Matrix4x4f m;

		m[0][0] = 1.0f - (yy + zz);
		m[0][1] = xy + wz;
		m[0][2] = xz - wy;
		m[0][3] = 0.0f;

		m[1][0] = xy - wz;
		m[1][1] = 1.0f - (xx + zz);
		m[1][2] = yz + wx;
		m[1][3] = 0.0f;

		m[2][0] = xz + wy;
		m[2][1] = yz - wx;
		m[2][2] = 1.0f - (xx + yy);
		m[2][3] = 0.0f;

		m[3][0] = 0.0f;
		m[3][1] = 0.0f;
		m[3][2] = 0.0f;
		m[3][3] = 1.0f;

		return m;
	}




};

//Quaternion operator*(const Quaternion& l, const Quaternion& r);
Quaternion operator*(const Quaternion& q, const Vec3f& v);

#endif
