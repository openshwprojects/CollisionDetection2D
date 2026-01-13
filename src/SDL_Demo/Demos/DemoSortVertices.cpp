

#include "DemoSortVertices.h"

#include <BBox2D.h>
#include <DemoSystem/DemoContainer.h>
#include <DemoSystem/IDemoRenderer.h>

CDemoSortVertices::CDemoSortVertices() {
	bSortVertices = true;
	bDrawRaw = true;
	bDrawAABB = true;
	bDrawConvex = true;
}

void CDemoSortVertices::onDemoInit() {
	container->addSetting(&bSortVertices, "Sort vertices", -1);
	container->addSetting(&bDrawRaw, "Draw user-drawn poly", -1);
	container->addSetting(&bDrawConvex, "Draw regenerated convex", -1);
	container->addSetting(&bDrawAABB, "Draw AABB", -1);
	container->addSetting(101, "Remove last vertex", -1);
}

void CDemoSortVertices::processMyEvent(int code) {
	if (code == 101) {
		poly.removeLastVertex();
	}
}
const char* CDemoSortVertices::getName() const {
	return "Polygon from points -> planes from polygon -> polygon from planes";
}

void CDemoSortVertices::onMouseEvent(int x, int y, int button, bool bDown) {
	if (bDown) {
		poly.addVertex(x, y);
	}
}

bool CDemoSortVertices::onQuit() { return true; }

void CDemoSortVertices::runFrame() {
	r->beginFrame(150, 150, 255);
	container->drawDebugTexts();
	container->drawDebugText("Convex hull from points.");
	container->drawDebugText("LMB: Add vertex");
	if (bDrawRaw) {
		drawPoly(poly, 255, 0, 0);
	}
	planes.fromPoly(poly.getPoints(), bSortVertices);
	poly2.fromPlanes(planes);
	if (bDrawConvex) {
		drawPoly(poly2, 0, 255, 0);
	}
	if (bDrawAABB) {
		Polygon2D tmp;
		BBox2D box;
		poly2.addToBB(box);
		for (int i = 0; i < 4; i++) {
			tmp.addVertex(box.getCorner(i));
		}
		drawPoly(tmp, 0, 0, 255);
	}
	r->endFrame();
}
