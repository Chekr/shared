#include "Collision.h"

bool LineCollision(const Vec2f& A1, const Vec2f& A2,
	const Vec2f& B1, const Vec2f& B2)
{
	Vec2f a(A2 - A1);
	Vec2f b(B2 - B1);

	double f = a.det(b);//PerpDot(a, b);
	if (!f)      // lines are parallel
		return false;

	Vec2f c(B2 - A2);
	double aa = a.det(c);//PerpDot(a, c);
	double bb = b.det(c);//PerpDot(b, c);

	if (f < 0)
	{
		if (aa >= 0)     return false;
		if (bb >= 0)     return false;
		if (aa <= f)     return false;
		if (bb <= f)     return false;
	}
	else
	{
		if (aa <= 0)     return false;
		if (bb <= 0)     return false;
		if (aa >= f)     return false;
		if (bb >= f)     return false;
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

	if (denom == 0.0f)
	{
		if (nume_a == 0.0f && nume_b == 0.0f)
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
