#ifndef __SHAPE2D_H__
#define __SHAPE2D_H__

#include "BBox2D.h"
#include "Vec2.h"

struct CCollision {
	Vec2D normal;
	float distance;
	Vec2D points[6];
	int numPoints;
};

enum EShapeType { ST_HULL, ST_CIRCLE };

class Shape2D {
public:
	virtual ~Shape2D() {
	}
	virtual const BBox2D& getBB() const = 0;
	virtual bool trace(class CTrace2D& tr) const = 0;
	virtual bool intersects(const Shape2D* other, CCollision* col = NULL) const = 0;
	virtual void translate(const Vec2D& p) = 0;
	virtual void rotateCenterRadians(float angle) = 0;
	virtual EShapeType getType() const = 0;
};

#endif	// __SHAPE2D_H__
