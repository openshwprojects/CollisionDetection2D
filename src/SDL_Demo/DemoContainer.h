#ifndef CDEMO_CONTAINER_H
#define CDEMO_CONTAINER_H

struct SDemoSetting {
    bool* boolean;
    int key;
    std::string name;
	int eventIndex;
};

class CDemoContainer : public IBaseDemo {
    std::vector<class IBaseDemo*> demos;
    int current;
    std::vector<SDemoSetting> settings;
    int nextSettingsKey;
    const char* settingsKeys;
    int textH;

    void addDemo(class CBaseDemo* d);
	bool isKeyInUse(int key) const;
	void prepareSettingKey(int *key);
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
