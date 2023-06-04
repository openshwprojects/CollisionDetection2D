#ifndef CBASE_DEMO_H
#define CBASE_DEMO_H

#include <DemoSystem/IBaseDemo.h>
#include <Polygon2D.h>

class CBaseDemo : public IBaseDemo {
protected:
	class CDemoContainer* container;

public:
    void setContainer(CDemoContainer* cc);
    virtual void processMyEvent(int code);
    virtual void onDemoInit();
    virtual const char* getName() const;
    void drawPoly(const Polygon2D& p, byte r, byte g, byte b, byte a = 255);
    virtual void onKeyEvent(int key, bool bDown);
};

#endif  // CBASE_DEMO_H

