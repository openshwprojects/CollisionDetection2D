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
		Vec2D d = tr.getEnd() - tr.getStart();	// Full delta, not normalized dir

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

	virtual bool intersects(const Shape2D* other, CCollision* col = NULL) const override {
		if (other->getType() == ST_CIRCLE) {
			const Circle2D* c = static_cast<const Circle2D*>(other);
			Vec2D d = c->center - center;
			float d2 = d.lengthSq();
			float rSum = radius + c->radius;
			if (d2 <= rSum * rSum) {
				if (col) {
					float len = sqrt(d2);
					col->distance = rSum - len;
					if (len > 0.0001f) {
						col->normal = d / len;
					} else {
						col->normal = Vec2D(1, 0);
					}
					col->points[0] = center + col->normal * radius;
					col->numPoints = 1;
				}
				return true;
			}
		} else if (other->getType() == ST_HULL) {
			const Hull2D* h = static_cast<const Hull2D*>(other);

			// 1. Check if Center is INSIDE the Hull
			// We can use the planes to determine this quickly and also find the deepest penetration
			// axis if inside.
			const PlaneSet2D& planes = h->getPlanes();
			float maxDist = -9999999.0f;
			const Plane2D* deepPlane = NULL;
			bool inside = true;

			for (int i = 0; i < planes.size(); i++) {
				float d = planes[i].distanceTo(center);
				if (d > 0) {
					inside = false;
					// Optimization: If clearly separated by more than radius, we can exit early.
					// But we need to be careful about corners.
					// A circle can be outside a face by > 0 but < radius, yet closer to a corner.
					// However, if d > radius, it is definitely separated by this axis.
					if (d > radius) return false;
				}
				if (d > maxDist) {
					maxDist = d;
					deepPlane = &planes[i];
				}
			}

			if (inside) {
				// Center is strictly inside the Hull.
				// Push out along the shallowest plane normal.
				if (col) {
					// We want to push the circle OUT.
					// Plane normal points OUT of Hull (Towards Circle).
					// Separation requires moving Object A (Circle) AGAINST Collision Normal.
					// So Collision Normal must be opposite to Plane Normal.
					// Collision Normal: Circle -> Hull.
					col->distance = radius - maxDist;
					col->normal = deepPlane->getNormal() * -1.0f;  // Points INTO Hull
					col->points[0] =
						center +
						col->normal *
							radius;	 // This placement might be weird if normal is reversed.
					// Contact point on circle surface: center + normal * radius?
					// If normal points Circle->Hull. Point is on Hull side.
					col->numPoints = 1;
				}
				return true;
			} else {
				// Center is OUTSIDE (or on edge), but within radius of some planes.
				// We must find the closest point on the polygon to the center.
				float minDSq = 9999999.0f;
				Vec2D closestP;

				const Polygon2D& poly = h->getPoly();
				for (int i = 0; i < poly.size(); i++) {
					Vec2D p1 = poly[i];
					Vec2D p2 = poly[(i + 1) % poly.size()];

					// Closest point on segment
					Vec2D edge = p2 - p1;
					float lenSq = edge.lengthSq();
					float t = 0.0f;
					if (lenSq > 0.0001f) {
						t = (center - p1).dot(edge) / lenSq;
					}
					if (t < 0)
						t = 0;
					else if (t > 1)
						t = 1;

					Vec2D p = p1 + edge * t;
					float dSq = (center - p).lengthSq();
					if (dSq < minDSq) {
						minDSq = dSq;
						closestP = p;
					}
				}

				if (minDSq <= radius * radius) {
					if (col) {
						float dist = sqrt(minDSq);
						col->distance = radius - dist;

						if (dist > 0.0001f) {
							// Old: (center - closestP). Points Hull -> Circle.
							// New: (closestP - center). Points Circle -> Hull.
							col->normal = (closestP - center) / dist;
						} else {
							if (deepPlane)
								col->normal = deepPlane->getNormal() * -1.0f;
							else
								col->normal = Vec2D(-1, 0);
						}
						// Contact point: closestP is exactly on the hull.
						// Or point on circle: center + normal * radius?
						col->points[0] = closestP;
						col->numPoints = 1;
					}
					return true;
				}
			}
		} else if (other->getType() == ST_CAPSULE) {
			bool hit = other->intersects(this, col);
			if (hit && col) {
				col->normal = col->normal * -1.0f;
			}
			return hit;
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
