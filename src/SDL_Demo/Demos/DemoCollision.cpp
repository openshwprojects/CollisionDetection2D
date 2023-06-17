

#include "DemoCollision.h"
#include <DemoSystem/DemoContainer.h>
#include <DemoSystem/IDemoRenderer.h>
#include <BBox2D.h>
#include <Trace2D.h>

CDemoCollision::CDemoCollision() {
	draggingIndex = -1;
}

void CDemoCollision::onDemoInit() {
	hulls.addPoly(Vec2D(200,100),Vec2D(500,100), Vec2D(500,50), Vec2D(200,50),true);
	hulls.addPoly(Vec2D(200,200),Vec2D(300,200), Vec2D(300,100), Vec2D(200,100),true);
	Hull2D &lp = hulls.addPoly(Vec2D(200,200),Vec2D(300,200), Vec2D(250,300),true);

}

void CDemoCollision::processMyEvent(int code) {
	//if(code == 101) {
	//	curType++;
	//	if(curType > TT_BOX)
	//		curType = TT_POINT;
	//}
}
const char* CDemoCollision::getName() const {
    return "Simple collision";
}

void CDemoCollision::onMouseMoveEvent(int x, int y, int dX, int dY) {
	if(draggingIndex!=-1) {
		hulls[draggingIndex].translate(Vec2D(dX,dY));
	}
}
void CDemoCollision::onMouseEvent(int x, int y, int button, bool bDown) {
	if(button == 1) {
		if (bDown) {
			draggingIndex = hulls.findByPoint(Vec2D(x,y));
			printf("Chosen: %i\n",draggingIndex);
		} else {
			draggingIndex = -1;
		}
	} else {

	}
}

bool CDemoCollision::onQuit() {
    return true;
}

void CDemoCollision::runFrame() {
	CTrace2D tr;

    r->beginFrame(150, 150, 255);
    container->drawDebugTexts();
	
		container->drawDebugText("Current qqqqqqqqqqqqqq");

	for(int i = 0; i < hulls.size(); i++) {
		const Hull2D &h = hulls[i];
		bool bHasCollision = false;
		for(int j = 0; j < hulls.size(); j++) {
			const Hull2D &hi = hulls[j];
			if(i == j) {
				continue;
			}
			if(hi.getBB().intersectsBox(h.getBB())){
				bHasCollision = true;
			}
		}
		if(bHasCollision) {
			this->drawPoly(h.getPoly(),250,0,0);
		} else {
			this->drawPoly(h.getPoly(),0,250,0);
		}
	}

    r->endFrame();
}
