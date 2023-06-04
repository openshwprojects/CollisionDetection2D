#ifndef __IDEMORENDERER_H__
#define __IDEMORENDERER_H__

class IDemoRenderer { 
public:
	virtual void setColor(byte r, byte g, byte b, byte a = 255) = 0;
	virtual void drawLine(const class Vec2D &a, const class Vec2D &b) = 0;
	virtual void drawLine(const class Vec2D &a, const class Vec2D &b, int width) = 0;
	virtual void beginFrame(byte r, byte g, byte b, byte a = 255) = 0;
	virtual int drawText(int x, int y, const char *s, byte r, byte g, byte b) = 0;
	virtual void endFrame() = 0;
};

#endif // __IDEMORENDERER_H__
