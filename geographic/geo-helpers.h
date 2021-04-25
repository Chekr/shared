#ifndef GEO_HELPERS_H
#define GEO_HELPERS_H

#include "../math/vec2.h"
#include "geo-coord.h"

inline Vec2f MercatorGeoCoordToCartesian(const GeoCoordF& coord, const Vec2f& worldRadii = Vec2f(40075000.0f, 20003930.0f))
{
	float latitude    = coord.latitude;
	float longitude   = coord.longitude;

	float mapWidth    = worldRadii.x; // equator in meters
	float mapHeight   = worldRadii.y; // prime meridiam in meters

	// get x value
	float x = (longitude+180.0f)*(mapWidth/360.0f);

	// convert from degrees to radians
	float latRad = latitude*g_pi/180;

	// get y value
	float mercN = log(tan((g_quarterPi)+(latRad/2.0f))); // natural log
	float y = (mapHeight/2.0f)-(mapWidth*mercN/(g_twoPi)); 

	return Vec2f(x, y);
}


#endif
