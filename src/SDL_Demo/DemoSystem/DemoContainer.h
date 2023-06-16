#ifndef CDEMO_CONTAINER_H
#define CDEMO_CONTAINER_H

#include <string.h>
#include <Common.h>
#include <DemoSystem/IBaseDemo.h>

struct SDemoSetting {
    bool* boolean;
	float *pfloat;
    int key;
    int keyU;
    int keyD;
    std::string name;
	int eventIndex;
	float step;
	float min;
	float max;
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
    virtual void addSetting(float *value, float step, float min, float max, const char* name, int keyU = -1, int keyD = -1);
    void runDemo(IBaseDemo* d);
    virtual const char* getName() const;
    virtual void setRenderer(IDemoRenderer* ren);
    virtual void onMouseEvent(int x, int y, int button, bool bDown);
	virtual void onMouseMoveEvent(int x, int y, int dX, int dY);
    virtual bool onQuit() override;
    virtual void runFrame() override;
};

#endif  // CDEMO_CONTAINER_H
