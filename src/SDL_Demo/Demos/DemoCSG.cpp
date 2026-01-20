#include "DemoCSG.h"

#include <BBox2D.h>
#include <DemoSystem/DemoContainer.h>
#include <DemoSystem/IDemoRenderer.h>

CDemoCSG::CDemoCSG() {
	cutter = NULL;
	bDrawFill = true;
}

void CDemoCSG::onDemoInit() {
	hulls.clear();
	// Add some world objects
	hulls.addPoly(Vec2D(100, 100), Vec2D(300, 100), Vec2D(300, 300), Vec2D(100, 300), true);
	hulls.addPoly(Vec2D(400, 150), Vec2D(600, 150), Vec2D(500, 400), true);
	hulls.addCircle(Vec2D(600, 100), 50.0f);

	// Create cutter
	cutter = new Hull2D();
	cutter->fromPoly(Vec2D(0, 0), Vec2D(80, 0), Vec2D(80, 60), Vec2D(0, 60), true);
	cutter->translate(Vec2D(300, 200));

	container->addSetting(&bDrawFill, "Draw Fill", 'f');
}

const char* CDemoCSG::getName() const {
	return "Example 4 - CSG Operations";
}

void CDemoCSG::onMouseMoveEvent(int x, int y, int dX, int dY) {
	if (cutter) {
		cutter->translate(Vec2D(dX, dY));
	}
}

void CDemoCSG::performCSG() {
	if (!cutter) return;

	ShapeList newHulls;
	BBox2D cutterBB = cutter->getBB();
	const PlaneSet2D& cutterPlanes = cutter->getPlanes();

	for (int i = 0; i < hulls.size(); i++) {
		Shape2D* s = hulls[i];

		// Skip check for non-hulls for now (handle them by just copying)
		// Or process them if possible. Hull2D CSG only supports Hull vs Hull.
		if (s->getType() != ST_HULL) {
			if (s->getType() == ST_CIRCLE) {
				Circle2D* c = static_cast<Circle2D*>(s);
				newHulls.addCircle(c->getCenter(), c->getRadius());
			}
			continue;
		}

		Hull2D* h = static_cast<Hull2D*>(s);
		if (!h->getBB().intersectsBox(cutterBB)) {
			newHulls.addPoly(h->getPoly().getPoints(), false);	// Copy
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
				newHulls.addPoly(fragment.getPoints(), false);
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

	// Replace world
	hulls.clear();
	// Copy back from newHulls
	// newHulls destroys its content on destruction, so we must be careful.
	// ShapeList owns pointers.
	// We can't just assignment-copy ShapeList.
	// Wait, ShapeList doesn't have copy constructor/assignment operator implemented deep copy.
	// It has vector of pointers.
	// CDemoCSG::performCSG uses newHulls as a temporary builder.
	// We should move pointers from newHulls to hulls.

	// Helper to transfer ownership (hacky if ShapeList API doesn't support it)
	// ShapeList::addShape takes ownership.
	// We can iterate newHulls, take ptrs, add to hulls, and clear newHulls' internal vector without
	// deleting? ShapeList doesn't expose 'release' or 'extract'.

	// Better: use 'hulls' to store new objects directly?
	// No, we are iterating 'hulls'.

	// I can rebuild 'hulls' from scratch.

	// Let's modify ShapeList/Hull2D usage or make newHulls persist and swap.
	// ShapeList::shapes is private.
	// ShapeList doesn't have swap.
	// I will iterate newHulls using operator[], clone/add to 'hulls', then clear newHulls (which
	// deletes its copies). Or better: Implement a 'takeOwnership' or just modify hulls in a
	// separate vector<Shape2D*> then rebuild hulls.

	std::vector<Shape2D*> extractedShapes;
	for (int i = 0; i < newHulls.size(); i++) {
		// We want to KEEP these shapes.
		// But newHulls will delete them.
		// We need to clone them into 'hulls' OR prevent newHulls from deleting them.

		// Since I can't easily prevent newHulls from deleting without modifying ShapeList,
		// I will just clone them.
		Shape2D* s = newHulls[i];
		if (s->getType() == ST_HULL) {
			extractedShapes.push_back(new Hull2D(*static_cast<Hull2D*>(s)));
		} else if (s->getType() == ST_CIRCLE) {
			extractedShapes.push_back(new Circle2D(*static_cast<Circle2D*>(s)));
		}
	}

	hulls.clear();
	for (size_t i = 0; i < extractedShapes.size(); i++) {
		hulls.addShape(extractedShapes[i]);
	}
	// newHulls destructor runs here, deleting its temporary shapes.
}

void CDemoCSG::onMouseEvent(int x, int y, int button, bool bDown) {
	if (button == 3) {	// RMB
		if (bDown && cutter) {
			cutter->rotateCenterRadians(DEG2RAD(45.0f));
		}
	} else if (button == 1) {  // LMB
		if (bDown) {
			performCSG();
		}
	}
}

bool CDemoCSG::onQuit() {
	if (cutter) delete cutter;
	return true;
}

void CDemoCSG::runFrame() {
	r->beginFrame(100, 100, 100);  // Darker background
	container->drawDebugTexts();

	container->drawDebugText("Mouse: Move Cutter, LMB: Cut, RMB: Rotate Cutter");

	// Draw World
	for (int i = 0; i < hulls.size(); i++) {
		Shape2D* h = hulls[i];
		if (h->getType() == ST_HULL) {
			Hull2D* hull = static_cast<Hull2D*>(h);
			if (bDrawFill) {
				r->setColor(0, 255, 0);	 // Green
				r->fillPoly(hull->getPoly());
			}
			r->setColor(255, 255, 255);	 // Outline
			// We don't have drawPolyLoop in IDemoRenderer, only drawLine.
			// Just fill is fine or iterate lines.
			const Polygon2D& p = hull->getPoly();
			for (int k = 0; k < p.size(); k++) {
				r->drawLine(p[k], p[(k + 1) % p.size()]);
			}
		} else if (h->getType() == ST_CIRCLE) {
			Circle2D* c = static_cast<Circle2D*>(h);
			r->setColor(0, 255, 0);
			r->drawCircle(c->getCenter(), c->getRadius());
		}
	}

	// Draw Cutter
	if (cutter) {
		// Red, semi transparent if separate alpha supported?
		// IDemoRenderer has setColor with alpha.
		// But SDLDemoRenderer might implementation dependent.
		r->setColor(255, 0, 0, 128);
		r->fillPoly(cutter->getPoly());
		r->setColor(255, 200, 200, 255);
		const Polygon2D& p = cutter->getPoly();
		for (int k = 0; k < p.size(); k++) {
			r->drawLine(p[k], p[(k + 1) % p.size()]);
		}
	}

	r->endFrame();
}
