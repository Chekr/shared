#include "collision.h"
#include "math-helpers.h"


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

Vec2f ClosestPointOnLineSegment(const Vec2f& point, const Vec2f& lineStart, const Vec2f& lineEnd)
{
	float lineMag;
	float u;
	
	lineMag = Distance(lineEnd, lineStart);

	u = (((point.x - lineStart.x) * (lineEnd.x - lineStart.x)) +
						((point.y - lineStart.y) * (lineEnd.y - lineStart.y)))
				/ (lineMag * lineMag);

	if (u < 0.0f)
	{
		return lineStart;
	}
	else if(u > 1.0f)
	{
		return lineEnd;
	}

	return Vec2f(
		lineStart.x + u * (lineEnd.x - lineStart.x),
		lineStart.y + u * (lineEnd.y - lineStart.y)
		);
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

bool IntersectionOfLineAndCircle(const Vec2f& pA, const Vec2f& pB, const Vec2f& center, const float& radius, Vec2f& colA, Vec2f& colB)
{
	float a,b,c;
	float bb4ac;
	Vec2f  dp;

	dp.x = pB.x - pA.x;
	dp.y = pB.y - pA.y;
	a = dp.x * dp.x + dp.y * dp.y;
	b = 2.0f * (dp.x * (pA.x - center.x) + dp.y * (pA.y - center.y)); 
	c = center.x * center.x + center.y * center.y;
	c += pA.x * pA.x + pA.y * pA.y;
	c -= 2.0f * (center.x * pA.x + center.y * pA.y);
	c -= radius * radius;
	bb4ac = b * b - 4.0f * a * c;
	if (IsZero(std::abs(a)) || bb4ac < 0.0f)
	{
		return false;
	}

	float muA = (-b + sqrt(bb4ac)) / (2.0f * a);
    colA = pA + muA*(pB-pA);
	float muB = (-b - sqrt(bb4ac)) / (2.0f * a);
    colB = pA + muB*(pB-pA);

	return true;
}

bool IntersectionOfLineAndMovingCircle(const Vec2f& pA, const Vec2f& pB, const Vec2f& start, const Vec2f& target, const float& radius, Vec2f& intersect, Vec2f& tangent)
{
	// Extend the line toward the Source position by "radius" amount
	Vec2f lineCollision;
	Vec2f circleCollisionA;
	Vec2f circleCollisionB;

	Vec2f centered = pB-pA;
	Vec2f startCentered = start - pA;
	int startSide = centered.sign(startCentered); // return (+1/-1) to know what way to extrude
    Vec2f perp = centered.perpendicular().normalize()*startSide*radius;
	Vec2f pC = pA+perp;
	Vec2f pD = pB+perp;
	
    // Check if line has been crossed
    Vec2f colCenter = pD-pC;
    Vec2f startOffcentered = start - pC;
    Vec2f targetOffcentered = target - pC;
    int colStartSide = colCenter.sign(startOffcentered);
    int colTargetSide = colCenter.sign(targetOffcentered);
    // If moving along line
    if(colStartSide == colTargetSide && colStartSide != startSide) { return false; }
    // If on line and moving away
    if(colStartSide != colTargetSide && colTargetSide == startSide) { return false; } // 



	// Make sure start position wasn't past the extrusion (precision error)
	//Vec2f altCentered = pD-pC;
	//Vec2f altStartCentered = start - pC;
	//Vec2f altStartSide = altCentered.sign(altStartCentered);
	//if(???) {} 

	// Find intersection of line and movement path 
	Vec2f lineIntersection;
	Collision::INTERSECT_RESULT res = LineSegmentIntersection(pC, pD, start, target, lineIntersection);

	// !! Should there be a pre-check for performance to look for a collision
	// !! between the end points and movement path (shortest distance between 
	// !! point and line?

	// Find collision points between movement path and line endpoints
    float distance;
    float rSq = radius*radius;
    float distAToStartSq = (pA - start).norm();
    float distAToTargetSq = (pA - target).norm();
    float distBToStartSq = (pB - start).norm();
    float distBToTargetSq = (pB - target).norm();
    //std::cout << distBToTargetSq << " " << rSq << std::endl;
	bool pACollision = 
        distAToStartSq < rSq
        || distAToTargetSq < rSq
        || (DistancePointLine(pA, start, target, distance) == Collision::INTERSECTING 
                && distance < radius
            );
               //&& IntersectionOfLineAndCircle(start, target, pA, radius, pAColA, pAColB);
    bool pBCollision = 
        distBToStartSq < rSq
        || distBToTargetSq < rSq
        || (DistancePointLine(pB, start, target, distance) == Collision::INTERSECTING
                && distance < radius
            );
                //&& IntersectionOfLineAndCircle(start, target, pB, radius, pBColA, pBColB);

	Vec2f pAColA, pAColB;
    Vec2f pBColA, pBColB;
    if(pACollision)
    {
        IntersectionOfLineAndCircle(start, target, pA, radius, pAColA, pAColB);
        std::cout << "A="
        << " s:" << start
        << " t:" << target
        << " p:" << pA
        << " r:" << radius
        << " 1:" << pAColA
        << " 2:" << pAColB
        << " l1:" << (pAColA - start).length()
        << " l2:" << (pAColB - start).length();
    }
    if(pBCollision)
    {
        IntersectionOfLineAndCircle(start, target, pB, radius, pBColA, pBColB);
        std::cout << "B="
        << " d:" << distance
        << " s:" << start
        << " t:" << target
        << " p:" << pB
        << " r:" << radius
        << " 1:" << pBColA
        << " 2:" << pBColB
        << " l1:" << (pBColA - start).length()
        << " l2:" << (pBColB - start).length();
    }



	//bool hasCollision = false;
    enum ColliderType
    {
        NONE = 0,
        LINE,
        POINTA,
        POINTB,
    };
    ColliderType collider = ColliderType::NONE;


	if(res == Collision::INTERSECT_RESULT::INTERSECTING)
	{
		//hasCollision = true;
        collider = ColliderType::LINE;
		intersect = lineIntersection; 
        //isLineCollision = true;
	}
	if(pACollision)
	{
std::cout << "b";
		float pAColADistSq = (pAColA - start).norm();
		float pAColBDistSq = (pAColB - start).norm();
		float closestPointDistSq;
		Vec2f closestPoint;
		if(pAColADistSq < pAColBDistSq)
		{
			closestPoint = pAColA;
			closestPointDistSq = pAColADistSq;
		}
		else
		{
			closestPoint = pAColB;
			closestPointDistSq = pAColBDistSq;
		}


		if(collider == ColliderType::NONE)
		{
			float intersectionDistSq = (intersect - start).norm();
			if(closestPointDistSq < intersectionDistSq)
			{
				intersect = closestPoint;
                collider = ColliderType::POINTA;
			} 
		}
        else
        {
            intersect = closestPoint;
            collider = ColliderType::POINTA;
        }
	}
	if(pBCollision)
	{
std::cout << "c";
		float pBColADistSq = (pBColA - start).norm();
		float pBColBDistSq = (pBColB - start).norm();
		float closestPointDistSq;
		Vec2f closestPoint;
		if(pBColADistSq < pBColBDistSq)
		{
			closestPoint = pBColA;
			closestPointDistSq = pBColADistSq;
		}
		else
		{
			closestPoint = pBColB;
			closestPointDistSq = pBColBDistSq;
		}


		if(collider == ColliderType::NONE)
		{
			float intersectionDistSq = (intersect - start).norm();
			if(closestPointDistSq < intersectionDistSq)
			{
				intersect = closestPoint;
                collider = ColliderType::POINTB;
			} 
		}
        else
        {
            intersect = closestPoint;
            collider = ColliderType::POINTB;

        }
	}


    switch(collider)
    {
    case LINE:
        {
            Vec2f v = target-start;
            Vec2f vd = pB-pA;
            v = vd * (v.x*vd.x + v.y*vd.y) / (vd.x*vd.x + vd.y*vd.y);
            tangent = v;
        }
        break;
    case POINTA:
        {
            float length = (pB-pA).length();
            tangent = (intersect-pA).perpendicular().normalize()*length;
std::cout << "2";

        }
        break;
    case POINTB:
        {
            float length = (pB-pA).length();
            tangent = (intersect-pB).perpendicular().normalize()*length;
std::cout << "3" << length << " " << intersect << " " << tangent << std::endl;
        }
        break;
    };

	return collider != ColliderType::NONE;	
}
/*
bool IntersectionOfLineAndMovingCircle2(const Vec2f& pA, const Vec2f& pB, const Vec2f& start, const Vec2f& target, const float& radius, Vec2f& intersect, Vec2f& tangent)
{
	// Extend the line toward the Source position by "radius" amount
	Vec2f lineCollision;
	Vec2f circleCollisionA;
	Vec2f circleCollisionB;

	Vec2f centered = pB-pA;
	Vec2f startCentered = start - pA;
	int startSide = centered.sign(startCentered); // return (+1/-1) to know what way to extrude
    Vec2f perp = centered.perpendicular().normalize()*startSide*radius;
	Vec2f pC = pA+perp;
	Vec2f pD = pB+perp;
	
    // Check if line has been crossed
    Vec2f colCenter = pD-pC;
    Vec2f startOffcentered = start - pC;
    Vec2f targetOffcentered = target - pC;
    int colStartSide = colCenter.sign(startOffcentered);
    int colTargetSide = colCenter.sign(targetOffcentered);
    // If moving along line
    if(colStartSide == colTargetSide && colStartSide != startSide) { return false; }
    // If on line and moving away
    if(colStartSide != colTargetSide && colTargetSide == startSide) { return false; } // 



	// Make sure start position wasn't past the extrusion (precision error)
	//Vec2f altCentered = pD-pC;
	//Vec2f altStartCentered = start - pC;
	//Vec2f altStartSide = altCentered.sign(altStartCentered);
	//if(???) {} 

	// Find intersection of line and movement path 
	Vec2f lineIntersection;
	Collision::INTERSECT_RESULT res = LineSegmentIntersection(pC, pD, start, target, lineIntersection);

	// !! Should there be a pre-check for performance to look for a collision
	// !! between the end points and movement path (shortest distance between 
	// !! point and line?

	// Find collision points between movement path and line endpoints
    float distance;
    float rSq = radius*radius;
    float distAToStartSq = (pA - start).norm();
    float distAToTargetSq = (pA - target).norm();
    float distBToStartSq = (pB - start).norm();
    float distBToTargetSq = (pB - target).norm();
    //std::cout << distBToTargetSq << " " << rSq << std::endl;

	//bool hasCollision = false;
    enum ColliderType
    {
        NONE = 0,
        LINE,
        POINTA,
        POINTB,
    };
    ColliderType collider = ColliderType::NONE;


	if(res == Collision::INTERSECT_RESULT::INTERSECTING)
	{
		//hasCollision = true;
        collider = ColliderType::LINE;
		intersect = lineIntersection; 
        //isLineCollision = true;

        // line collider
        Vec2f v = target-start;
        Vec2f vd = pB-pA;
        v = vd * (v.x*vd.x + v.y*vd.y) / (vd.x*vd.x + vd.y*vd.y);
        tangent = v;


	}



   	return collider != ColliderType::NONE;	
}
*/

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

bool IntersectionOfLineExtensionAndMovingCircle(const Vec2f& pA, const Vec2f& pB, const Vec2f& start, const Vec2f& target, const float& radius, Vec2f& intersect, Vec2f& tangent)
{
	// Extend the line toward the Source position by "radius" amount
	Vec2f centered = pB-pA;
	Vec2f startCentered = start - pA;
	int startSide = centered.sign(startCentered); // return (+1/-1) to know what way to extrude
    Vec2f perp = centered.perpendicular().normalize()*startSide*radius;
	Vec2f pC = pA+perp;
	Vec2f pD = pB+perp;
	
    // Check if line has been crossed
    Vec2f colCenter = pD-pC;
    Vec2f startOffcentered = start - pC;
    Vec2f targetOffcentered = target - pC;
    int colStartSide = colCenter.sign(startOffcentered);
    int colTargetSide = colCenter.sign(targetOffcentered);
    // If moving along line
    if(colStartSide == colTargetSide && colStartSide != startSide) { return false; }
    // If on line and moving away
    if(colStartSide != colTargetSide && colTargetSide == startSide) { return false; } // 

    // NOTE : Conincident might not fire because of the above "moving along line" test

	// Make sure start position wasn't past the extrusion (precision error)
	//Vec2f altCentered = pD-pC;
	//Vec2f altStartCentered = start - pC;
	//Vec2f altStartSide = altCentered.sign(altStartCentered);
	//if(???) {} 

	// Find intersection of line and movement path 
	Vec2f lineIntersection;
	Collision::INTERSECT_RESULT res = LineSegmentIntersection(pC, pD, start, target, lineIntersection);

    bool hasCollision = false;

	if(res == Collision::INTERSECT_RESULT::INTERSECTING)
	{
		intersect = lineIntersection; 
        hasCollision = true;

        Vec2f v = target-start;
        Vec2f vd = pB-pA;
        v = vd * (v.x*vd.x + v.y*vd.y) / (vd.x*vd.x + vd.y*vd.y);
        tangent = v;
	}
    else if(res == Collision::COINCIDENT)
    {
		intersect = lineIntersection; 
        hasCollision = true;
        tangent = target-start;
    }


    //Vec2f v = target-start;
    //Vec2f vd = pB-pA;
    //v = vd * (v.x*vd.x + v.y*vd.y) / (vd.x*vd.x + vd.y*vd.y);
    //tangent = v;

	return hasCollision;	
}
