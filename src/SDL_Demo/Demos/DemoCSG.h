#ifndef __DEMOCSG_H__
#define __DEMOCSG_H__

#include <DemoSystem/BaseDemo.h>

#include "ShapeList.h"

class CDemoCSG : public CBaseDemo {
	ShapeList hulls;
	Hull2D* cutter;
	bool bDrawFill;

public:
	CDemoCSG();
	virtual void onDemoInit() override;
	virtual const char* getName() const override;
	virtual void runFrame() override;
	virtual void onMouseEvent(int x, int y, int button, bool bDown) override;
	virtual void onMouseMoveEvent(int x, int y, int dX, int dY) override;
	virtual bool onQuit() override;

private:
	void performCSG();
};

#endif	// __DEMOCSG_H__
