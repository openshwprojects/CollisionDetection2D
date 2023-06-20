#ifndef __IDEMORENDERER_H__
#define __IDEMORENDERER_H__

class IDemoRenderer { 
public:
	virtual void setColor(byte r, byte g, byte b, byte a = 255) = 0;
	virtual void drawLine(const class Vec2D &a, const class Vec2D &b) = 0;
	virtual void drawLine(const class Vec2D &a, const class Vec2D &b, int width) = 0;
	virtual void drawCircle(const class Vec2D &a, float rad) = 0;
	virtual void drawBox(const class BBox2D &box) = 0;
	virtual void fillPoly(const class Polygon2D& p) = 0;
	virtual void beginFrame(byte r, byte g, byte b, byte a = 255) = 0;
	virtual int drawText(int x, int y, const char *s, byte r, byte g, byte b) = 0;
	virtual void endFrame() = 0;
	// TODO: split into another interface
	virtual bool isMouseButtonDown(int button) = 0;
};

#endif // __IDEMORENDERER_H__
