#ifndef __POLYGON2D_H__
#define __POLYGON2D_H__

#include <Common.h>
#include <Vec2.h>

class Polygon2D {
	Array<Vec2D> points;
public:


	int size() const {
		return points.size();
	}
	const Vec2D &operator[](int idx) const {
		return points[idx];
	}
	Vec2D &operator[](int idx) {
		return points[idx];
	}
	bool hasVertex(const Vec2D &v, float margin) const {
		for(int i = 0; i < points.size(); i++){
			if(points[i].isWithin(v,margin))
				return true;
		}
		return false;
	}
	// setup infinite polygon capped by one plane
	// normal facing outwards
	void fromPlane(const class Plane2D &pl, float maxWorldSize = 9999.0f);
	// removes everything in front of plane
	void clipByPlane(const class Plane2D &pl);
	void fromPlanes(const class PlaneSet2D &planes);
};

#endif // __POLYGON2D_H__
