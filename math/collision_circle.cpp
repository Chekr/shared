#include "collision.h"
#include "math-helpers.h"


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
    if(colStartSide == colTargetSide && colStartSide != startSide) {
        std::cout << "q";
        return false; }
    // If on line and moving away
    if(colStartSide != colTargetSide && colTargetSide == startSide) { 
        std::cout <<"w";
        return false; } // 



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

    /*
	// Find collision points between movement path and line endpoints
    float distance;
    float rSq = radius*radius;
    float distAToStartSq = (pA - start).norm();
    float distAToTargetSq = (pA - target).norm();
    float distBToStartSq = (pB - start).norm();
    float distBToTargetSq = (pB - target).norm();
    //std::cout << distBToTargetSq << " " << rSq << std::endl;
*/

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


