#ifndef __HULL2D_LIST_H__
#define __HULL2D_LIST_H__

#include <Hull2D.h>

class Hull2DList {
	Array<Hull2D> hulls;
public:

	int size() const {
		return hulls.size();
	}
	void addPoly(const Vec2D &a, const Vec2D &b, const Vec2D&c, bool bSort = false) {
		Hull2D h;
		h.fromPoly(a,b,c,bSort);
		hulls.push_back(h);
	}
	void addPoly(const Vec2D &a, const Vec2D &b, const Vec2D&c, const Vec2D&d, bool bSort = false) {
		Hull2D h;
		h.fromPoly(a,b,c,d,bSort);
		hulls.push_back(h);
	}
	void addPoly(const Array<Vec2D> &ar, bool bSort = false) {
		Hull2D h;
		h.fromPoly(ar,bSort);
		hulls.push_back(h);
	}
	const Hull2D&operator[](int i) const {
		return hulls[i];
	}
	Hull2D&operator[](int i) {
		return hulls[i];
	}
};
#endif // __HULL2D_LIST_H__
