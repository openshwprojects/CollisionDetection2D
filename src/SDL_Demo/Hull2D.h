#ifndef __HULL2D_H__
#define __HULL2D_H__

#include <BBox2D.h>
#include <PlaneSet2D.h>
#include <Polygon2D.h>

#include "Shape2D.h"

class Hull2D : public Shape2D {
	BBox2D bb;
	Polygon2D vertices;
	PlaneSet2D planes;

public:
	void fromPoly(const class Vec2D& a, const Vec2D& b, const Vec2D& c, bool bSort = false);
	void fromPoly(const class Vec2D& a, const Vec2D& b, const Vec2D& c, const Vec2D& d,
				  bool bSort = false);
	void fromPoly(const Array<Vec2D>& poly, bool bSort = false);
	virtual bool trace(class CTrace2D& tr) const override;
	bool hasPlane(const Plane2D& pl) const;
	bool isInside(const class Vec2D& p) const;
	bool intersectsSAT(const Hull2D& other, CCollision* col) const;

	// Shape2D interface
	virtual bool intersects(const Shape2D* other, CCollision* col = NULL) const override;
	virtual void translate(const class Vec2D& p) override;
	virtual void rotateCenterRadians(float angle) override;
	virtual EShapeType getType() const override {
		return ST_HULL;
	}

	void addAxisAlignedPlanesFromBounds();
	void rebuildPolygonFromPlanes();
	void rebuildPlanesFromPolygons();
	void rotateAroundRadians(const class Vec2D& center, float angle);

	const Polygon2D& getPoly() const {
		return vertices;
	}
	const PlaneSet2D& getPlanes() const {
		return planes;
	}
	virtual const BBox2D& getBB() const override {
		return bb;
	}
};

#endif	// __HULL2D_H__
