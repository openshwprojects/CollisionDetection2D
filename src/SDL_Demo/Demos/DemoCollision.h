#ifndef __DEMOCOLLISION_H__
#define __DEMOCOLLISION_H__

#include <DemoSystem/BaseDemo.h>

#include "ShapeList.h"

class CDemoCollision : public CBaseDemo {
	ShapeList hulls;
	int draggingIndex;

public:
	CDemoCollision();

	virtual void onDemoInit();
	virtual const char* getName() const;
	virtual void onMouseEvent(int x, int y, int button, bool bDown);
	virtual void onMouseMoveEvent(int x, int y, int dX, int dY);
	virtual bool onQuit();
	virtual void runFrame();
	virtual void processMyEvent(int code);
};

#endif	// __DEMOCOLLISION_H__
