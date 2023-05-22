#ifndef __LINE2D_H__
#define __LINE2D_H__

#include <Vec2.h>

class Line2D {
	Vec2D a, b;
public:

	void set(const Vec2D &pa, const Vec2D &pb) {
		this->a = pa;
		this->b = pb;
	}
	const Vec2D &getA() const {
		return a;
	}
	const Vec2D &getB() const {
		return b;
	}
};

#endif

