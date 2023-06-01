#include <Demos/SDLDemoRenderer.h>
#include <Vec2.h>
#include <Demos/IBaseDemo.h>

#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_ttf.h>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_ttf.lib")

SDLDemoRenderer::SDLDemoRenderer() {
	font = 0;
}
void SDLDemoRenderer::drawText() {

}
void SDLDemoRenderer::setDemo(IBaseDemo *demo) {
	this->demo = demo;
}
void SDLDemoRenderer::createWindow() {
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Create a window
	window = SDL_CreateWindow("SDL Rectangle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

	// Create a renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	///SDL_RenderSetScale(renderer,3,3);

	if ( TTF_Init() < 0 ) {
		printf("TTF_Init failed\n");
	}
	// Load font
	font = TTF_OpenFont("fonts/open-sans/OpenSans-Regular.ttf", 15);
	if ( !font ) {
		printf("Font load failed\n");
	}

}
int SDLDemoRenderer::drawText(int x, int y, const char *s, byte r, byte g, byte b) {

	SDL_Rect dest;
	SDL_Color foreground = { r, g, b };

	SDL_Surface* text_surf = TTF_RenderText_Solid(font, s, foreground);
	if(text_surf == 0) {
		return y;
	}
	SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, text_surf);
	if(text == 0) {
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
void SDLDemoRenderer::drawLine(const Vec2D &a, const Vec2D &b, int width) {
	Vec2D v = (a-b);
	v.normalize();
	v = v.getPerpendicular();
	for(float i = -width; i < width; i+=1.0f) {
		Vec2D a2 = a + v * i;
		Vec2D b2 = b + v * i;
		drawLine(a2,b2);
	}
}
void SDLDemoRenderer::drawLine(const Vec2D &a, const Vec2D &b) {
	SDL_RenderDrawLineF(renderer, a.getX(),a.getY(),b.getX(),b.getY());
}
bool SDLDemoRenderer::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
			if(demo->onQuit()) {
				running = 0;
				return true;
			}
		} else if (event.type == SDL_MOUSEBUTTONDOWN) {
			demo->onMouseEvent(event.button.x,event.button.y,event.button.button, true);
		} else if (event.type == SDL_MOUSEBUTTONUP) {
			demo->onMouseEvent(event.button.x,event.button.y,event.button.button, false);
		} else if(event.type == SDL_KEYDOWN) {
			demo->onKeyEvent(event.key.keysym.sym, true);
		} else if(event.type == SDL_KEYUP) {
			demo->onKeyEvent(event.key.keysym.sym, false);
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

    SDL_Delay(10);  // Add a small delay to control the frame rate
}

