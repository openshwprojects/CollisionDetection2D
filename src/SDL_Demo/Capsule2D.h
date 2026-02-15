#ifndef __CAPSULE2D_H__
#define __CAPSULE2D_H__

#include "Circle2D.h"
#include "Shape2D.h"
#include "Trace2D.h"

class Capsule2D : public Shape2D {
	Vec2D pointA, pointB;
	float radius;
	BBox2D bb;

	void rebuildBB() {
		bb.clear();
		bb.addPoint(pointA);
		bb.addPoint(pointB);
		// expand by radius in all directions
		bb.addPoint(pointA.getX() - radius, pointA.getY() - radius);
		bb.addPoint(pointA.getX() + radius, pointA.getY() + radius);
		bb.addPoint(pointB.getX() - radius, pointB.getY() - radius);
		bb.addPoint(pointB.getX() + radius, pointB.getY() + radius);
	}

	// Returns the closest point on the internal segment AB to point p
	Vec2D closestPointOnSegment(const Vec2D& p) const {
		Vec2D ab = pointB - pointA;
		float lenSq = ab.lengthSq();
		if (lenSq < 0.0001f) return pointA;
		float t = (p - pointA).dot(ab) / lenSq;
		if (t < 0)
			t = 0;
		else if (t > 1)
			t = 1;
		return pointA + ab * t;
	}

	// Closest distance squared between two segments (AB and CD)
	// Also outputs the closest points on each segment
	static float closestDistSqSegments(const Vec2D& a, const Vec2D& b, const Vec2D& c,
									   const Vec2D& d, Vec2D& outOnAB, Vec2D& outOnCD) {
		Vec2D ab = b - a;
		Vec2D cd = d - c;
		Vec2D ac = c - a;

		float d1 = ab.dot(ab);	// |AB|^2
		float d2 = ab.dot(cd);
		float d3 = cd.dot(cd);	// |CD|^2
		float d4 = ab.dot(ac);
		float d5 = cd.dot(ac);

		float denom = d1 * d3 - d2 * d2;

		float sN, sD, tN, tD;
		sD = tD = denom;

		if (denom < 0.0001f) {
			// Segments are parallel
			sN = 0.0f;
			sD = 1.0f;
			tN = -d5;
			tD = d3;
		} else {
			sN = d3 * d4 - d2 * d5;
			tN = d2 * d4 - d1 * d5;
		}

		// Clamp s
		if (sN < 0.0f) {
			sN = 0.0f;
			tN = -d5;
			tD = d3;
		} else if (sN > sD) {
			sN = sD;
			tN = -d5 + d2;
			tD = d3;
		}

		// Clamp t
		if (tN < 0.0f) {
			tN = 0.0f;
			if (d4 < 0.0f)
				sN = 0.0f;
			else if (d4 > d1)
				sN = sD;
			else {
				sN = d4;
				sD = d1;
			}
		} else if (tN > tD) {
			tN = tD;
			if ((d4 + d2) < 0.0f)
				sN = 0.0f;
			else if ((d4 + d2) > d1)
				sN = sD;
			else {
				sN = d4 + d2;
				sD = d1;
			}
		}

		float s = (fabs(sN) < 0.0001f) ? 0.0f : sN / sD;
		float t = (fabs(tN) < 0.0001f) ? 0.0f : tN / tD;

		outOnAB = a + ab * s;
		outOnCD = c + cd * t;

		return (outOnAB - outOnCD).lengthSq();
	}

public:
	Capsule2D() : radius(1.0f) {
		rebuildBB();
	}
	Capsule2D(const Vec2D& a, const Vec2D& b, float r) : pointA(a), pointB(b), radius(r) {
		rebuildBB();
	}

	virtual const BBox2D& getBB() const override {
		return bb;
	}

	virtual bool trace(class CTrace2D& tr) const override {
		// Trace vs Capsule: expand capsule radius by trace radius, then do ray vs capsule
		float r = radius + tr.getRadius();

		// Ray vs capsule = ray vs segment with radius r
		// Check the two hemicircles (at A and B) and the rectangle slab

		Vec2D rayStart = tr.getStart();
		Vec2D rayDelta = tr.getDir() * tr.getFraction();  // Only check remaining fraction
		// Actually use the full delta from trace
		Vec2D rayEnd = tr.getEnd();
		Vec2D rayD = rayEnd - rayStart;

		float bestT = tr.getFraction();
		Vec2D bestNormal;
		bool bHit = false;

		// 1. Check hemisphere at pointA
		{
			Vec2D f = rayStart - pointA;
			float a = rayD.dot(rayD);
			float b = 2.0f * f.dot(rayD);
			float c = f.dot(f) - r * r;
			float disc = b * b - 4.0f * a * c;
			if (disc >= 0) {
				disc = sqrtf(disc);
				float t1 = (-b - disc) / (2.0f * a);
				if (t1 >= 0 && t1 <= 1.0f && t1 < bestT) {
					bestT = t1;
					bestNormal = (rayStart + rayD * t1) - pointA;
					bestNormal.normalize();
					bHit = true;
				}
			}
		}
		// 2. Check hemisphere at pointB
		{
			Vec2D f = rayStart - pointB;
			float a = rayD.dot(rayD);
			float b = 2.0f * f.dot(rayD);
			float c = f.dot(f) - r * r;
			float disc = b * b - 4.0f * a * c;
			if (disc >= 0) {
				disc = sqrtf(disc);
				float t1 = (-b - disc) / (2.0f * a);
				if (t1 >= 0 && t1 <= 1.0f && t1 < bestT) {
					bestT = t1;
					bestNormal = (rayStart + rayD * t1) - pointB;
					bestNormal.normalize();
					bHit = true;
				}
			}
		}
		// 3. Check the "slab" — the infinite rectangle between A and B expanded by r
		// Project ray onto the perpendicular of segment AB
		{
			Vec2D segDir = pointB - pointA;
			float segLen = segDir.normalize();
			if (segLen > 0.0001f) {
				Vec2D segPerp = segDir.getPerpendicular();

				// Distance of ray start and end from the segment line
				float distStart = (rayStart - pointA).dot(segPerp);
				float distEnd = (rayEnd - pointA).dot(segPerp);

				// Check if ray crosses the slab [-r, +r]
				float tEnter = 0.0f, tExit = 1.0f;
				bool valid = true;

				if (fabs(distEnd - distStart) < 0.0001f) {
					// Ray parallel to segment
					if (fabs(distStart) > r) valid = false;
				} else {
					float t1 = (r - distStart) / (distEnd - distStart);
					float t2 = (-r - distStart) / (distEnd - distStart);
					if (t1 > t2) {
						float tmp = t1;
						t1 = t2;
						t2 = tmp;
					}
					tEnter = t1;
					tExit = t2;
					if (tEnter > 1.0f || tExit < 0.0f) valid = false;
					if (tEnter < 0.0f) tEnter = 0.0f;
					if (tExit > 1.0f) tExit = 1.0f;
				}

				if (valid && tEnter < bestT) {
					// Check that the hit point projects onto the segment (between A and B)
					Vec2D hitP = rayStart + rayD * tEnter;
					float proj = (hitP - pointA).dot(segDir);
					if (proj >= 0 && proj <= segLen) {
						bestT = tEnter;
						// Normal is perpendicular to segment, pointing toward ray start
						float side = (rayStart - pointA).dot(segPerp);
						bestNormal = (side >= 0) ? segPerp : segPerp * -1.0f;
						bHit = true;
					}
				}
			}
		}

		if (bHit && bestT < tr.getFraction()) {
			tr.fraction = bestT;
			tr.onFractionChanged();
			tr.plane.fromPointAndNormal(tr.getHit(), bestNormal);
			return true;
		}
		return false;
	}

	virtual bool intersects(const Shape2D* other, CCollision* col = NULL) const override {
		if (other->getType() == ST_CIRCLE) {
			const Circle2D* c = static_cast<const Circle2D*>(other);
			Vec2D closest = closestPointOnSegment(c->getCenter());
			Vec2D d = c->getCenter() - closest;
			float d2 = d.lengthSq();
			float rSum = radius + c->getRadius();
			if (d2 <= rSum * rSum) {
				if (col) {
					float len = sqrt(d2);
					col->distance = rSum - len;
					if (len > 0.0001f) {
						col->normal = d / len;
					} else {
						col->normal = Vec2D(1, 0);
					}
					col->points[0] = closest + col->normal * radius;
					col->numPoints = 1;
				}
				return true;
			}
		} else if (other->getType() == ST_CAPSULE) {
			const Capsule2D* o = static_cast<const Capsule2D*>(other);
			Vec2D onThis, onOther;
			float d2 = closestDistSqSegments(pointA, pointB, o->pointA, o->pointB, onThis, onOther);
			float rSum = radius + o->radius;
			if (d2 <= rSum * rSum) {
				if (col) {
					float len = sqrt(d2);
					col->distance = rSum - len;
					Vec2D d = onOther - onThis;
					if (len > 0.0001f) {
						col->normal = d / len;
					} else {
						col->normal = Vec2D(1, 0);
					}
					col->points[0] = onThis + col->normal * radius;
					col->numPoints = 1;
				}
				return true;
			}
		} else if (other->getType() == ST_HULL) {
			const Hull2D* h = static_cast<const Hull2D*>(other);
			const Polygon2D& poly = h->getPoly();
			const PlaneSet2D& planes = h->getPlanes();

			// SAT: Separating Axis Theorem for Capsule vs Convex Hull
			// Capsule = segment [pointA, pointB] + radius
			// Test axes: 1) hull face normals, 2) hull vertex to closest-on-segment normals
			float bestOverlap = 9999999.0f;
			Vec2D bestNormal;

			// --- 1. Hull face normals ---
			for (int i = 0; i < planes.size(); i++) {
				Vec2D n = planes[i].getNormal();
				float d = planes[i].getDistance();

				// Project capsule onto this axis
				float projA = pointA.dot(n) + d;
				float projB = pointB.dot(n) + d;
				// Closest capsule segment point to hull on this axis
				float closestProj = (projA < projB) ? projA : projB;
				// Capsule surface extends radius further toward hull (negative direction)
				float separation = closestProj - radius;

				if (separation > 0) {
					return false;  // Separating axis found
				}

				float overlap = -separation;
				if (overlap < bestOverlap) {
					bestOverlap = overlap;
					bestNormal = n;
				}
			}

			// --- 2. Hull vertex to closest point on capsule segment ---
			for (int i = 0; i < poly.size(); i++) {
				Vec2D v = poly[i];
				Vec2D closest = closestPointOnSegment(v);
				Vec2D axis = v - closest;
				float dist = axis.length();

				if (dist < 0.0001f) {
					// Vertex is ON the segment, definitely overlapping on this axis
					continue;
				}

				axis = axis / dist;	 // normalize

				// Project capsule onto axis
				float capProjA = pointA.dot(axis);
				float capProjB = pointB.dot(axis);
				float capMin = ((capProjA < capProjB) ? capProjA : capProjB) - radius;
				float capMax = ((capProjA > capProjB) ? capProjA : capProjB) + radius;

				// Project hull onto axis
				float hullMin = 9999999.0f, hullMax = -9999999.0f;
				for (int j = 0; j < poly.size(); j++) {
					float p = poly[j].dot(axis);
					if (p < hullMin) hullMin = p;
					if (p > hullMax) hullMax = p;
				}

				if (capMax < hullMin || hullMax < capMin) {
					return false;  // Separating axis found
				}

				// Compute overlap on this axis
				float overlap1 = capMax - hullMin;
				float overlap2 = hullMax - capMin;
				float overlap = (overlap1 < overlap2) ? overlap1 : overlap2;
				if (overlap < bestOverlap) {
					bestOverlap = overlap;
					// Normal should point from capsule toward hull
					bestNormal = axis;
				}
			}

			// No separating axis found, shapes overlap
			if (col) {
				col->distance = bestOverlap;
				// Ensure normal points from this (capsule) toward other (hull)
				Vec2D capCenter = (pointA + pointB) * 0.5f;
				Vec2D hullCenter = h->getBB().getCenter();
				Vec2D toHull = hullCenter - capCenter;
				if (toHull.dot(bestNormal) < 0) {
					bestNormal = bestNormal * -1.0f;
				}
				col->normal = bestNormal;
				col->points[0] = capCenter + bestNormal * radius;
				col->numPoints = 1;
			}
			return true;
		}
		return false;
	}

	virtual void translate(const Vec2D& p) override {
		pointA += p;
		pointB += p;
		rebuildBB();
	}

	virtual void rotateCenterRadians(float angle) override {
		Vec2D center = (pointA + pointB) * 0.5f;
		pointA.rotateAroundRadians(center, angle);
		pointB.rotateAroundRadians(center, angle);
		rebuildBB();
	}

	virtual EShapeType getType() const override {
		return ST_CAPSULE;
	}

	const Vec2D& getPointA() const {
		return pointA;
	}
	const Vec2D& getPointB() const {
		return pointB;
	}
	Vec2D getCenter() const {
		return (pointA + pointB) * 0.5f;
	}
	float getRadius() const {
		return radius;
	}
};

#endif	// __CAPSULE2D_H__
