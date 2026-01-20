#include <BBox2D.h>
#include <DemoSystem/IBaseDemo.h>
#include <DemoSystem/SDLDemoRenderer.h>
#include <Polygon2D.h>
#include <Vec2.h>

#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_ttf.h>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_ttf.lib")

SDLDemoRenderer::SDLDemoRenderer() {
	font = 0;
	for (int i = 0; i < MAX_MOUSE_BUTTONS; i++) {
		mouseButtonState[i] = false;
	}
}
void SDLDemoRenderer::setDemo(IBaseDemo* demo) {
	this->demo = demo;
}
void SDLDemoRenderer::createWindow() {
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Create a window
	window = SDL_CreateWindow("SDL Rectangle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
							  800, 600, SDL_WINDOW_SHOWN);

	// Create a renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	/// SDL_RenderSetScale(renderer,3,3);

	if (TTF_Init() < 0) {
		printf("TTF_Init failed\n");
	}
	// Load font
	font = TTF_OpenFont("fonts/open-sans/OpenSans-Regular.ttf", 15);
	if (!font) {
		printf("Font load failed\n");
	}
}
bool SDLDemoRenderer::isMouseButtonDown(int button) {
	return mouseButtonState[button];
}
int SDLDemoRenderer::drawText(int x, int y, const char* s, byte r, byte g, byte b) {
	SDL_Rect dest;
	SDL_Color foreground = {r, g, b};

	SDL_Surface* text_surf = TTF_RenderText_Solid(font, s, foreground);
	if (text_surf == 0) {
		return y;
	}
	SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, text_surf);
	if (text == 0) {
		SDL_FreeSurface(text_surf);
		return y;
	}
	dest.x = x;
	dest.y = y;
	dest.w = text_surf->w;
	dest.h = text_surf->h;
	SDL_RenderCopy(renderer, text, NULL, &dest);

	SDL_DestroyTexture(text);
	SDL_FreeSurface(text_surf);
	return y + dest.h;
}
void SDLDemoRenderer::setColor(byte r, byte g, byte b, byte a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}
void SDLDemoRenderer::drawLine(const Vec2D& a, const Vec2D& b, int width) {
	Vec2D v = (a - b);
	v.normalize();
	v = v.getPerpendicular();
	for (float i = -width; i < width; i += 1.0f) {
		Vec2D a2 = a + v * i;
		Vec2D b2 = b + v * i;
		drawLine(a2, b2);
	}
}
void SDLDemoRenderer::fillPoly(const class Polygon2D& p) {
	if (p.size() < 3) return;

	// specific valid triangulation for CONVEX polygons (fan)
	int num_vertices = p.size();
	int num_indices = (p.size() - 2) * 3;

	SDL_Vertex* verts = (SDL_Vertex*)alloca(num_vertices * sizeof(SDL_Vertex));
	int* indices = (int*)alloca(num_indices * sizeof(int));

	byte r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_Color col = {r, g, b, a};

	for (int i = 0; i < p.size(); i++) {
		const Vec2D& s = p[i];
		verts[i].position.x = s.getX();
		verts[i].position.y = s.getY();
		verts[i].color = col;
		verts[i].tex_coord.x = 0;
		verts[i].tex_coord.y = 0;
	}

	int idx = 0;
	for (int i = 0; i < p.size() - 2; i++) {
		indices[idx++] = 0;
		indices[idx++] = i + 1;
		indices[idx++] = i + 2;
	}

	SDL_RenderGeometry(renderer, NULL, verts, num_vertices, indices, num_indices);
}
void SDLDemoRenderer::drawBox(const class BBox2D& box) {
	for (int i = 0; i < 4; i++) {
		int ni = (i + 1) % 4;
		drawLine(box.getCorner(i), box.getCorner(ni));
	}
}
void SDLDemoRenderer::drawCircle(const class Vec2D& center, float radius) {
	float step = 5.0f;
	for (float a = 0; a < 360; a += step) {
		float ab = a + step;
		Vec2D da, db;
		da.fromDegs(a, radius);
		db.fromDegs(ab, radius);
		drawLine(center + da, center + db);
	}
}
void SDLDemoRenderer::drawLine(const Vec2D& a, const Vec2D& b) {
	SDL_RenderDrawLineF(renderer, a.getX(), a.getY(), b.getX(), b.getY());
}
bool SDLDemoRenderer::processEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			if (demo->onQuit()) {
				running = 0;
				return true;
			}
		} else if (event.type == SDL_MOUSEBUTTONDOWN) {
			mouseButtonState[event.button.button] = true;
			demo->onMouseEvent(event.button.x, event.button.y, event.button.button, true);
		} else if (event.type == SDL_MOUSEBUTTONUP) {
			mouseButtonState[event.button.button] = false;
			demo->onMouseEvent(event.button.x, event.button.y, event.button.button, false);
		} else if (event.type == SDL_KEYDOWN) {
			demo->onKeyEvent(event.key.keysym.sym, true);
		} else if (event.type == SDL_KEYUP) {
			demo->onKeyEvent(event.key.keysym.sym, false);
		} else if (event.type == SDL_MOUSEMOTION) {
			demo->onMouseMoveEvent(event.motion.x, event.motion.y, event.motion.xrel,
								   event.motion.yrel);
		}
	}
	return false;
}
void SDLDemoRenderer::shutdown() {
	// Cleanup and quit SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void SDLDemoRenderer::beginFrame(byte r, byte g, byte b, byte a) {
	// Set the background color to green
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	// Clear the renderer with the background color
	SDL_RenderClear(renderer);
}
void SDLDemoRenderer::endFrame() {
	// Update the window
	SDL_RenderPresent(renderer);

	SDL_Delay(10);	// Add a small delay to control the frame rate
}
