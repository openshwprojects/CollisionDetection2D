#ifndef __HULL2D_H__
#define __HULL2D_H__

#include <Polygon2D.h>
#include <PlaneSet2D.h>
#include <BBox2D.h>

class Hull2D {
	BBox2D bb;
    Polygon2D vertices;
    PlaneSet2D planes;
public:
	void fromPoly(const class Vec2D &a, const Vec2D &b, const Vec2D&c, bool bSort = false);
	void fromPoly(const class Vec2D &a, const Vec2D &b, const Vec2D&c, const Vec2D&d, bool bSort = false);
	void fromPoly(const Array<Vec2D> &poly, bool bSort = false);
	bool trace(class CTrace2D &tr) const;
	bool hasPlane(const Plane2D &pl) const;
	bool isInside(const class Vec2D &p) const;
	void addAxisAlignedPlanesFromBounds();
	void rebuildPolygonFromPlanes();
	void translate(const class Vec2D &p);

	const Polygon2D &getPoly() const {
		return vertices;
	}
	const PlaneSet2D &getPlanes() const {
		return planes;
	}
};

#endif // __HULL2D_H__
