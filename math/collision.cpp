#include "collision.h"

bool LineCollision(const Vec2f& a1, const Vec2f& a2,
	const Vec2f& b1, const Vec2f& b2)
{
	Vec2f a(a2 - a1);
	Vec2f b(b2 - b1);

	double f = a.det(b);//PerpDot(a, b);
	if (!f)      // lines are parallel
	{
		return false;
	}
	Vec2f c(b2 - a2);
	double aa = a.det(c);//PerpDot(a, c);
	double bb = b.det(c);//PerpDot(b, c);

	if (f < 0)
	{
		if (aa >= 0) { return false; }
		if (bb >= 0) { return false; }
		if (aa <= f) { return false; }
		if (bb <= f) { return false; }
	}
	else
	{
		if (aa <= 0) { return false; }
		if (bb <= 0) { return false; }
		if (aa >= f) { return false; }
		if (bb >= f) { return false; }
	}

	return true;
}

Collision::INTERSECT_RESULT LineSegmentIntersection(const Vec2f& p0, const Vec2f& p1, const Vec2f& p2, const Vec2f& p3, Vec2f& intersection)
{
	float denom = ((p3.y - p2.y)*(p1.x - p0.x)) -
				((p3.x - p2.x)*(p1.y - p0.y));

	float nume_a = ((p3.x - p2.x)*(p0.y - p2.y)) -
				((p3.y - p2.y)*(p0.x - p2.x));

	float nume_b = ((p1.x - p0.x)*(p0.y - p2.y)) -
				((p1.y - p0.y)*(p0.x - p2.x));

	if (IsZero(denom))
	{
		if (IsZero(nume_a) && IsZero(nume_b))
		{
			return Collision::COINCIDENT;
		}
		return Collision::PARALLEL;
	}

	float ua = nume_a / denom;
	float ub = nume_b / denom;

	// Get the intersection point.
	intersection.x = p0.x + ua*(p1.x - p0.x);
	intersection.y = p0.y + ua*(p1.y - p0.y);

	if (ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f)
	{
		return Collision::INTERSECTING;
	}

	return Collision::OUTSIDE_BOUNDS;
}

Collision::INTERSECT_RESULT DistancePointLine(const Vec2f& point, const Vec2f& lineStart, const Vec2f& lineEnd, float& distance)
{
	float lineMag;
	float u;
	Vec2f intersection;

	lineMag = Distance(lineEnd, lineStart);

	if (IsZero(lineMag))
	{
		return Collision::COINCIDENT;
	}

	u = ( ((point.x - lineStart.x) * (lineEnd.x - lineStart.x)) +
						((point.y - lineStart.y) * (lineEnd.y - lineStart.y)) )
				/ (lineMag * lineMag);



	intersection.x = lineStart.x + u * (lineEnd.x - lineStart.x);
	intersection.y = lineStart.y + u * (lineEnd.y - lineStart.y);


	distance = Distance(point, intersection);

	if (u < 0.0f || u > 1.0f)
	{
		return Collision::OUTSIDE_BOUNDS;   // closest point does not fall within the line segment
	}
	return Collision::INTERSECTING;
}

Collision::INTERSECT_RESULT ClosestPointOnLine(const Vec2f& point, const Vec2f& lineStart, const Vec2f& lineEnd, Vec2f& closestPoint)
{
	float lineMag;
	float u;
	
	lineMag = Distance(lineEnd, lineStart);

	u = (((point.x - lineStart.x) * (lineEnd.x - lineStart.x)) +
						((point.y - lineStart.y) * (lineEnd.y - lineStart.y)))
				/ (lineMag * lineMag);


	closestPoint.x = lineStart.x + u * (lineEnd.x - lineStart.x);
	closestPoint.y = lineStart.y + u * (lineEnd.y - lineStart.y);

	if (u < 0.0f || u > 1.0f)
	{
		return Collision::OUTSIDE_BOUNDS;   // closest point does not fall within the line segment
	}
	return Collision::INTERSECTING;
}

float DistanceSqrd(const Vec2f& pt1, const Vec2f& pt2)
{
	float dx = (pt1.x - pt2.x);
	float dy = (pt1.y - pt2.y);
	return (dx*dx + dy*dy);
}


float Distance(const Vec2f& pt1, const Vec2f& pt2)
{
	float distSquared = DistanceSqrd(pt1, pt2);
	if (distSquared == 0.0f)
	{
		return 0.0f;
	}

	return std::sqrt(distSquared);
}

Collision::WINDING FindTripletOrientation(const Vec2f& p, const Vec2f& q, const Vec2f& r)
{
	int val = (q.y - p.y) * (r.x - q.x) - 
			  (q.x - p.x) * (r.y - q.y); 
	  
	
	if (val > 0 ) { return Collision::WINDING::CLOCKWISE; }
	if (val < 0) { return Collision::WINDING::COUNTER_CLOCKWISE; }
	return Collision::WINDING::COLINEAR; 
}

// polygonXMax represents the maximum X constraint of the polygon
bool IsPointInsidePolygon(const vector<Vec2f>& polygon, const Vec2f& p, const float& polygonXMax)
{
	int n = polygon.size();
	if (n < 3) { return false; } 

	Vec2f outsidePoint(polygonXMax+1.0f, p.y);

	int count =0;
	int i = 0;
	do
	{
		int next = (i+1)%n;

		if(LineCollision(polygon[i], polygon[next], p, outsidePoint))
		{
			if(FindTripletOrientation(polygon[i], p, polygon[next]) == Collision::WINDING::COLINEAR)
			{
				float distToPoint;
				return DistancePointLine(polygon[i], polygon[next], p, distToPoint) == Collision::COINCIDENT;
			}
			count++;
		}
		i = next;
	} while(i != 0);

	return count&1; // same as (count%2 == 1)
}

bool IsPointInFov(const Vec2f& pos, const Vec2f& heading, const Vec2f& target, const float& fov)
{
	Vec2f toTarget = (target - pos).normalize();

	return pos.dot(toTarget) >= cos(fov/2.0f);
}

bool IsPointInsideAabb(const Vec2f& p, const Aabb& rect)
{
	return (p.x >= rect.pos.x && p.y >= rect.pos.y && p.x < rect.pos.x + rect.size.x && p.y < rect.pos.y + rect.size.y);
}

bool IsCollidingAabbVsAabb(const Aabb& r1, const Aabb& r2)
{
	return (r1.pos.x < r2.pos.x + r2.size.x && r1.pos.x + r1.size.x > r2.pos.x && r1.pos.y < r2.pos.y + r2.size.y && r1.pos.y + r1.size.y > r2.pos.y);
}

// Adapted from OneLoneCoder collision
bool RayVsRect(const Vec2f& rayOrigin, const Vec2f& rayDir, const Aabb& target, Vec2f& contactPoint, Vec2f& contactNormal, float& targetHitNear)
{
	contactNormal = { 0.0f,0.0f };
	contactPoint = { 0.0f,0.0f };

	// Cache division
	Vec2f invdir = 1.0f / rayDir;

	// Calculate intersections with rectangle bounding axes
	Vec2f tNear = (target.pos - rayOrigin) * invdir;
	Vec2f tFar = (target.pos + target.size - rayOrigin) * invdir;

	if (std::isnan(tFar.y) || std::isnan(tFar.x)) { return false; }
	if (std::isnan(tNear.y) || std::isnan(tNear.x)) { return false; }

	// Sort distances
	if (tNear.x > tFar.x) { std::swap(tNear.x, tFar.x); }
	if (tNear.y > tFar.y) { std::swap(tNear.y, tFar.y); }

	// Early rejection		
	if (tNear.x > tFar.y || tNear.y > tFar.x) { return false; }

	// Closest 'time' will be the first contact
	targetHitNear = std::max(tNear.x, tNear.y);

	// Furthest 'time' is contact on opposite side of target
	float tHitFar = std::min(tFar.x, tFar.y);

	// Reject if ray direction is pointing away from object
	if (tHitFar < 0)
	{
		return false;
	}

	// Contact point of collision from parametric line equation
	contactPoint = rayOrigin + targetHitNear * rayDir;

	//ASDF
	// the problem:
	// this isn't taking parallel movement into account. The side wall is, indeed, closer
	// but it's not actually trying to move in that direction
	if (tNear.x > tNear.y)
	{
		if (invdir.x < 0.0f)
		{
			contactNormal = { 1, 0 };
		}
		else
		{
			contactNormal = { -1, 0 };
		}
	}
	else if (tNear.x < tNear.y)
	{	
		if (invdir.y < 0.0f)
		{
			contactNormal = { 0, 1 };
		}
		else
		{
			contactNormal = { 0, -1 };
		}
	}

	// Note if tNear == tFar, collision is principly in a diagonal
	// so pointless to resolve. By returning a CN={0,0} even though its
	// considered a hit, the resolver wont change anything.
	return true;		
}

bool IsCollidingMovingAabbVsAabb(const float& dt, const Aabb& movingRect, const Vec2f& velocity, const Aabb& staticRect, Vec2f& intersectionPoint, Vec2f& intersectionNormal, float& contactTimeRatio)
{

// Check if dynamic rec:tangle is actually moving - we assume rectangles are NOT in collision to start
	if (IsZero(velocity))
	{
		return false;
	}

	// Expand target rectangle by source dimensions
	Aabb expanded_target;
	expanded_target.pos = staticRect.pos - movingRect.size / 2.0f;
	expanded_target.size = staticRect.size + movingRect.size;

	if (RayVsRect(movingRect.pos + movingRect.size / 2.0f, velocity * dt, expanded_target, intersectionPoint, intersectionNormal, contactTimeRatio))
	{
		return (contactTimeRatio >= 0.0f && contactTimeRatio < 1.0f);
	}	

	return false;
}

bool ResolveMovingAabbVsAabb(const float& dt, const Aabb& movingRect, Vec2f& velocity, const Aabb& staticRect, std::array<Aabb*, 4>& contact)
{
	Vec2f contactPoint, contactNormal;
	float contactTime = 0.0f;
	if (IsCollidingMovingAabbVsAabb(dt, movingRect, velocity, staticRect, contactPoint, contactNormal, contactTime))
	{
		//contact[0] = (contactNormal.y > 0.0f) ? &staticRect : nullptr;
		//contact[1] = (contactNormal.x < 0.0f) ? &staticRect : nullptr;
		//contact[2] = (contactNormal.y < 0.0f) ? &staticRect : nullptr;
		//contact[3] = (contactNormal.x > 0.0f) ? &staticRect : nullptr;


		velocity += contactNormal * Vec2f(std::abs(velocity.x), std::abs(velocity.y)) * (1.0f - contactTime);
		return true;
	}

	
	return false;
}


Aabb MinkowskiDifference(const Aabb& a, const Aabb& b)
{
	Vec2f topLeft = a.pos - (b.pos+b.size);
	Vec2f fullSize = a.size + b.size;
	//return new Aabb{topLeft + (fullSize / 2.0f), fullSize / 2.0f};
	return Aabb{topLeft, fullSize};
}

Vec2f ClosestPointOnBoundsToPoint(const Aabb& box, const Vec2f& point)
{
	// test x first
	float minDist = abs(point.x - box.pos.x);
	Vec2f boundsPoint(box.pos.x, point.y);
	Vec2f max = box.pos + box.size;
	if (abs(max.x - point.x) < minDist)
	{
		minDist = abs(max.x - point.x);
		boundsPoint = Vec2f(max.x, point.y);
	}
	if (abs(max.y - point.y) < minDist)
	{
		minDist = abs(max.y - point.y);
		boundsPoint = Vec2f(point.x, max.y);
	}
	if (abs(box.pos.y - point.y) < minDist)
	{
		minDist = abs(box.pos.y - point.y);
		boundsPoint = Vec2f(point.x, box.pos.y);
	}
	return boundsPoint;
}

