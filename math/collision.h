#ifndef COLLISION_H
#pragma once
#define COLLISION_H

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "vec2.h"

namespace Collision
{
	enum INTERSECT_RESULT
	{
		PARALLEL,
		COINCIDENT,
		NOT_INTERSECTING,
		INTERSECTING,
		OUTSIDE_BOUNDS,
		MULTIPLE_INTERSECTING
	};

	enum WINDING
	{
		COLINEAR = 0,
		CLOCKWISE,
		COUNTER_CLOCKWISE
	};
}

bool LineCollision(const Vec2f& A1, const Vec2f& A2, const Vec2f& B1, const Vec2f& B2);
Collision::INTERSECT_RESULT LineSegmentIntersection(const Vec2f& p0, const Vec2f& p1, const Vec2f& p2, const Vec2f& p3, Vec2f& intersection);
Collision::INTERSECT_RESULT DistancePointLine(const Vec2f& point, const Vec2f& lineStart, const Vec2f& lineEnd, float& distance);
Collision::INTERSECT_RESULT ClosestPointOnLine(const Vec2f& point, const Vec2f& lineStart, const Vec2f& lineEnd, Vec2f& closestPoint);
float DistanceSqrd(const Vec2f& pt1, const Vec2f& pt2);
float Distance(const Vec2f& pt1, const Vec2f& pt2);
Collision::WINDING FindTripletOrientation(const Vec2f& p, const Vec2f& q, const Vec2f& r);
bool IsPointInsidePolygon(const vector<Vec2f>& polygon, const Vec2f& p, const float& polygonXMax);
bool IsPointInFov(const Vec2f& pos, const Vec2f& heading, const Vec2f& target, const float& fov);

#endif
