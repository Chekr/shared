#ifndef MATRIX_4_X_4_H
#pragma once
#define MATRIX_4_X_4_H

#include <cstdlib> 
#include <cstdio> 

#include <iomanip>
#include <iostream>
#include <cmath>
#include <cstdint>

#include <vector>

#include "math-helpers.h"
#include "vec3.h"

template<typename T>
class Matrix4x4
{
public:
	T matrix[4][4] = 
	{
		{zero,zero,zero,zero},
		{zero,zero,zero,zero},
		{zero,zero,zero,zero},
		{zero,zero,zero,zero}
	};
	T zero = static_cast<T>(0);
	T one = static_cast<T>(1);

	Matrix4x4() {}

	const T* operator[] (uint8_t index) const { return matrix[index];  }
	T* operator[] (uint8_t index) { return matrix[index];  }

	Matrix4x4 operator * (const Matrix4x4& mat) const
	{
		Matrix4x4 temp;
		Multiply(*this, mat, temp);
		return temp;
	}

	Matrix4x4& operator*= (const Matrix4x4& mat)
	{
		return *this = *this * mat;
	}

	static void Multiply(const Matrix4x4<T> &a, const Matrix4x4& b, Matrix4x4 &c)
	{
		// This would be faster in longform
		for (uint8_t i = 0; i < 4; ++i)
		{ 
			for (uint8_t j = 0; j < 4; ++j)
			{ 
				c[i][j] = a[i][0] * b[0][j] + 
					a[i][1] * b[1][j] + 
					a[i][2] * b[2][j]; 
			} 
		} 
	};

	void TransformVecs(std::vector<Vec3<T>> &points)
	{	
		for(Vec3<T>& point : points)
		{
			TransformVec(point);
		}
	}

	void TransformVec(Vec3<T> &point)
	{
		float tempX =(matrix[0][0]*point.x) + (matrix[1][0]*point.y) + (matrix[2][0]*point.z) + (matrix[3][0]);
		float tempY = (matrix[0][1]*point.x) + (matrix[1][1]*point.y) + (matrix[2][1]*point.z) + (matrix[3][1]);
  		float tempZ = (matrix[0][2]*point.x) + (matrix[1][2]*point.y) + (matrix[2][2]*point.z) + (matrix[3][2]);

		point.x = tempX;
		point.y = tempY;
		point.z = tempZ;
	}

	void Identity()
	{
		T * __restrict t = &matrix[0][0];
		t[0] = one; t[1] = zero; t[2] = zero; t[3] = zero;
		t[4] = zero; t[5] = one; t[6] = zero; t[7] = zero;
		t[8] = zero; t[9] = zero; t[10] = one; t[11] = zero;
		t[12] = zero; t[13] = zero; t[14] = zero; t[15] = one;
	}

	void Rotation(T yaw, T pitch, T roll)
    {            
        float cosH = cosf(yaw);   
        float cosP = cosf(pitch);
        float cosR = cosf(roll);
        float sinH = sinf(yaw);
        float sinP = sinf(pitch);
        float sinR = sinf(roll);

        matrix[0][0] = cosR * cosH - sinR * sinP * sinH;
        matrix[0][1] = sinR * cosH + cosR * sinP * sinH;
        matrix[0][2] = -cosP * sinH;
		matrix[0][3] = 0.0f;
 
        matrix[1][0] = -sinR * cosP;
        matrix[1][1] = cosR * cosP;
        matrix[1][2] = sinP;
		matrix[1][3] = 0.0f;

        matrix[2][0] = cosR * sinH + sinR * sinP * cosH;
        matrix[2][1] = sinR * sinH - cosR * sinP * cosH;
        matrix[2][2] = cosP * cosH;
		matrix[2][3] = 0.0f;
		
		matrix[3][0] = 0.0f;
		matrix[3][1] = 0.0f;
		matrix[3][2] = 0.0f;
		matrix[3][3] = 0.0f;
    }

	void Translate(T x, T y, T z)
	{
		Matrix4x4 temp;

		T * __restrict t = &temp.matrix[0][0];

		t[0] = one; t[1] = zero; t[2] = zero; t[3] = zero;
		t[4] = zero; t[5] = one; t[6] = zero; t[7] = zero;
		t[8] = zero; t[9] = zero; t[10] = one; t[11] = zero;
		t[12] = x; t[13] = y; t[14] = z; t[15] = one;

		*this *= temp;
	}

	void Scale(T xScale, T yScale, T zScale)
	{
		Matrix4x4 temp;

		T * __restrict t = &temp.matrix[0][0];

		t[0] = xScale; t[1] = zero; t[2] = zero; t[3] = zero;
		t[4] = zero; t[5] = yScale; t[6] = zero; t[7] = zero;
		t[8] = zero; t[9] = zero; t[10] = zScale; t[11] = zero;
		t[12] = zero; t[13] = zero; t[14] = zero; t[15] = one;

		*this *= temp;
	}



	void Rotate(T rot)
	{
		Matrix4x4 temp;

		float s = sin(rot);
		float c = cos(rot);

		T * __restrict t = &temp.matrix[0][0];

		t[0] =  one; t[1] = zero; t[2] = zero; t[3] = zero;
		t[4] = zero; t[5] = one; t[6] = zero; t[7] = zero;
		t[8] =  zero; t[9] =  zero; t[10] = one; t[11] = zero;
		t[12] = zero; t[13] = zero; t[14] = zero; t[15] = one;

		*this *= temp;
	}

	void RotateX(T rot)
	{
		Matrix4x4 temp;
		
		float s = sin(rot);
		float c = cos(rot);

		T * __restrict t = &temp.matrix[0][0];

		t[0] =  one; t[1] = zero; t[2] = zero; t[3] = zero;
		t[4] = zero; t[5] = c; t[6] = s; t[7] = zero;
		t[8] =  zero; t[9] =  -s; t[10] = c; t[11] = zero;
		t[12] = zero; t[13] = zero; t[14] = zero; t[15] = one;

		*this *= temp;
	}

	void RotateY(T rot)
	{
		Matrix4x4 temp;
		
		float s = sin(rot);
		float c = cos(rot);

		T * __restrict t = &temp.matrix[0][0];

		t[0] =  c; t[1] = zero; t[2] = -s; t[3] = zero;
		t[4] = zero; t[5] = one; t[6] = zero; t[7] = zero;
		t[8] =  s; t[9] =  zero; t[10] = c; t[11] = zero;
		t[12] = zero; t[13] = zero; t[14] = zero; t[15] = one;

		*this *= temp;
	}

	void RotateZ(T rot)
	{
		Matrix4x4 temp;
		
		float s = sin(rot);
		float c = cos(rot);

		T * __restrict t = &temp.matrix[0][0];

		t[0] =  c; t[1] = s; t[2] = zero; t[3] = zero;
		t[4] = -s; t[5] = c; t[6] = zero; t[7] = zero;
		t[8] =  zero; t[9] =  zero; t[10] = one; t[11] = zero;
		t[12] = zero; t[13] = zero; t[14] = zero; t[15] = one;

		*this *= temp;
	}

	friend std::ostream& operator << (std::ostream &s, const Matrix4x4<T> &mat)
	{
		std::ios_base::fmtflags tempFlags = s.flags(); 
		const int width = 16;
		s.precision(6);
		s.setf (std::ios_base::fixed);

		s << "[" << std::setw (width) << mat[0][0] << 
			" " << std::setw (width) << mat[0][1] << 
			" " << std::setw (width) << mat[0][2] << 
			" " << std::setw (width) << mat[0][3] <<
			"\n" <<

			" " << std::setw (width) << mat[1][0] << 
			" " << std::setw (width) << mat[1][1] << 
			" " << std::setw (width) << mat[1][2] << 
			" " << std::setw (width) << mat[1][3] <<
			"\n" <<

			" " << std::setw (width) << mat[2][0] << 
			" " << std::setw (width) << mat[2][1] << 
			" " << std::setw (width) << mat[2][2] << 
			" " << std::setw (width) << mat[2][3] <<
			"\n" <<

			" " << std::setw (width) << mat[3][0] << 
			" " << std::setw (width) << mat[3][1] << 
			" " << std::setw (width) << mat[3][2] <<
			" " << std::setw (width) << mat[3][3] << "]"; 
		
		s.flags (tempFlags); 
		return s; 
	}
};


typedef Matrix4x4<float> Matrix4x4f;


#endif
