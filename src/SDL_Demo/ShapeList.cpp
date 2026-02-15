#include "ShapeList.h"

#include <vector>

#include "BBox2D.h"

void ShapeList::subtract(Shape2D* cutter) {
	if (!cutter) return;

	// Only Hull2D can be used as a cutter for now
	if (cutter->getType() != ST_HULL) return;

	Hull2D* cutterHull = static_cast<Hull2D*>(cutter);
	BBox2D cutterBB = cutterHull->getBB();
	const PlaneSet2D& cutterPlanes = cutterHull->getPlanes();

	ShapeList newShapes;

	for (int i = 0; i < shapes.size(); i++) {
		Shape2D* s = shapes[i];

		// Skip check for non-hulls for now (handle them by just copying)
		// Or process them if possible. Hull2D CSG only supports Hull vs Hull.
		if (s->getType() != ST_HULL) {
			if (s->getType() == ST_CIRCLE) {
				Circle2D* c = static_cast<Circle2D*>(s);
				newShapes.addCircle(c->getCenter(), c->getRadius());
			} else if (s->getType() == ST_CAPSULE) {
				Capsule2D* cap = static_cast<Capsule2D*>(s);
				newShapes.addCapsule(cap->getPointA(), cap->getPointB(), cap->getRadius());
			}
			continue;
		}

		Hull2D* h = static_cast<Hull2D*>(s);
		if (!h->getBB().intersectsBox(cutterBB)) {
			newShapes.addPoly(h->getPoly().getPoints(), false);	 // Copy
			continue;
		}

		// Clip logic
		Polygon2D remainingPoly = h->getPoly();
		bool fullyRemoved = false;

		for (int p = 0; p < cutterPlanes.size(); p++) {
			const Plane2D& plane = cutterPlanes[p];

			// Plane points OUT of cutter.
			// We want the piece of 'h' that is OUTSIDE cutter (Front of plane).
			// clipByPlane keeps BACK.
			// So we use flipped plane (-Normal, -Dist) to keep Front.

			Plane2D outsidePlane(plane.getNormal() * -1.0f, plane.getDistance() * -1.0f);

			Polygon2D fragment = remainingPoly;
			fragment.clipByPlane(outsidePlane);

			if (fragment.size() >= 3) {
				newShapes.addPoly(fragment.getPoints(), false);
			}

			// Now clip 'remainingPoly' to be INSIDE this plane (Back of plane)
			// to continue digging into the cutter volume.
			remainingPoly.clipByPlane(plane);

			if (remainingPoly.size() < 3) {
				fullyRemoved = true;
				break;
			}
		}

		// If remainingPoly is still valid here, it means it is inside ALL cutter planes.
		// i.e. it is the intersection (h INTERSECT cutter).
		// We discard it because we are doing DIFFERENCE.
	}

	// Replace current shapes with new shapes
	std::vector<Shape2D*> extractedShapes;
	for (int i = 0; i < newShapes.size(); i++) {
		// Clone the shapes from newShapes to prevent double deletion
		Shape2D* s = newShapes[i];
		if (s->getType() == ST_HULL) {
			extractedShapes.push_back(new Hull2D(*static_cast<Hull2D*>(s)));
		} else if (s->getType() == ST_CIRCLE) {
			extractedShapes.push_back(new Circle2D(*static_cast<Circle2D*>(s)));
		} else if (s->getType() == ST_CAPSULE) {
			extractedShapes.push_back(new Capsule2D(*static_cast<Capsule2D*>(s)));
		}
	}

	clear();
	for (size_t i = 0; i < extractedShapes.size(); i++) {
		addShape(extractedShapes[i]);
	}
	// newShapes destructor runs here, deleting its temporary shapes.
}
