#ifndef __SHAPELIST_H__
#define __SHAPELIST_H__

#include "Circle2D.h"
#include "Common.h"
#include "Hull2D.h"
#include "Shape2D.h"


// Replaces Hull2DList
class ShapeList {
	Array<Shape2D*> shapes;

public:
	~ShapeList() {
		clear();
	}

	int size() const {
		return shapes.size();
	}

	void clear() {
		for (int i = 0; i < shapes.size(); i++) {
			delete shapes[i];
		}
		shapes.clear();
	}

	// Add existing shape (takes ownership)
	void addShape(Shape2D* s) {
		shapes.push_back(s);
	}

	Hull2D& addPoly(const Vec2D& a, const Vec2D& b, const Vec2D& c, bool bSort = false) {
		Hull2D* h = new Hull2D();
		h->fromPoly(a, b, c, bSort);
		addShape(h);
		return *h;
	}
	Hull2D& addPoly(const Vec2D& a, const Vec2D& b, const Vec2D& c, const Vec2D& d,
					bool bSort = false) {
		Hull2D* h = new Hull2D();
		h->fromPoly(a, b, c, d, bSort);
		addShape(h);
		return *h;
	}
	Hull2D& addPoly(const Array<Vec2D>& ar, bool bSort = false) {
		Hull2D* h = new Hull2D();
		h->fromPoly(ar, bSort);
		addShape(h);
		return *h;
	}

	Circle2D& addCircle(const Vec2D& center, float radius) {
		Circle2D* c = new Circle2D(center, radius);
		addShape(c);
		return *c;
	}

	const Shape2D* operator[](int i) const {
		return shapes[i];
	}
	Shape2D* operator[](int i) {
		return shapes[i];
	}

	bool trace(class CTrace2D& tr) const {
		bool bHit = false;
		for (int i = 0; i < shapes.size(); i++) {
			// Basic checking bbox before detailed trace?
			if (shapes[i]->getBB().intersectsBox(tr.getBox())) {  // Optimization
				// This assumes trace box is the full sweep bounds.
				// Trace2D has 'box' which is the shape being swept, usually centered at start?
				// Actually Trace2D doesn't readily expose a "swept bounds" unless calculate it.
				// Let's rely on individual shape trace checking bbox.
			}
			if (shapes[i]->trace(tr)) {
				bHit = true;
			}
		}
		return bHit;
	}

	int findByPoint(const Vec2D& p) const {
		for (int i = 0; i < shapes.size(); i++) {
			// For generic shapes, we might check BB or specific strict check.
			// Hull2D has isInside. Shape2D doesn't have isInside (only trace/intersects).
			// Let's check BB for now or cast?
			// Simplest: Check traces with a point? Or add isInside to Shape2D.

			// For now, let's use BB as a coarse check
			if (shapes[i]->getBB().isInside(p)) {
				// Refined check
				if (shapes[i]->getType() == ST_HULL) {
					if (static_cast<Hull2D*>(shapes[i])->isInside(p)) return i;
				} else if (shapes[i]->getType() == ST_CIRCLE) {
					const Circle2D* c = static_cast<Circle2D*>(shapes[i]);
					if (c->getCenter().distanceToSq(p) <= c->getRadius() * c->getRadius()) return i;
				}
			}
		}
		return -1;
	}
};

#endif	// __SHAPELIST_H__
