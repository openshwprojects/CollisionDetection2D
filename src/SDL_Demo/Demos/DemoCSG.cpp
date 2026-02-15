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
	hulls.subtract(cutter);
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
