

#include "DemoTrace.h"
#include <DemoSystem/DemoContainer.h>
#include <DemoSystem/IDemoRenderer.h>
#include <BBox2D.h>
#include <Trace2D.h>

CDemoTrace::CDemoTrace() {
	curType = TT_POINT;
}

void CDemoTrace::onDemoInit() {
    //container->addSetting(&bDrawAABB, "Draw AABB", -1);
    container->addSetting(101, "Next trace type", -1);
	hulls.addPoly(Vec2D(200,100),Vec2D(500,100), Vec2D(500,50), Vec2D(200,50),true);
	hulls.addPoly(Vec2D(200,200),Vec2D(300,200), Vec2D(300,100), Vec2D(200,100),true);
	Hull2D &lp = hulls.addPoly(Vec2D(200,200),Vec2D(300,200), Vec2D(250,300),true);
	lp.addAxisAlignedPlanesFromBounds();
	// just to test if it still works
	lp.rebuildPolygonFromPlanes();
}

void CDemoTrace::processMyEvent(int code) {
	if(code == 101) {
		curType++;
		if(curType > TT_SPHERE)
			curType = TT_POINT;
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
	CTrace2D tr;

    r->beginFrame(150, 150, 255);
    container->drawDebugTexts();
	if(curType == TT_POINT) {
		container->drawDebugText("Current trace type: ray");
	} else {
		container->drawDebugText("Current trace type: circle");
	}
	for(int i = 0; i < hulls.size(); i++) {
		const Hull2D &h = hulls[i];
		this->drawPoly(h.getPoly(),100,0,0);
	}
	if(curType == TT_POINT) {
		tr.setupRay(pointA,pointB);
	} else {
		tr.setupSphere(pointA,pointB,15);
	}
	hulls.trace(tr);
	if(tr.hasHit()) {
		r->setColor(255,0,0);
		r->drawLine(tr.getStart(),tr.getHit());
		if(tr.isSphere()) {
			Vec2D perp = tr.getPerp().getNormalized() * tr.getRadius();
			r->drawLine(tr.getStart()+perp,tr.getHit()+perp);
			r->drawLine(tr.getStart()-perp,tr.getHit()-perp);
		}
	} else {
		r->setColor(0,255,0);
		r->drawLine(tr.getStart(),tr.getEnd());
		if(tr.isSphere()) {
			Vec2D perp = tr.getPerp().getNormalized() * tr.getRadius();
			r->drawLine(tr.getStart()+perp,tr.getEnd()+perp);
			r->drawLine(tr.getStart()-perp,tr.getEnd()-perp);
		}
	}
	r->drawText(pointA.getX(),pointA.getY(),"A",0,0,0);
	r->drawText(pointB.getX(),pointB.getY(),"B",0,0,0);
    r->endFrame();
}
