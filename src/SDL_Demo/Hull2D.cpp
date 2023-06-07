#include "Hull2D.h"
#include "Trace2D.h"

void Hull2D::fromPoly(const Vec2D &a, const Vec2D &b, const Vec2D&c, const Vec2D&d, bool bSort) {
	planes.fromPoly(a,b,c,d,bSort);
	vertices.fromPlanes(planes);
	vertices.calcBounds(bb);
}
void Hull2D::fromPoly(const Vec2D &a, const Vec2D &b, const Vec2D&c, bool bSort) {
	planes.fromPoly(a,b,c,bSort);
	vertices.fromPlanes(planes);
	vertices.calcBounds(bb);
}
void Hull2D::addAxisAlignedPlanesFromBounds() {
	for(int i = 0; i < 4; i++) {
		Plane2D pl = bb.getPlane(i);
		planes.addPlane(pl);
	}
}
void Hull2D::fromPoly(const Array<Vec2D> &poly, bool bSort) {
	planes.fromPoly(poly,bSort);
	vertices.fromPlanes(planes);
	vertices.calcBounds(bb);
}
bool Hull2D::trace(class CTrace2D &tr) const {
	bool bHit = false;
	float eps = 0.001f;
	bool bEndOut = false;
	bool bStartOut = false;
	float enterFrac = -1.0f;
	float leaveFrac = 1.0f;
	const Plane2D *bestPlane = 0;

	for(int i = 0; i < planes.size(); i++) {
		const Plane2D &pl = planes[i];
		float dist = pl.getDistance();
		// NOTE: this will work only if we have force-added
		// extra axis aligned planes
		if(tr.isSphere()) {
			dist -= tr.getRadius();
		}
		float ds = tr.getStart().dot(pl.getNormal()) + dist;
		float de = tr.getEnd().dot(pl.getNormal()) + dist;
		if(de > 0) {
			bEndOut = true;
		}
		if(ds > 0) {
			bStartOut = true;
		}
		if (ds > 0 && (de >= eps || de >= ds)) {
			return false;
		}
		if (ds <= 0 && de <= 0) {
			continue;
		}
		if (ds > de) {
			float f = (ds-eps) / (ds-de);
			if (f < 0) {
				f = 0;
			}
			if (f > enterFrac) {
				enterFrac = f;
				bestPlane = &pl;
			}
		} else {
			float f = (ds+eps)/(ds-de);
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



