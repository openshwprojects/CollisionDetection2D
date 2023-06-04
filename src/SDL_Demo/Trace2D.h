#ifndef __TRACE2D_H__
#define __TRACE2D_H__

#include <Vec2.h>

class CTrace2D {
	bool bStartSolid;
	bool bAllSolid;
	Vec2D start, end;
	Vec2D dir;
	float length;
public:
	void setup(const Vec2D &a, const Vec2D &b) {
		this->start = a;
		this->end = b;
		this->dir = b - a;
		this->length = this->dir.normalize();
	}
};

#endif // __TRACE2D_H__

