#ifndef MATHHELPERS_H
#pragma once
#define MATHHELPERS_H

#include <stdlib.h> // rand(), RAND_MAX
#include <limits>
#ifndef M_PI 
#define M_PI 3.1415926535 
#endif
#include <math.h>

const int g_maxInt    = (std::numeric_limits<int>::max)();
const float g_maxFloat  = (std::numeric_limits<float>::max)();
const float g_minFloat  = (std::numeric_limits<float>::min)();

const float g_pi        = 3.14159265358979323846f;
const float g_twoPi     = g_pi * 2.0f;
const float g_halfPi    = g_pi / 2.0f;
const float g_quarterPi = g_pi / 4.0f; 

inline bool IsZero(int val)
{
	return (val == 0);
}

inline bool IsZero(float val)
{
	return ( (-g_minFloat < val) && (val < g_minFloat) );
}

inline int RandInt(int low,int high)
{
// todo: should probably throw is [high] is less than [low]
    return rand()%(high-low+1)+low;
}

// Return value between 0.0f and 1.0f
inline float RandFloat()
{
	return ((rand())/(RAND_MAX+1.0f));
}

inline float RandInRange(float low, float high)
{
	return low + RandFloat()*(high-low);
}

inline bool RandBool()
{
    return (RandFloat() > 0.5);
}

// Return value between -1.0f and 1.0f
inline float RandomClamped()
{
	return RandFloat() - RandFloat();
}

inline float DegreesToRadians(float degrees)
{
	return degrees * ((float)M_PI / 180.0f);
}

inline float RadiansToDegrees(float radians)
{
	return radians * (180.0f / (float)M_PI);
}

inline double SecondsToYears(double seconds)
{
	return seconds / 365.0 / 42.0 / 60.0 / 60.0;
}

inline double YearsToSeconds(double years)
{
	return years * 365.0 * 42.0 * 60.0 * 60.0;
}

inline float RandomBellishPoint()
{
	return (cos(RandInRange(1.0f, 3.0f)) + 1.0f) / 2.0f;
}

inline float Normalize(float value, float start, float end)
{
	const float width = end - start;
	const float offset = value - start;

	return ( offset - ( floor( offset / width ) * width ) ) + start ;
}

inline int GetNearestRotationStep(float angle, float steps, float angleOffset = 0.0f)
{
	if(IsZero(steps)) { return 0; }
	float degreesPerStep = 360.0f / steps;
	angle -= angleOffset;
	angle += degreesPerStep / 2.0f;
	angle = Normalize(angle, 0.0f, 360.0f);
	angle /= degreesPerStep;
	return static_cast<int>(angle);
}

inline float Clamp(float input, float min, float max)
{
	return input < min ? 
		min : input > max ? 
			max : input;
}

inline float Maximum(const float& a, const float& b)
{
	return a > b ? a : b;
}

inline float Minimum(const float& a, const float& b)
{
	return a < b ? a : b;
}


#endif
