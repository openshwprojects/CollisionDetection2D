#include "Hull2DList.h"
#include "Trace2D.h"

bool Hull2DList::trace(class CTrace2D &tr) const {
	for(int i = 0; i < size(); i++) {
		hulls[i].trace(tr);
	}
	return tr.hasHit();
}

int Hull2DList::findByPoint(const Vec2D &p) const {
	for(int i = 0; i < size(); i++) {
		if(hulls[i].isInside(p)) {
			return i;
		}
	}
	return -1;
}