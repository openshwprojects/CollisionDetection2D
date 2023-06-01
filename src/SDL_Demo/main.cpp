
#include <Demos/DemoContainer.h>
#include <Demos/IDemoRenderer.h>
#include <Demos/SDLDemoRenderer.h>

void doBasicSelfTests();

int main() {

	doBasicSelfTests();


	CDemoContainer *demo = new CDemoContainer();
	SDLDemoRenderer *ren = new SDLDemoRenderer();
	demo->setRenderer(ren);
	ren->createWindow();
	ren->setDemo(demo);
	while(!ren->processEvents()) {
		demo->runFrame();
	}
	ren->shutdown();

	return 0;
}
