

#include "DemoTrace.h"
#include <DemoSystem/DemoContainer.h>
#include <DemoSystem/IDemoRenderer.h>
#include <BBox2D.h>


CDemoTrace::CDemoTrace() {
}

void CDemoTrace::onDemoInit() {
    //container->addSetting(&bDrawAABB, "Draw AABB", -1);
    //container->addSetting(101, "Remove last vertex", -1);
	hulls.addPoly(Vec2D(200,100),Vec2D(500,100), Vec2D(500,50), Vec2D(200,50),true);
	hulls.addPoly(Vec2D(200,200),Vec2D(300,200), Vec2D(300,100), Vec2D(200,100),true);
	hulls.addPoly(Vec2D(200,200),Vec2D(300,200), Vec2D(250,300),true);
}

void CDemoTrace::processMyEvent(int code) {
	if(code == 101) {
	}
}
const char* CDemoTrace::getName() const {
    return "Simple continous collision detection trace through 2D hulls";
}

void CDemoTrace::onMouseEvent(int x, int y, int button, bool bDown) {
    if (bDown) {
		if(button == 1) {
			pointA = Vec2D(x,y);
		} else {
			pointB = Vec2D(x,y);
		}
    }
}

bool CDemoTrace::onQuit() {
    return true;
}

void CDemoTrace::runFrame() {
    r->beginFrame(150, 150, 255);
    container->drawDebugTexts();
	for(int i = 0; i < hulls.size(); i++) {
		const Hull2D &h = hulls[i];
		this->drawPoly(h.getPoly(),100,0,0);
	}
	r->drawText(pointA.getX(),pointA.getY(),"A",0,0,0);
	r->drawText(pointB.getX(),pointB.getY(),"B",0,0,0);
    r->endFrame();
}
