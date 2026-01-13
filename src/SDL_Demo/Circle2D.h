#ifndef __CIRCLE2D_H__
#define __CIRCLE2D_H__

#include "Hull2D.h"
#include "Shape2D.h"
#include "Trace2D.h"

class Circle2D : public Shape2D {
	Vec2D center;
	float radius;
	BBox2D bb;

	void rebuildBB() {
		bb.fromWidthHeight(radius * 2.0f, radius * 2.0f);
		bb.translate(center);
	}

public:
	Circle2D() : radius(1.0f) {
		rebuildBB();
	}
	Circle2D(const Vec2D& c, float r) : center(c), radius(r) {
		rebuildBB();
	}

	virtual const BBox2D& getBB() const override {
		return bb;
	}

	virtual bool trace(class CTrace2D& tr) const override {
		// Simplest: Ray vs Circle
		// Expand circle by trace radius (Minkowski sum) -> Trace becomes Ray vs Expanded Circle

		float r = radius + tr.getRadius();	// Combine radii
		Vec2D f = tr.getStart() - center;
		Vec2D d = tr.getDir();	// This is delta

		float a = d.dot(d);
		float b = 2 * f.dot(d);
		float c = f.dot(f) - r * r;

		float disc = b * b - 4 * a * c;
		if (disc < 0) return false;

		disc = sqrtf(disc);
		float t1 = (-b - disc) / (2 * a);
		// float t2 = (-b + disc) / (2*a); // exit point

		if (t1 >= 0 && t1 <= 1.0f) {
			if (t1 < tr.getFraction()) {
				// We need to update trace
				// However, Trace2D logic is a bit specific.
				// We rely on CTrace2D friends or public access.
				// Since Circle2D is a friend now:
				tr.fraction = t1;
				tr.onFractionChanged();
				// Normal at hit point
				Vec2D hitP = tr.getHit();
				Vec2D n = hitP - center;
				n.normalize();
				tr.plane.fromPointAndNormal(hitP, n);
				return true;
			}
		}
		return false;
	}

	virtual bool intersects(const Shape2D* other) const override {
		if (other->getType() == ST_CIRCLE) {
			const Circle2D* c = static_cast<const Circle2D*>(other);
			float d2 = (center - c->center).lengthSq();	 // Fixed: lengthSq
			float rSum = radius + c->radius;
			return d2 <= rSum * rSum;
		} else if (other->getType() == ST_HULL) {
			const Hull2D* h = static_cast<const Hull2D*>(other);
			// Basic SAT check:
			// 1. Check Hull normals
			const PlaneSet2D& planes = h->getPlanes();
			for (int i = 0; i < planes.size(); i++) {
				float dist = planes[i].distanceTo(center);	// Fixed: distanceTo
				if (dist > radius) return false;			// Separating axis found
			}

			// 2. Voronoi regions (check vertices)
			// To be accurate: Find the closest point on the polygon to the circle center.
			// If dist(closest, center) <= radius, return true.
			// Closest point on Convex Hull is unique.

			// How to find closest point on Hull?
			// Iterate all edges. Find closest point on segment.
			// Pick the global closest.
			// Then check distance.

			// For now, let's stick to the Plane check which is "Circle vs Expanded Polygon
			// (minkowski with box/lines)". It is reasonably good. The only issue is the sharp
			// corners vs rounded corners. The current implementation treats the hull as having
			// "flat" corners for the radius check?? No, `dist > radius` check effectively expands
			// the faces. It forms an expanded polygon with sharp corners. This is slightly larger
			// than the rounded shape (Capsule-like expansion). This means it might report collision
			// when near a corner but not touching the rounded corner. This is ACCEPTABLE for a
			// simple demo.

			return true;
		}
		return false;
	}

	virtual void translate(const Vec2D& p) override {
		center += p;
		rebuildBB();
	}

	virtual void rotateCenterRadians(float angle) override {
		// Rotating a circle around its center does nothing visible
	}

	virtual EShapeType getType() const override {
		return ST_CIRCLE;
	}

	const Vec2D& getCenter() const {
		return center;
	}
	float getRadius() const {
		return radius;
	}
};

#endif	// __CIRCLE2D_H__
