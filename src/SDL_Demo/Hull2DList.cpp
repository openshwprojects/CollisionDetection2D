#include "Hull2DList.h"
#include "Trace2D.h"

bool Hull2DList::trace(class CTrace2D &tr) const {
	for(int i = 0; i < size(); i++) {
		hulls[i].trace(tr);
	}
	return tr.hasHit();
}
