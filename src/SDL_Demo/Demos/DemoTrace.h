#ifndef __DEMOTRACE_H__
#define __DEMOTRACE_H__

#include <Hull2DList.h>
#include <DemoSystem/BaseDemo.h>

class CDemoTrace : public CBaseDemo {
	Hull2DList hulls;
	Vec2D pointA, pointB;
	int curType;
	float traceRadius;
public:
    CDemoTrace();

    virtual void onDemoInit();
    virtual const char* getName() const;
    virtual void onMouseEvent(int x, int y, int button, bool bDown);
    virtual bool onQuit();
    virtual void runFrame();
	virtual void processMyEvent(int code);
};

#endif // __DEMOTRACE_H__
