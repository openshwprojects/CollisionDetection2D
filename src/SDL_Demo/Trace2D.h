#ifndef __TRACE2D_H__
#define __TRACE2D_H__

#include <Vec2.h>
#include <Plane2D.h>

enum ETraceType {
	TT_NONE,
	TT_POINT,
	TT_SPHERE
};

class CTrace2D {
friend class Hull2D;
	bool bStartSolid;
	bool bAllSolid;
	Vec2D start, end;
	Vec2D delta;
	Vec2D dir;
	Vec2D hit;
	float radius;
	float length;
	float fraction;
	Plane2D plane;
	ETraceType type;

	void onFractionChanged() {
		hit = start + delta * fraction;
	}
	void setup(const Vec2D &a, const Vec2D &b) {
		this->bStartSolid = false;
		this->bAllSolid = false;
		this->start = a;
		this->hit = this->end = b;
		this->dir = this->delta = b - a;
		this->length = this->dir.normalize();
		this->fraction = 1.0f;
	}
public:
	Vec2D getPerp() const {
		return dir.getPerpendicular();
	}
	float getRadius() const {
		return radius;
	}
	bool isSphere() const {
		return type == TT_SPHERE;
	}
	ETraceType getType() const {
		return type;
	}
	void setupRay(const Vec2D &a, const Vec2D &b) {
		this->setup(a,b);
		this->type = TT_POINT;
	}
	void setupSphere(const Vec2D &a, const Vec2D &b, float r) {
		this->setup(a,b);
		this->radius = r;
		this->type = TT_SPHERE;
	}
	bool hasHit() const {
		return fraction != 1.0f;
	}
	const Vec2D &getStart() const {
		return start;
	}
	const Vec2D &getEnd() const {
		return end;
	}
	const Vec2D &getHit() const {
		return hit;
	}
	const Vec2D &getDir() const {
		return dir;
	}
};

#endif // __TRACE2D_H__

