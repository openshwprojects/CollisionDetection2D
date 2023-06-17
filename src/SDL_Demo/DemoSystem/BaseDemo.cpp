#include "BaseDemo.h"
#include "IDemoRenderer.h"

void CBaseDemo::setContainer(CDemoContainer* cc) {
    this->container = cc;
}

void CBaseDemo::processMyEvent(int code) {
    // Implementation for processing custom events
}

void CBaseDemo::onDemoInit() {
    // Implementation for demo initialization
}

const char* CBaseDemo::getName() const {
    return "UnnamedDemo";
}

void CBaseDemo::drawPoly(const Polygon2D& p, byte r, byte g, byte b, byte a) {
    this->r->setColor(r, g, b);
	if(true) {
		for (int i = 0; i < p.size(); i++) {
			int n = (i + 1) % p.size();
			this->r->drawLine(p[i], p[n]);
		}
	} else {
		this->r->fillPoly(p);
	}
}

void CBaseDemo::onKeyEvent(int key, bool bDown) {
    // Implementation for handling key events
}
void CBaseDemo::onMouseMoveEvent(int x, int y, int dX, int dY) {

}

