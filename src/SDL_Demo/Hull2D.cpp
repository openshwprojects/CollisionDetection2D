#include "Hull2D.h"


void Hull2D::fromPoly(const Vec2D &a, const Vec2D &b, const Vec2D&c, const Vec2D&d, bool bSort) {
	planes.fromPoly(a,b,c,d,bSort);
	vertices.fromPlanes(planes);
}
void Hull2D::fromPoly(const Vec2D &a, const Vec2D &b, const Vec2D&c, bool bSort) {
	planes.fromPoly(a,b,c,bSort);
	vertices.fromPlanes(planes);
}
void Hull2D::fromPoly(const Array<Vec2D> &poly, bool bSort) {
	planes.fromPoly(poly,bSort);
	vertices.fromPlanes(planes);

}