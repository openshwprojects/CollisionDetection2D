#ifndef __DEMOCOLLISION_H__
#define __DEMOCOLLISION_H__

#include <Hull2DList.h>
#include <DemoSystem/BaseDemo.h>

class CDemoCollision : public CBaseDemo {
	Hull2DList hulls;
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

#endif // __DEMOCOLLISION_H__
