

#include "DemoCollision.h"

#include <BBox2D.h>
#include <DemoSystem/DemoContainer.h>
#include <DemoSystem/IDemoRenderer.h>
#include <Trace2D.h>

CDemoCollision::CDemoCollision() {
	draggingIndex = -1;
}

void CDemoCollision::onDemoInit() {
	hulls.clear();
	hulls.addPoly(Vec2D(200, 100), Vec2D(500, 100), Vec2D(500, 50), Vec2D(200, 50), true);
	hulls.addPoly(Vec2D(200, 200), Vec2D(300, 200), Vec2D(300, 100), Vec2D(200, 100), true);
	hulls.addPoly(Vec2D(200, 200), Vec2D(300, 200), Vec2D(250, 300), true);

	// New shapes
	// Vertical wall on the right
	hulls.addPoly(Vec2D(600, 50), Vec2D(650, 50), Vec2D(650, 350), Vec2D(600, 350), true);

	// Triangle obstacle on the left
	hulls.addPoly(Vec2D(50, 250), Vec2D(150, 350), Vec2D(50, 350), true);

	// Angled box in the middle-bottom
	hulls.addPoly(Vec2D(400, 300), Vec2D(500, 250), Vec2D(550, 350), Vec2D(450, 400), true);

	// Sample Circle
	hulls.addCircle(Vec2D(100, 100), 40.0f);
	// Second Circle (Larger)
	hulls.addCircle(Vec2D(400, 200), 60.0f);

	// Sample Capsules
	hulls.addCapsule(Vec2D(350, 450), Vec2D(550, 450), 25.0f);
	hulls.addCapsule(Vec2D(100, 400), Vec2D(100, 500), 20.0f);
}

void CDemoCollision::processMyEvent(int code) {
	// if(code == 101) {
	//	curType++;
	//	if(curType > TT_BOX)
	//		curType = TT_POINT;
	// }
}
const char* CDemoCollision::getName() const {
	return "Simple collision";
}

void CDemoCollision::onMouseMoveEvent(int x, int y, int dX, int dY) {
	if (draggingIndex != -1) {
		hulls[draggingIndex]->translate(Vec2D(dX, dY));
	}
}
void CDemoCollision::onMouseEvent(int x, int y, int button, bool bDown) {
	if (button == 1) {
		if (bDown) {
			draggingIndex = hulls.findByPoint(Vec2D(x, y));
			printf("Chosen: %i\n", draggingIndex);
		} else {
			draggingIndex = -1;
		}
	} else if (button == 3) {
		if (bDown) {
			int which = hulls.findByPoint(Vec2D(x, y));
			if (which != -1) {
				hulls[which]->rotateCenterRadians(DEG2RAD(45.0f));
			}
		}
	}
}

bool CDemoCollision::onQuit() {
	return true;
}

void CDemoCollision::runFrame() {
	CTrace2D tr;

	r->beginFrame(150, 150, 255);
	container->drawDebugTexts();

	container->drawDebugText("Simple polygon collision.");
	container->drawDebugText("LMB: Drag, RMB: Rotate");

	std::vector<CCollision> collisions;

	for (int i = 0; i < hulls.size(); i++) {
		Shape2D* h = hulls[i];
		bool bHasCollision = false;
		for (int j = 0; j < hulls.size(); j++) {
			Shape2D* hi = hulls[j];
			if (i == j) {
				continue;
			}
			if (hi->getBB().intersectsBox(h->getBB())) {
				CCollision col;
				if (h->intersects(hi, &col)) {
					bHasCollision = true;
					collisions.push_back(col);

					// Draw Shadows (Moved out positions)
					// Only draw once per pair to avoid duplicates
					if (j > i) {
						Vec2D sep = col.normal * col.distance;

						// Shadow Color
						byte sR = 0, sG = 255, sB = 255;  // Cyan

						// Shadow A (this) - Moves AGAINST normal
						Vec2D offA = sep * -0.5f;
						if (h->getType() == ST_HULL) {
							Polygon2D poly = static_cast<Hull2D*>(h)->getPoly();
							poly.translate(offA);
							this->drawPoly(poly, sR, sG, sB, 0);
						} else if (h->getType() == ST_CIRCLE) {
							Circle2D* c = static_cast<Circle2D*>(h);
							r->setColor(sR, sG, sB);
							r->drawCircle(c->getCenter() + offA, c->getRadius());
						} else if (h->getType() == ST_CAPSULE) {
							Capsule2D* cap = static_cast<Capsule2D*>(h);
							r->setColor(sR, sG, sB);
							Vec2D ga = cap->getPointA() + offA;
							Vec2D gb = cap->getPointB() + offA;
							Vec2D gdir = gb - ga;
							gdir.normalize();
							Vec2D gperp = gdir.getPerpendicular() * cap->getRadius();
							r->drawLine(ga + gperp, gb + gperp);
							r->drawLine(ga - gperp, gb - gperp);
							float gBaseAngle = gdir.getAngleDeg();
							float gstep = 10.0f;
							for (float ang = 90; ang < 270; ang += gstep) {
								Vec2D da, db;
								da.fromDegs(gBaseAngle + ang, cap->getRadius());
								db.fromDegs(gBaseAngle + ang + gstep, cap->getRadius());
								r->drawLine(ga + da, ga + db);
							}
							for (float ang = -90; ang < 90; ang += gstep) {
								Vec2D da, db;
								da.fromDegs(gBaseAngle + ang, cap->getRadius());
								db.fromDegs(gBaseAngle + ang + gstep, cap->getRadius());
								r->drawLine(gb + da, gb + db);
							}
						}

						// Shadow B (other) - Moves ALONG normal
						Vec2D offB = sep * 0.5f;
						if (hi->getType() == ST_HULL) {
							Polygon2D poly = static_cast<Hull2D*>(hi)->getPoly();
							poly.translate(offB);
							this->drawPoly(poly, sR, sG, sB, 0);
						} else if (hi->getType() == ST_CIRCLE) {
							Circle2D* c = static_cast<Circle2D*>(hi);
							r->setColor(sR, sG, sB);
							r->drawCircle(c->getCenter() + offB, c->getRadius());
						} else if (hi->getType() == ST_CAPSULE) {
							Capsule2D* cap = static_cast<Capsule2D*>(hi);
							r->setColor(sR, sG, sB);
							Vec2D ga = cap->getPointA() + offB;
							Vec2D gb = cap->getPointB() + offB;
							Vec2D gdir = gb - ga;
							gdir.normalize();
							Vec2D gperp = gdir.getPerpendicular() * cap->getRadius();
							r->drawLine(ga + gperp, gb + gperp);
							r->drawLine(ga - gperp, gb - gperp);
							float gBaseAngle = gdir.getAngleDeg();
							float gstep = 10.0f;
							for (float ang = 90; ang < 270; ang += gstep) {
								Vec2D da, db;
								da.fromDegs(gBaseAngle + ang, cap->getRadius());
								db.fromDegs(gBaseAngle + ang + gstep, cap->getRadius());
								r->drawLine(ga + da, ga + db);
							}
							for (float ang = -90; ang < 90; ang += gstep) {
								Vec2D da, db;
								da.fromDegs(gBaseAngle + ang, cap->getRadius());
								db.fromDegs(gBaseAngle + ang + gstep, cap->getRadius());
								r->drawLine(gb + da, gb + db);
							}
						}
					}
				}
			}
		}

		int rCol = 0;
		int gCol = 250;
		if (bHasCollision) {
			rCol = 250;
			gCol = 0;
		}

		if (h->getType() == ST_HULL) {
			Hull2D* hull = static_cast<Hull2D*>(h);
			this->drawPoly(hull->getPoly(), rCol, gCol, 0);
			// Draw Normals
			const PlaneSet2D& planes = hull->getPlanes();
			r->setColor(255, 255, 0);  // Yellow normals
			for (int k = 0; k < planes.size(); k++) {
				const Plane2D& pl = planes[k];
				Vec2D cent;
				// Estimate center of edge?
				// Requires vertices.
				// But we can just use plane info.
				// Or use polygon points.
				const Polygon2D& poly = hull->getPoly();
				if (k < poly.size()) {
					Vec2D p1 = poly[k];
					Vec2D p2 = poly[(k + 1) % poly.size()];
					cent = (p1 + p2) * 0.5f;
					r->drawLine(cent, cent + pl.getNormal() * 10.0f);
				}
			}
		} else if (h->getType() == ST_CIRCLE) {
			Circle2D* c = static_cast<Circle2D*>(h);
			r->setColor(rCol, gCol, 0);
			r->drawCircle(c->getCenter(), c->getRadius());
		} else if (h->getType() == ST_CAPSULE) {
			Capsule2D* cap = static_cast<Capsule2D*>(h);
			r->setColor(rCol, gCol, 0);
			// Draw capsule: two semicircles at endpoints + two connecting lines
			Vec2D a = cap->getPointA();
			Vec2D b = cap->getPointB();
			Vec2D dir = b - a;
			dir.normalize();
			Vec2D perp = dir.getPerpendicular() * cap->getRadius();
			r->drawLine(a + perp, b + perp);
			r->drawLine(a - perp, b - perp);
			// Semicircles at each end
			float baseAngle = dir.getAngleDeg();
			float step = 5.0f;
			for (float ang = 90; ang < 270; ang += step) {
				Vec2D da, db;
				da.fromDegs(baseAngle + ang, cap->getRadius());
				db.fromDegs(baseAngle + ang + step, cap->getRadius());
				r->drawLine(a + da, a + db);
			}
			for (float ang = -90; ang < 90; ang += step) {
				Vec2D da, db;
				da.fromDegs(baseAngle + ang, cap->getRadius());
				db.fromDegs(baseAngle + ang + step, cap->getRadius());
				r->drawLine(b + da, b + db);
			}
		}
	}

	// Draw collisions
	for (size_t i = 0; i < collisions.size(); i++) {
		const CCollision& col = collisions[i];
		r->setColor(255, 0, 255);  // Purple for contact info
		for (int p = 0; p < col.numPoints; p++) {
			r->drawCircle(col.points[p], 3.0f);
			r->drawLine(col.points[p],
						col.points[p] + col.normal * col.distance * 10.0f);	 // Scale up normal
		}
	}

	r->endFrame();
}
