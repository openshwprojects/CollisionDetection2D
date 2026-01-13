

#include "DemoCollision.h"

#include <BBox2D.h>
#include <DemoSystem/DemoContainer.h>
#include <DemoSystem/IDemoRenderer.h>
#include <Trace2D.h>

CDemoCollision::CDemoCollision() {
	draggingIndex = -1;
}

void CDemoCollision::onDemoInit() {
	hulls.clear();
	hulls.addPoly(Vec2D(200, 100), Vec2D(500, 100), Vec2D(500, 50), Vec2D(200, 50), true);
	hulls.addPoly(Vec2D(200, 200), Vec2D(300, 200), Vec2D(300, 100), Vec2D(200, 100), true);
	hulls.addPoly(Vec2D(200, 200), Vec2D(300, 200), Vec2D(250, 300), true);

	// New shapes
	// Vertical wall on the right
	hulls.addPoly(Vec2D(600, 50), Vec2D(650, 50), Vec2D(650, 350), Vec2D(600, 350), true);

	// Triangle obstacle on the left
	hulls.addPoly(Vec2D(50, 250), Vec2D(150, 350), Vec2D(50, 350), true);

	// Angled box in the middle-bottom
	hulls.addPoly(Vec2D(400, 300), Vec2D(500, 250), Vec2D(550, 350), Vec2D(450, 400), true);

	// Sample Circle
	hulls.addCircle(Vec2D(100, 100), 40.0f);
	// Second Circle (Larger)
	hulls.addCircle(Vec2D(400, 200), 60.0f);
}

void CDemoCollision::processMyEvent(int code) {
	// if(code == 101) {
	//	curType++;
	//	if(curType > TT_BOX)
	//		curType = TT_POINT;
	// }
}
const char* CDemoCollision::getName() const {
	return "Simple collision";
}

void CDemoCollision::onMouseMoveEvent(int x, int y, int dX, int dY) {
	if (draggingIndex != -1) {
		hulls[draggingIndex]->translate(Vec2D(dX, dY));
	}
}
void CDemoCollision::onMouseEvent(int x, int y, int button, bool bDown) {
	if (button == 1) {
		if (bDown) {
			draggingIndex = hulls.findByPoint(Vec2D(x, y));
			printf("Chosen: %i\n", draggingIndex);
		} else {
			draggingIndex = -1;
		}
	} else if (button == 3) {
		if (bDown) {
			int which = hulls.findByPoint(Vec2D(x, y));
			if (which != -1) {
				hulls[which]->rotateCenterRadians(DEG2RAD(45.0f));
			}
		}
	}
}

bool CDemoCollision::onQuit() {
	return true;
}

void CDemoCollision::runFrame() {
	CTrace2D tr;

	r->beginFrame(150, 150, 255);
	container->drawDebugTexts();

	container->drawDebugText("Simple polygon collision.");
	container->drawDebugText("LMB: Drag, RMB: Rotate");

	for (int i = 0; i < hulls.size(); i++) {
		Shape2D* h = hulls[i];
		bool bHasCollision = false;
		for (int j = 0; j < hulls.size(); j++) {
			Shape2D* hi = hulls[j];
			if (i == j) {
				continue;
			}
			if (hi->getBB().intersectsBox(h->getBB())) {
				if (h->intersects(hi)) {
					bHasCollision = true;
				}
			}
		}

		int rCol = 0;
		int gCol = 250;
		if (bHasCollision) {
			rCol = 250;
			gCol = 0;
		}

		if (h->getType() == ST_HULL) {
			this->drawPoly(static_cast<Hull2D*>(h)->getPoly(), rCol, gCol, 0);
		} else if (h->getType() == ST_CIRCLE) {
			Circle2D* c = static_cast<Circle2D*>(h);
			r->setColor(rCol, gCol, 0);
			r->drawCircle(c->getCenter(), c->getRadius());
		}
	}

	r->endFrame();
}
