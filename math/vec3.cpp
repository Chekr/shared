#include "vec3.h"
#include "quaternion.h"

template<typename T>
Vec3<T> & Vec3<T>::rotate(float angle, const Vec3& axis)
{

	const float SinHalfAngle = sinf(DegreesToRadians(angle / 2));
	const float CosHalfAngle = cosf(DegreesToRadians(angle / 2));

	const float Rx = axis.x * SinHalfAngle;
	const float Ry = axis.y * SinHalfAngle;
	const float Rz = axis.z * SinHalfAngle;
	const float Rw = CosHalfAngle;
	Quaternion RotationQ(Rx, Ry, Rz, Rw);

	Quaternion ConjugateQ = RotationQ.Conjugate();
	//  ConjugateQ.Normalize();
	Quaternion W = RotationQ * (*this) * ConjugateQ;

	x = W.x;
	y = W.y;
	z = W.z;

	return*this;
}

//template class Vec3<int>;
template class Vec3<float>;
