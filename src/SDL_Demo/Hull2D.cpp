#include "Hull2D.h"

#include "Trace2D.h"

void Hull2D::fromPoly(const Vec2D& a, const Vec2D& b, const Vec2D& c, const Vec2D& d, bool bSort) {
	planes.fromPoly(a, b, c, d, bSort);
	rebuildPolygonFromPlanes();
}
void Hull2D::fromPoly(const Vec2D& a, const Vec2D& b, const Vec2D& c, bool bSort) {
	planes.fromPoly(a, b, c, bSort);
	rebuildPolygonFromPlanes();
}
void Hull2D::rebuildPlanesFromPolygons() {
	planes.fromPoly(vertices);
	vertices.calcBounds(bb);
}
void Hull2D::rebuildPolygonFromPlanes() {
	vertices.fromPlanes(planes);
	vertices.calcBounds(bb);
}
void Hull2D::addAxisAlignedPlanesFromBounds() {
	for (int i = 0; i < 4; i++) {
		Plane2D pl = bb.getPlane(i);
		if (planes.hasPlane(pl) == false) {
			planes.addPlane(pl);
		}
	}
}
void Hull2D::fromPoly(const Array<Vec2D>& poly, bool bSort) {
	planes.fromPoly(poly, bSort);
	rebuildPolygonFromPlanes();
}
void Hull2D::rotateAroundRadians(const class Vec2D& center, float angle) {
#if 0
	planes.rotateAroundRadians(center,angle);
	rebuildPolygonFromPlanes();
#else
	vertices.rotateAroundRadians(center, angle);
	rebuildPlanesFromPolygons();
#endif
}
void Hull2D::rotateCenterRadians(float angle) {
	Vec2D center = bb.getCenter();
	rotateAroundRadians(center, angle);
}
void Hull2D::translate(const class Vec2D& p) {
	planes.translate(p);
	vertices.translate(p);
	bb.translate(p);
}
bool Hull2D::isInside(const class Vec2D& p) const {
	for (int i = 0; i < 4; i++) {
		Plane2D pl = bb.getPlane(i);
		float d = pl.distanceTo(p);
		if (d > 0) return false;
	}
	return true;
}
bool Hull2D::intersectsSAT(const Hull2D& other, CCollision* col) const {
	float bestOverlap = -999999.0f;
	const Plane2D* bestPlane = NULL;
	int axisFrom = 0;  // 0 = this, 1 = other
	int bestPlaneIdx = -1;

	// Check planes of this hull
	for (int i = 0; i < planes.size(); i++) {
		const Plane2D& pl = planes[i];
		float minProj = 999999.0f;
		const Polygon2D& otherPoly = other.getPoly();
		for (int v = 0; v < otherPoly.size(); v++) {
			float d = pl.distanceTo(otherPoly[v]);
			if (d < minProj) {
				minProj = d;
			}
		}
		if (minProj > 0) return false;	// Separating axis found

		if (minProj > bestOverlap) {
			bestOverlap = minProj;
			bestPlane = &pl;
			axisFrom = 0;
			bestPlaneIdx = i;
		}
	}

	// Check planes of other hull
	const PlaneSet2D& otherPlanes = other.getPlanes();
	for (int i = 0; i < otherPlanes.size(); i++) {
		const Plane2D& pl = otherPlanes[i];
		float minProj = 999999.0f;
		const Polygon2D& myPoly = vertices;
		for (int v = 0; v < myPoly.size(); v++) {
			float d = pl.distanceTo(myPoly[v]);
			if (d < minProj) {
				minProj = d;
			}
		}
		if (minProj > 0) return false;	// Separating axis found

		if (minProj > bestOverlap) {
			bestOverlap = minProj;
			bestPlane = &pl;
			axisFrom = 1;
			bestPlaneIdx = i;
		}
	}

	if (col) {
		col->distance = -bestOverlap;
		col->normal = bestPlane->getNormal();

		const Hull2D* RefShape = this;
		const Hull2D* IncShape = &other;

		// Ensure normal points from Ref to Inc?
		// If axis is from THIS (0), normal points OUT of THIS (towards OTHER).
		// We want normal to point A -> B (This -> Other).
		// If axisFrom == 1 (Other), normal points OUT of OTHER (towards THIS).
		// So if axisFrom == 1, we flip normal to point This -> Other.

		if (axisFrom == 1) {
			col->normal = col->normal * -1.0f;
			RefShape = &other;
			IncShape = this;
		} else {
			// axis from This
		}

		// Find Incident Face on IncShape
		// Best alignment with -normal
		float bestDot = 1.0f;
		int incEdgeIdx = -1;
		const PlaneSet2D& incPlanes = IncShape->getPlanes();
		for (int i = 0; i < incPlanes.size(); i++) {
			float d = incPlanes[i].getNormal().dot(col->normal);
			if (d < bestDot) {
				bestDot = d;
				incEdgeIdx = i;
			}
		}

		// Incident Edge
		const Polygon2D& incPoly = IncShape->getPoly();
		Vec2D v1 = incPoly[incEdgeIdx];
		Vec2D v2 = incPoly[(incEdgeIdx + 1) % incPoly.size()];

		// Reference Edge (from bestPlaneIdx on RefShape)
		// We need the side planes
		const Polygon2D& refPoly = RefShape->getPoly();
		int refEdgeIdx = bestPlaneIdx;	// Indices match
		Vec2D refV1 = refPoly[refEdgeIdx];
		Vec2D refV2 = refPoly[(refEdgeIdx + 1) % refPoly.size()];

		Vec2D refEdgeDir = (refV2 - refV1);
		refEdgeDir.normalize();

		// Clip v1, v2 against side planes of Ref Face
		// Side Plane 1: at refV1, normal = refEdgeDir (points inside segment)
		// Distance to point p: (p - refV1).dot(refEdgeDir) >= 0

		// Clip v1, v2 against side planes of Ref Face
		// Side Plane 1: at refV1, normal = refEdgeDir (points inside segment)
		// Distance to point p: (p - refV1).dot(refEdgeDir) >= 0

		Vec2D clipPoints[8];
		clipPoints[0] = v1;
		clipPoints[1] = v2;
		int clipCount = 2;

		// We need to clip against TWO planes:
		// 1. Plane at refV1 with normal refEdgeDir
		// 2. Plane at refV2 with normal -refEdgeDir

		for (int pPass = 0; pPass < 2; pPass++) {
			Vec2D pRef;
			Vec2D n;
			if (pPass == 0) {
				pRef = refV1;
				n = refEdgeDir;
			} else {
				pRef = refV2;
				n = refEdgeDir * -1.0f;
			}

			std::vector<Vec2D> inputList;
			for (int k = 0; k < clipCount; k++) inputList.push_back(clipPoints[k]);

			clipCount = 0;
			if (inputList.empty()) break;

			Vec2D S = inputList[inputList.size() - 1];
			for (size_t k = 0; k < inputList.size(); k++) {
				Vec2D E = inputList[k];
				float dS = (S - pRef).dot(n);
				float dE = (E - pRef).dot(n);

				if (dE >= 0) {
					if (dS < 0) {
						// S outside, E inside -> output intersection
						float t = dS / (dS - dE);
						if (clipCount < 8) clipPoints[clipCount++] = S + (E - S) * t;
					}
					// E inside -> output E
					if (clipCount < 8) clipPoints[clipCount++] = E;
				} else if (dS >= 0) {
					// S inside, E outside -> output intersection
					float t = dS / (dS - dE);
					if (clipCount < 8) clipPoints[clipCount++] = S + (E - S) * t;
				}
				S = E;
			}
			if (clipCount < 2)
				break;	// Optimization: we usually want 2 points or maybe 1 deep point
		}

		// Use the Clipped points that are below reference face
		col->numPoints = 0;
		// bestPlane is from RefShape. distanceTo(p) <= 0 means contact

		for (int k = 0; k < clipCount; k++) {
			Vec2D p = clipPoints[k];
			if (bestPlane->distanceTo(p) <= 0.01f) {  // slightly relaxed
				if (col->numPoints < 2) {
					col->points[col->numPoints++] = p;
				}
			}
		}
	}

	return true;
}

bool Hull2D::intersects(const Shape2D* other, CCollision* col) const {
	if (other->getType() == ST_HULL) {
		return intersectsSAT(*static_cast<const Hull2D*>(other), col);
	} else if (other->getType() == ST_CIRCLE) {
		// Note: We might need to handle reverse collision to fill 'col' correctly
		// Currently Circle::intersects handles Circle vs Hull
		// If we call other->intersects(this), 'normal' will be from Circle to Hull
		// We might want to flip it?
		bool hit = other->intersects(this, col);
		if (hit && col) {
			col->normal = col->normal * -1.0f;
		}
		return hit;
	}
	return false;
}

bool Hull2D::trace(class CTrace2D& tr) const {
	bool bHit = false;
	float eps = 0.001f;
	bool bEndOut = false;
	bool bStartOut = false;
	float enterFrac = -1.0f;
	float leaveFrac = 1.0f;
	const Plane2D* bestPlane = 0;

	for (int i = 0; i < planes.size(); i++) {
		const Plane2D& pl = planes[i];
		float dist = pl.getDistance();
		// NOTE: this will work only if we have force-added
		// extra axis aligned planes
		if (tr.isSphere()) {
			dist -= tr.getRadius();
		} else if (tr.isBox()) {
			dist += tr.getLowestCornerDot(pl.getNormal());
		}
		float ds = tr.getStart().dot(pl.getNormal()) + dist;
		float de = tr.getEnd().dot(pl.getNormal()) + dist;
		if (de > 0) {
			bEndOut = true;
		}
		if (ds > 0) {
			bStartOut = true;
		}
		if (ds > 0 && (de >= eps || de >= ds)) {
			return false;
		}
		if (ds <= 0 && de <= 0) {
			continue;
		}
		if (ds > de) {
			float f = (ds - eps) / (ds - de);
			if (f < 0) {
				f = 0;
			}
			if (f > enterFrac) {
				enterFrac = f;
				bestPlane = &pl;
			}
		} else {
			float f = (ds + eps) / (ds - de);
			if (f > 1) {
				f = 1;
			}
			if (f < leaveFrac) {
				leaveFrac = f;
			}
		}
	}

	if (!bStartOut) {
		tr.bStartSolid = true;
		if (leaveFrac > tr.exitFraction) {
			tr.exitFraction = leaveFrac;
		}
		if (!bEndOut) {
			tr.bAllSolid = true;
			tr.fraction = 0;
		}
		return true;
	}

	if (enterFrac < leaveFrac) {
		if (enterFrac > -1 && enterFrac < tr.fraction) {
			if (enterFrac < 0) {
				enterFrac = 0;
			}
			tr.fraction = enterFrac;
			tr.plane = *bestPlane;
			tr.onFractionChanged();
			return true;
		}
	}
	return false;
}
