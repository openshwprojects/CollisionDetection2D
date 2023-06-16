#ifndef __HULL2D_LIST_H__
#define __HULL2D_LIST_H__

#include <Hull2D.h>

class Hull2DList {
	Array<Hull2D> hulls;
public:

	int size() const {
		return hulls.size();
	}
	Hull2D &addHull2D(const Hull2D &h) {
		int ri = hulls.size();
		hulls.push_back(h);
		return hulls[ri];
	}
	Hull2D &addPoly(const Vec2D &a, const Vec2D &b, const Vec2D&c, bool bSort = false) {
		Hull2D h;
		h.fromPoly(a,b,c,bSort);
		return addHull2D(h);
	}
	Hull2D &addPoly(const Vec2D &a, const Vec2D &b, const Vec2D&c, const Vec2D&d, bool bSort = false) {
		Hull2D h;
		h.fromPoly(a,b,c,d,bSort);
		return addHull2D(h);
	}
	Hull2D &addPoly(const Array<Vec2D> &ar, bool bSort = false) {
		Hull2D h;
		h.fromPoly(ar,bSort);
		return addHull2D(h);
	}
	const Hull2D&operator[](int i) const {
		return hulls[i];
	}
	Hull2D&operator[](int i) {
		return hulls[i];
	}
	bool trace(class CTrace2D &tr) const;
	int findByPoint(const Vec2D &p) const;
};
#endif // __HULL2D_LIST_H__
