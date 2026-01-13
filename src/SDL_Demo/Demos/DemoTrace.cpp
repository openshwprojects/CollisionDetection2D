

#include "DemoTrace.h"

#include <BBox2D.h>
#include <DemoSystem/DemoContainer.h>
#include <DemoSystem/IDemoRenderer.h>
#include <Trace2D.h>

CDemoTrace::CDemoTrace() {
	curType = TT_POINT;
	traceRadius = 5.0f;
}

void CDemoTrace::onDemoInit() {
	// container->addSetting(&bDrawAABB, "Draw AABB", -1);
	container->addSetting(101, "Next trace type", -1);
	container->addSetting(&traceRadius, 5, 1, 100, "Trace Radius");
	// Existing shapes
	hulls.addPoly(Vec2D(200, 100), Vec2D(500, 100), Vec2D(500, 50), Vec2D(200, 50), true);
	hulls.addPoly(Vec2D(200, 200), Vec2D(300, 200), Vec2D(300, 100), Vec2D(200, 100), true);
	Hull2D& lp = hulls.addPoly(Vec2D(200, 200), Vec2D(300, 200), Vec2D(250, 300), true);
	lp.addAxisAlignedPlanesFromBounds();
	lp.rebuildPolygonFromPlanes();

	// New shapes
	// Vertical wall on the right
	hulls.addPoly(Vec2D(600, 50), Vec2D(650, 50), Vec2D(650, 350), Vec2D(600, 350), true);

	// Triangle obstacle on the left
	hulls.addPoly(Vec2D(50, 250), Vec2D(150, 350), Vec2D(50, 350), true);

	// Angled box in the middle-bottom
	hulls.addPoly(Vec2D(400, 300), Vec2D(500, 250), Vec2D(550, 350), Vec2D(450, 400), true);
}

void CDemoTrace::processMyEvent(int code) {
	if (code == 101) {
		curType++;
		if (curType > TT_BOX) curType = TT_POINT;
	}
}
const char* CDemoTrace::getName() const {
	return "Simple continous collision detection trace through 2D hulls";
}

void CDemoTrace::onMouseEvent(int x, int y, int button, bool bDown) {
	if (bDown) {
		if (button == 1) {
			pointA = Vec2D(x, y);
		} else {
			pointB = Vec2D(x, y);
		}
	}
}

bool CDemoTrace::onQuit() {
	return true;
}

void CDemoTrace::runFrame() {
	CTrace2D tr;

	r->beginFrame(150, 150, 255);
	container->drawDebugTexts();
	container->drawDebugText("Continuous collision trace.");
	container->drawDebugText("LMB: Set Start, RMB: Set End");
	if (curType == TT_POINT) {
		container->drawDebugText("Current trace type: ray");
	} else if (curType == TT_BOX) {
		container->drawDebugText("Current trace type: box");
	} else {
		container->drawDebugText("Current trace type: circle");
	}
	for (int i = 0; i < hulls.size(); i++) {
		const Hull2D& h = hulls[i];
		this->drawPoly(h.getPoly(), 100, 0, 0);
	}
	if (curType == TT_POINT) {
		tr.setupRay(pointA, pointB);
	} else if (curType == TT_BOX) {
		tr.setupBox(pointA, pointB, traceRadius, traceRadius * 2);
	} else {
		tr.setupSphere(pointA, pointB, traceRadius);
	}
	hulls.trace(tr);

	char buf[256];
	sprintf(buf, "StartSolid: %s", tr.isStartSolid() ? "Yes" : "No");
	container->drawDebugText(buf);
	sprintf(buf, "AllSolid: %s", tr.isAllSolid() ? "Yes" : "No");
	container->drawDebugText(buf);
	sprintf(buf, "Fraction: %.2f", tr.getFraction());
	container->drawDebugText(buf);

	if (tr.isStartSolid()) {
		// Calculate exit point
		Vec2D delta = tr.getEnd() - tr.getStart();
		Vec2D exitPoint = tr.getStart() + delta * tr.getExitFraction();

		// Draw solid part in Yellow
		r->setColor(255, 255, 0);
		r->drawLine(tr.getStart(), exitPoint);

		// Draw clear part in Green
		r->setColor(0, 255, 0);
		r->drawLine(exitPoint, tr.getEnd());

		// Mark exit point
		r->drawCircle(exitPoint, 3.0f);

		if (tr.isSphere()) {
			Vec2D perp = tr.getPerp().getNormalized() * tr.getRadius();
			r->setColor(255, 255, 0);  // Boundaries for solid part
			r->drawLine(tr.getStart() + perp, exitPoint + perp);
			r->drawLine(tr.getStart() - perp, exitPoint - perp);
		} else if (tr.isBox()) {
			BBox2D box = tr.getBox();
			box.translate(tr.getStart());
			r->setColor(255, 255, 0);
			r->drawBox(box);
		}
	} else if (tr.hasHit()) {
		// Draw valid path in Red
		r->setColor(255, 0, 0);
		r->drawLine(tr.getStart(), tr.getHit());

		// Draw obstructed path in Grey
		r->setColor(100, 100, 100);
		r->drawLine(tr.getHit(), tr.getEnd());

		if (tr.isSphere()) {
			Vec2D perp = tr.getPerp().getNormalized() * tr.getRadius();
			r->setColor(255, 0, 0);
			r->drawLine(tr.getStart() + perp, tr.getHit() + perp);
			r->drawLine(tr.getStart() - perp, tr.getHit() - perp);
			r->drawCircle(tr.getHit(), tr.getRadius());

			// Ghost target
			r->setColor(100, 100, 100);
			r->drawCircle(tr.getEnd(), tr.getRadius());
		} else if (tr.isBox()) {
			BBox2D box = tr.getBox();
			r->setColor(255, 0, 0);
			box.translate(tr.getHit());
			r->drawBox(box);

			// Ghost target
			r->setColor(100, 100, 100);
			BBox2D endBox = tr.getBox();
			endBox.translate(tr.getEnd());
			r->drawBox(endBox);
		}
	} else {
		r->setColor(0, 255, 0);
		r->drawLine(tr.getStart(), tr.getEnd());
		if (tr.isSphere()) {
			Vec2D perp = tr.getPerp().getNormalized() * tr.getRadius();
			r->drawLine(tr.getStart() + perp, tr.getEnd() + perp);
			r->drawLine(tr.getStart() - perp, tr.getEnd() - perp);
		} else if (tr.isBox()) {
			BBox2D box = tr.getBox();
			box.translate(tr.getEnd());
			r->drawBox(box);
		}
	}
	r->drawText(pointA.getX(), pointA.getY(), "A", 0, 0, 0);
	r->drawText(pointB.getX(), pointB.getY(), "B", 0, 0, 0);
	r->endFrame();
}
