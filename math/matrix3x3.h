#ifndef MATRIX_3_X_3_H
#pragma once
#define MATRIX_3_X_3_H

#include <cstdlib> 
#include <cstdio> 

#include <iomanip>
#include <iostream>
#include <cmath>

#include <vector>

#include "math-helpers.h"
#include "vec2.h"

template<typename T>
class Matrix3x3
{
public:
	T matrix[3][3] = 
	{
		{zero,zero,zero},
		{zero,zero,zero},
		{zero,zero,zero}
	};
	T zero = static_cast<T>(0);
	T one = static_cast<T>(1);

	Matrix3x3() {}

	const T* operator[] (uint8_t index) const { return matrix[index];  }
	T* operator[] (uint8_t index) { return matrix[index];  }

	Matrix3x3 operator * (const Matrix3x3& mat) const
	{
		Matrix3x3 temp;
		Multiply(*this, mat, temp);
		return temp;
	}

	Matrix3x3& operator*= (const Matrix3x3& mat)
	{
		return *this = *this * mat;
	}

	static void Multiply(const Matrix3x3<T> &a, const Matrix3x3& b, Matrix3x3 &c)
	{
		// This would be faster in longform
		for (uint8_t i = 0; i < 3; ++i)
		{ 
			for (uint8_t j = 0; j < 3; ++j)
			{ 
				c[i][j] = a[i][0] * b[0][j] + 
					a[i][1] * b[1][j] + 
					a[i][2] * b[2][j]; 
			} 
		} 
	};

	void TransformVecs(std::vector<Vec2<T>> &points)
	{	
		for(Vec2<T>& point : points)
		{
			TransformVec(point);
		}
	}

	void TransformVec(Vec2<T> &point)
	{
		float tempX =(matrix[0][0]*point.x) + (matrix[1][0]*point.y) + (matrix[2][0]);
		float tempY = (matrix[0][1]*point.x) + (matrix[1][1]*point.y) + (matrix[2][1]);
  
		point.x = tempX;
		point.y = tempY;
	}

	void Identity()
	{
		T * __restrict t = &matrix[0][0];
		t[0] = one; t[1] = zero; t[2] = zero;
		t[3] = zero; t[4] = one; t[5] = zero;
		t[6] = zero; t[7] = zero; t[8] = one;
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
 
		matrix[1][0] = -sinR * cosP; 
		matrix[1][1] = cosR * cosP; 
		matrix[1][2] = sinP; 
	 
		matrix[2][0] = cosR * sinH + sinR * sinP * cosH; 
		matrix[2][1] = sinR * sinH - cosR * sinP * cosH; 
		matrix[2][2] = cosP * cosH;
	}

	void Translate(T x, T y)
	{
		Matrix3x3 temp;

		T * __restrict t = &temp.matrix[0][0];

		t[0] = one; t[1] = zero; t[2] = zero;
		t[3] = zero; t[4] = one; t[5] = zero;
		t[6] = x; t[7] = y; t[8] = one;

		*this *= temp;
	}

	void Scale(T xScale, T yScale)
	{
		Matrix3x3 temp;

		T * __restrict t = &temp.matrix[0][0];

		t[0] = xScale; t[1] = zero; t[2] = zero;
		t[3] = zero; t[4] = yScale; t[5] = zero;
		t[6] = zero; t[7] = zero; t[8] = one;

		*this *= temp;
	}

	void Rotate(T rot)
	{
		Matrix3x3 temp;

		float s = sin(rot);
		float c = cos(rot);

		T * __restrict t = &temp.matrix[0][0];

		t[0] =  c; t[1] = s; t[2] = zero;
		t[3] = -s; t[4] =  c; t[5] = zero;
		t[6] =  0; t[7] =  0; t[8] = one;

		*this *= temp;
	}

	void Rotate(const Vec2<T> &forward, const Vec2<T> &side)
	{
		Matrix3x3 temp;
		
		T * __restrict t = &temp.matrix[0][0];

		t[0] = forward.x; t[1] = forward.y; t[2] = zero;
		t[3] = side.x; t[4] = side.y; t[5] = zero;
		t[6] = zero; t[7] = zero; t[8] = one;

		*this *= temp;
	}

	

	friend std::ostream& operator << (std::ostream &s, const Matrix3x3<T> &mat)
	{
		std::ios_base::fmtflags tempFlags = s.flags(); 
		const int width = 16;
		s.precision(6);
		s.setf (std::ios_base::fixed);

		s << "[" << std::setw (width) << mat[0][0] << 
			" " << std::setw (width) << mat[0][1] << 
			" " << std::setw (width) << mat[0][2] << 
			"\n" <<

			" " << std::setw (width) << mat[1][0] << 
			" " << std::setw (width) << mat[1][1] << 
			" " << std::setw (width) << mat[1][2] << 
			"\n" <<

			" " << std::setw (width) << mat[2][0] << 
			" " << std::setw (width) << mat[2][1] << 
			" " << std::setw (width) << mat[2][2] << "]"; 
		
		s.flags (tempFlags); 
		return s; 
	}
};


typedef Matrix3x3<float> Matrix3x3f;


#endif
