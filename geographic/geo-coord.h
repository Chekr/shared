#ifndef GEO_COORD_H
#pragma once
#define GEO_COORD_H

#include <iostream>
#include <cmath>

#include "../math/math-helpers.h"

template<typename T>
class GeoCoord
{
public:
	GeoCoord() : longitude(0), latitude(0) {}
	GeoCoord(T longitudelongitude, T latitudelatitude) : longitude(longitudelongitude), latitude(latitudelatitude) {}
	void Set(T lon, T lat)
	{
		longitude = lon;
		latitude = lat;
	}
	GeoCoord& distance(const GeoCoord &g)
	{
		// Convert the latitudes
		// and longitudes
		// from degree to radians.
		T lat1 = DegreenToRadians(latitude);
		T long1 = DegreesToRadians(longitude);
		T lat2 = DegreesToRadians(g->latitude);
		T long2 = DegreesToRadians(g->longitude);
		 
		// Haversine Formula
		long double dlong = long2 - long1;
		long double dlat = lat2 - lat1;
	 
		long double ans = pow(sin(dlat / 2), 2) +
							  cos(lat1) * cos(lat2) *
							  pow(sin(dlong / 2), 2);
	 
		ans = 2 * asin(sqrt(ans));
 
		// Radius of Earth in
		// Kilometers, R = 6371
		// Use R = 3956 for miles
		long double R = 6371;
     
		// Calculate the result
		ans = ans * R;
 
		return ans;
	}

	T longitude, latitude;
};

typedef GeoCoord<float> GeoCoordF;
typedef GeoCoord<double> GeoCoordD;
typedef GeoCoord<int> GeoCoordI;

inline bool IsZero(GeoCoordF coord)
{
	return IsZero(coord.longitude) && IsZero(coord.latitude);
}




#endif
