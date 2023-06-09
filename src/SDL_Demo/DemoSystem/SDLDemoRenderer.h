#ifndef __SDLDEMORENDERER_H__
#define __SDLDEMORENDERER_H__

#include <Common.h>
#include <DemoSystem/IDemoRenderer.h>

typedef struct _TTF_Font TTF_Font;

#define MAX_MOUSE_BUTTONS		10

class SDLDemoRenderer : public IDemoRenderer {
	struct SDL_Renderer* renderer;
    struct SDL_Window* window;
	class IBaseDemo *demo ;
	int running;
	TTF_Font* font;
	bool mouseButtonState[MAX_MOUSE_BUTTONS];
public:
	SDLDemoRenderer();
	void setDemo(IBaseDemo *demo);
	void createWindow();
	virtual bool isMouseButtonDown(int button);
	virtual int drawText(int x, int y, const char *s, byte r, byte g, byte b);
	virtual void setColor(byte r, byte g, byte b, byte a = 255) ;
	virtual void drawLine(const Vec2D &a, const Vec2D &b, int width);
	virtual void drawLine(const Vec2D &a, const Vec2D &b);
	virtual void drawCircle(const class Vec2D &a, float rad);
	virtual void drawBox(const class BBox2D &box);
	virtual void fillPoly(const class Polygon2D& p);
	virtual bool processEvents();
	virtual void shutdown();
	virtual void beginFrame(byte r, byte g, byte b, byte a);
	virtual void endFrame();
};

#endif // __SDLDEMORENDERER_H__
