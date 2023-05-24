#include "PlaneSet2D.h"
#include "Polygon2D.h"

Vec2D center;
int compareVecs(const void *a, const void *b) {
	const Vec2D *va = (const Vec2D*)a;
	const Vec2D *vb = (const Vec2D*)b;
	Vec2D la = (*va) - center;
	Vec2D lb = (*vb) - center;
	float aa = la.getAngleRad();
	float ab = lb.getAngleRad();
	if(aa == ab){
		return 0;
	}
	if(aa>ab){
		return 1;
	}
	return -1;
}
void SortPolygon(Array<Vec2D> &pl) {

	center = pl[0];
	for(int i = 1; i < pl.size(); i++) {
		center += pl[i];
	}
	center /= pl.size();
	qsort(pl.getArray(),pl.size(),pl.elementSize(),compareVecs);
}
bool PlaneSet2D::fromPoly(const class Array<Vec2D> &pl, bool bSort) {
	planes.clear();
	if(pl.size() < 3) {
		return false;
	}
	if(bSort) {
		Array<Vec2D> np = pl;
		SortPolygon(np);
		return fromPoly(np,false);
	}
	for(int i = 0; i < pl.size(); i++) {
		int n = (i+1)%pl.size();
		const Vec2D &a = pl[i];
		const Vec2D &b = pl[n];
		Plane2D pl;
		pl.fromTwoPoints(a,b);
		this->addPlane(pl);
	}
	return false;
}



