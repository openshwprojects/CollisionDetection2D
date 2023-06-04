#ifndef CDEMO_CONTAINER_H
#define CDEMO_CONTAINER_H

#include <string.h>
#include <Common.h>
#include <DemoSystem/IBaseDemo.h>

struct SDemoSetting {
    bool* boolean;
    int key;
    std::string name;
	int eventIndex;
};

class CDemoContainer : public IBaseDemo {
	Array<class CBaseDemo*> demos;
    int current;
    std::vector<SDemoSetting> settings;
    int nextSettingsKey;
    const char* settingsKeys;
    int textH;

	void initDemo();
    void addDemo(class CBaseDemo* d);
	bool isKeyInUse(int key) const;
	void prepareSettingKey(int *key);
	void scrollDemos(int ofs);
public:
    CDemoContainer();
    void resetDebugText();
    void drawDebugTexts();
	virtual void processMyEvent(int code);
    virtual void onKeyEvent(int key, bool bDown);
    void drawDebugText(const char* s, ...);
    virtual void addSetting(bool* targetBool, const char* name, int key);
    virtual void addSetting(int targetEvent, const char* name, int key);
    void runDemo(IBaseDemo* d);
    virtual const char* getName() const;
    virtual void setRenderer(IDemoRenderer* ren);
    virtual void onMouseEvent(int x, int y, int button, bool bDown);
    virtual bool onQuit() override;
    virtual void runFrame() override;
};

#endif  // CDEMO_CONTAINER_H
