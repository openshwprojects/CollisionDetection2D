#ifndef __IBASEDEMO_H__
#define __IBASEDEMO_H__

class IBaseDemo {
protected:
	class IDemoRenderer *r;
public:
	virtual const char *getName() const = 0;
	virtual void onMouseEvent(int x, int y, int button, bool bDown) = 0;
	virtual void onKeyEvent(int key, bool bDown) = 0;
	virtual bool onQuit() = 0;
	virtual void runFrame() = 0;
	virtual void processMyEvent(int code) = 0;

	void setRenderer(IDemoRenderer *ren) {
		this->r = ren;
	}
};

#endif
