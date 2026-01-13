#ifndef __TRACE2D_H__
#define __TRACE2D_H__

#include <BBox2D.h>
#include <Plane2D.h>
#include <Vec2.h>

enum ETraceType { TT_NONE, TT_POINT, TT_SPHERE, TT_BOX };

class CTrace2D {
	friend class Hull2D;
	friend class Circle2D;
	bool bStartSolid;
	bool bAllSolid;
	Vec2D start, end;
	Vec2D delta;
	Vec2D dir;
	Vec2D hit;
	float length;
	float fraction;
	float exitFraction;
	Plane2D plane;
	ETraceType type;
	// for TT_SPHERE
	float radius;
	// for TT_BOX
	BBox2D box;

	void onFractionChanged() {
		hit = start + delta * fraction;
	}
	void setup(const Vec2D& a, const Vec2D& b) {
		this->bStartSolid = false;
		this->bAllSolid = false;
		this->start = a;
		this->hit = this->end = b;
		this->dir = this->delta = b - a;
		this->length = this->dir.normalize();
		this->fraction = 1.0f;
		this->exitFraction = 0.0f;
	}

public:
	float getLowestCornerDot(const Vec2D& n) {
		return box.getLowestCornerDot(n);
	}
	const BBox2D& getBox() const {
		return box;
	}
	bool isStartSolid() const {
		return this->bStartSolid;
	}
	bool isAllSolid() const {
		return this->bAllSolid;
	}
	float getFraction() const {
		return fraction;
	}
	Vec2D getPerp() const {
		return dir.getPerpendicular();
	}
	float getRadius() const {
		return radius;
	}
	bool isBox() const {
		return type == TT_BOX;
	}
	bool isSphere() const {
		return type == TT_SPHERE;
	}
	ETraceType getType() const {
		return type;
	}
	void setupRay(const Vec2D& a, const Vec2D& b) {
		this->setup(a, b);
		this->type = TT_POINT;
		this->radius = 0;
	}
	void setupSphere(const Vec2D& a, const Vec2D& b, float r) {
		this->setup(a, b);
		this->radius = r;
		this->type = TT_SPHERE;
	}
	void setupBox(const Vec2D& a, const Vec2D& b, float w, float h) {
		this->setup(a, b);
		this->box.fromWidthHeight(w, h);
		//	this->radius = this->box.getRadius();
		this->type = TT_BOX;
	}
	bool hasHit() const {
		return fraction != 1.0f;
	}
	const Vec2D& getStart() const {
		return start;
	}
	const Vec2D& getEnd() const {
		return end;
	}
	const Vec2D& getHit() const {
		return hit;
	}
	Vec2D getDir() const {
		return dir;
	}
	float getExitFraction() const {
		return exitFraction;
	}
};

#endif	// __TRACE2D_H__
