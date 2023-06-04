
#include <DemoSystem/DemoContainer.h>
#include <DemoSystem/IDemoRenderer.h>
#include <DemoSystem/SDLDemoRenderer.h>

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
