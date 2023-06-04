#include "DemoContainer.h"
#include <Demos/DemoSortVertices.h>
#include "IDemoRenderer.h"
#include <stdarg.h>

enum {
	MY_EVENT_FIRST = 10,
	MY_EVENT_NEXT_DEMO,
	MY_EVENT_PREV_DEMO,
};
CDemoContainer::CDemoContainer() {
    settingsKeys = "qwertyuiopasdfghjklzxcvbnm";
    nextSettingsKey = 0;
    current = 0;
    addDemo(new CDemoSortVertices());
}

void CDemoContainer::addDemo(CBaseDemo* d) {
    demos.push_back(d);
	d->setContainer(this);
    d->setRenderer(r);
	settings.clear();
	addSetting(MY_EVENT_NEXT_DEMO,"",'q');
	addSetting(MY_EVENT_PREV_DEMO,"",'w');
	d->onDemoInit();
}

void CDemoContainer::resetDebugText() {
    textH = 30;
}

void CDemoContainer::drawDebugTexts() {
    resetDebugText();
	drawDebugText("Demo %i/%i, use Q and W to change", current,demos.size());
    drawDebugText(demos[current]->getName());
    for (int i = 0; i < settings.size(); i++) {
        const SDemoSetting& s = settings[i];
		if(s.name.size()) {
			if(s.boolean) {
				drawDebugText(" - %s = %i - press %c to change", s.name.c_str(), *s.boolean, s.key);
			} else {
				drawDebugText(" - %s = press %c", s.name.c_str(), s.key);
			}
		}
    }
}

void CDemoContainer::processMyEvent(int code) {
	if(code == MY_EVENT_NEXT_DEMO) {
		printf("nxt");
	} else if(code == MY_EVENT_PREV_DEMO) {
		printf("prv");
	}
	demos[current]->processMyEvent(code);
}
void CDemoContainer::onKeyEvent(int key, bool bDown) {
    if (bDown) {
        for (int i = 0; i < settings.size(); i++) {
            SDemoSetting& s = settings[i];
            if (s.key == key) {
				if(s.boolean) {
					*s.boolean = !*s.boolean;
				}
				if(s.eventIndex) {
					processMyEvent(s.eventIndex);
				}
            }
        }
    }
    demos[current]->onKeyEvent(key, bDown);
}

void CDemoContainer::drawDebugText(const char* s, ...) {
    char buffer[256];
    va_list args;
    va_start(args, s);
    vsprintf(buffer, s, args);
    va_end(args);

    textH = r->drawText(20, textH, buffer, 255, 250, 250);
}
bool CDemoContainer::isKeyInUse(int key) const {
	for(int i = 0; i < settings.size(); i++) {
		if(settings[i].key == key)
			return true;
	}
	return false;
}
void CDemoContainer::prepareSettingKey(int *key) {
    if (*key == -1) {
		do {
			*key = settingsKeys[nextSettingsKey];
			nextSettingsKey++;
		} while(isKeyInUse(*key));
    }
}

void CDemoContainer::addSetting(int targetEvent, const char* name, int key) {
    prepareSettingKey(&key);
    SDemoSetting set;
    set.boolean = 0;
	set.eventIndex = targetEvent;
    set.key = key;
    set.name = name;
    settings.push_back(set);
}
void CDemoContainer::addSetting(bool* targetBool, const char* name, int key) {
    prepareSettingKey(&key);
    SDemoSetting set;
    set.boolean = targetBool;
	set.eventIndex = 0;
    set.key = key;
    set.name = name;
    settings.push_back(set);
}

void CDemoContainer::runDemo(IBaseDemo* d) {

}

const char* CDemoContainer::getName() const {
    return "";
}

void CDemoContainer::setRenderer(IDemoRenderer* ren) {
    for (int i = 0; i < demos.size(); i++) {
        demos[i]->setRenderer(ren);
    }
    IBaseDemo::setRenderer(ren);
}

void CDemoContainer::onMouseEvent(int x, int y, int button, bool bDown) {
    demos[current]->onMouseEvent(x, y, button, bDown);
}



bool CDemoContainer::onQuit() {
    return demos[current]->onQuit();
}

void CDemoContainer::runFrame() {
    demos[current]->runFrame();
}
