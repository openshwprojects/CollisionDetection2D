#include "TestSystem.h"

#include <BBox2D.h>
#include <Common.h>
#include <Plane2D.h>
#include <PlaneSet2D.h>
#include <Polygon2D.h>
#include <Vec2.h>

#include "Capsule2D.h"
#include "Circle2D.h"
#include "Hull2D.h"

// ---- Test runner globals ----
int g_testsPassed = 0;
int g_testsFailed = 0;
int g_testsChecksPassed = 0;
int g_testsChecksFailed = 0;
const char* g_currentTestName = NULL;
TestEntry g_testEntries[TESTSYSTEM_MAX_TESTS];
int g_testCount = 0;

void TestSystem_RunAll() {
	g_testsPassed = 0;
	g_testsFailed = 0;

	printf("=== Running %d tests ===\n", g_testCount);

	for (int i = 0; i < g_testCount; i++) {
		g_currentTestName = g_testEntries[i].name;
		int failsBefore = g_testsChecksFailed;

		g_testEntries[i].func();

		if (g_testsChecksFailed == failsBefore) {
			printf("[TEST] %s ... PASS\n", g_currentTestName);
			g_testsPassed++;
		} else {
			printf("[TEST] %s ... FAIL\n", g_currentTestName);
			g_testsFailed++;
		}
	}

	printf("=== Tests: %d passed, %d failed (%d checks total) ===\n", g_testsPassed, g_testsFailed,
		   g_testsChecksPassed + g_testsChecksFailed);
}

// ================================================================
// Circle vs Circle
// ================================================================

TEST(CircleCircle_Overlap) {
	Circle2D a(Vec2D(0, 0), 5.0f);
	Circle2D b(Vec2D(3, 0), 5.0f);
	CCollision col;
	TEST_CHECK(a.intersects(&b, &col) == true, "Overlapping circles must intersect");
	TEST_CHECK(col.distance > 0, "Penetration distance must be positive");
}

TEST(CircleCircle_NoOverlap) {
	Circle2D a(Vec2D(0, 0), 2.0f);
	Circle2D b(Vec2D(10, 0), 2.0f);
	TEST_CHECK(a.intersects(&b) == false, "Far-apart circles must not intersect");
}

TEST(CircleCircle_Touching) {
	Circle2D a(Vec2D(0, 0), 5.0f);
	Circle2D b(Vec2D(10, 0), 5.0f);
	// Edge-to-edge: distance == sum of radii, should count as intersecting (<=)
	TEST_CHECK(a.intersects(&b) == true, "Edge-to-edge circles should intersect");
}

// ================================================================
// Capsule vs Capsule
// ================================================================

TEST(CapsuleCapsule_Overlap) {
	Capsule2D a(Vec2D(0, 0), Vec2D(10, 0), 3.0f);
	Capsule2D b(Vec2D(5, 2), Vec2D(15, 2), 3.0f);
	CCollision col;
	TEST_CHECK(a.intersects(&b, &col) == true, "Overlapping capsules must intersect");
	TEST_CHECK(col.distance > 0, "Penetration distance must be positive");
}

TEST(CapsuleCapsule_NoOverlap) {
	Capsule2D a(Vec2D(0, 0), Vec2D(10, 0), 1.0f);
	Capsule2D b(Vec2D(0, 20), Vec2D(10, 20), 1.0f);
	TEST_CHECK(a.intersects(&b) == false, "Far-apart capsules must not intersect");
}

TEST(CapsuleCapsule_Parallel) {
	// Two parallel horizontal capsules, close together
	Capsule2D a(Vec2D(0, 0), Vec2D(10, 0), 2.0f);
	Capsule2D b(Vec2D(0, 3), Vec2D(10, 3), 2.0f);
	TEST_CHECK(a.intersects(&b) == true, "Close parallel capsules must intersect");
}

// ================================================================
// Capsule vs Circle
// ================================================================

TEST(CapsuleCircle_Overlap) {
	Capsule2D cap(Vec2D(0, 0), Vec2D(10, 0), 2.0f);
	Circle2D circ(Vec2D(5, 1), 2.0f);
	CCollision col;
	TEST_CHECK(cap.intersects(&circ, &col) == true, "Capsule touching circle must intersect");
	TEST_CHECK(col.distance > 0, "Penetration distance must be positive");
}

TEST(CapsuleCircle_NoOverlap) {
	Capsule2D cap(Vec2D(0, 0), Vec2D(10, 0), 1.0f);
	Circle2D circ(Vec2D(5, 20), 1.0f);
	TEST_CHECK(cap.intersects(&circ) == false, "Far-apart capsule and circle must not intersect");
}

// ================================================================
// Circle vs Hull
// ================================================================

TEST(CircleHull_Inside) {
	Hull2D hull;
	hull.fromPoly(Vec2D(0, 0), Vec2D(10, 0), Vec2D(10, 10), Vec2D(0, 10));
	Circle2D circ(Vec2D(5, 5), 1.0f);
	CCollision col;
	TEST_CHECK(circ.intersects(&hull, &col) == true, "Circle inside hull must intersect");
	TEST_CHECK(col.distance > 0, "Penetration distance must be positive");
}

TEST(CircleHull_NoOverlap) {
	Hull2D hull;
	hull.fromPoly(Vec2D(0, 0), Vec2D(10, 0), Vec2D(10, 10), Vec2D(0, 10));
	Circle2D circ(Vec2D(50, 50), 1.0f);
	TEST_CHECK(circ.intersects(&hull) == false, "Circle far from hull must not intersect");
}

// ================================================================
// Capsule vs Hull
// ================================================================

TEST(CapsuleHull_Overlap) {
	Hull2D hull;
	hull.fromPoly(Vec2D(0, 0), Vec2D(10, 0), Vec2D(10, 10), Vec2D(0, 10));
	Capsule2D cap(Vec2D(5, -1), Vec2D(5, 5), 2.0f);
	CCollision col;
	TEST_CHECK(cap.intersects(&hull, &col) == true, "Capsule overlapping hull must intersect");
}

TEST(CapsuleHull_NoOverlap) {
	Hull2D hull;
	hull.fromPoly(Vec2D(0, 0), Vec2D(10, 0), Vec2D(10, 10), Vec2D(0, 10));
	Capsule2D cap(Vec2D(50, 50), Vec2D(60, 50), 1.0f);
	TEST_CHECK(cap.intersects(&hull) == false, "Capsule far from hull must not intersect");
}

// ================================================================
// Vec2D math
// ================================================================

TEST(Vec2D_Length) {
	Vec2D v(3.0f, 4.0f);
	TEST_CHECK_FLOAT(v.length(), 5.0f, 0.001f, "3-4-5 triangle length");
	TEST_CHECK_FLOAT(v.lengthSq(), 25.0f, 0.001f, "3-4-5 triangle length squared");
}

TEST(Vec2D_Normalize) {
	Vec2D v(3.0f, 4.0f);
	float oldLen = v.normalize();
	TEST_CHECK_FLOAT(oldLen, 5.0f, 0.001f, "normalize returns old length");
	TEST_CHECK_FLOAT(v.length(), 1.0f, 0.001f, "after normalize, length is 1");
}

TEST(Vec2D_DotProduct) {
	Vec2D a(1.0f, 0.0f);
	Vec2D b(0.0f, 1.0f);
	TEST_CHECK_FLOAT(a.dot(b), 0.0f, 0.001f, "perpendicular dot is 0");
	TEST_CHECK_FLOAT(a.dot(a), 1.0f, 0.001f, "self dot of unit vec is 1");
	Vec2D c(-1.0f, 0.0f);
	TEST_CHECK_FLOAT(a.dot(c), -1.0f, 0.001f, "opposite dot is -1");
}

TEST(Vec2D_CrossProduct) {
	Vec2D a(1.0f, 0.0f);
	Vec2D b(0.0f, 1.0f);
	TEST_CHECK_FLOAT(a.cross(b), 1.0f, 0.001f, "cross of x,y unit vectors is 1");
	TEST_CHECK_FLOAT(b.cross(a), -1.0f, 0.001f, "cross is anti-commutative");
}

TEST(Vec2D_Perpendicular) {
	Vec2D v(1.0f, 0.0f);
	Vec2D p = v.getPerpendicular();
	TEST_CHECK_FLOAT(v.dot(p), 0.0f, 0.001f, "perpendicular vector has dot = 0");
}

TEST(Vec2D_Arithmetic) {
	Vec2D a(2.0f, 3.0f);
	Vec2D b(4.0f, 1.0f);
	Vec2D sum = a + b;
	TEST_CHECK_FLOAT(sum.getX(), 6.0f, 0.001f, "add X");
	TEST_CHECK_FLOAT(sum.getY(), 4.0f, 0.001f, "add Y");
	Vec2D diff = a - b;
	TEST_CHECK_FLOAT(diff.getX(), -2.0f, 0.001f, "sub X");
	TEST_CHECK_FLOAT(diff.getY(), 2.0f, 0.001f, "sub Y");
	Vec2D scaled = a * 3.0f;
	TEST_CHECK_FLOAT(scaled.getX(), 6.0f, 0.001f, "scale X");
	TEST_CHECK_FLOAT(scaled.getY(), 9.0f, 0.001f, "scale Y");
}

TEST(Vec2D_DistanceTo) {
	Vec2D a(0, 0);
	Vec2D b(3, 4);
	TEST_CHECK_FLOAT(a.distanceTo(b), 5.0f, 0.001f, "distance 3-4-5");
	TEST_CHECK_FLOAT(a.distanceToSq(b), 25.0f, 0.001f, "distanceSq 3-4-5");
}

TEST(Vec2D_Lerp) {
	Vec2D a(0, 0);
	Vec2D b(10, 20);
	Vec2D mid = a.lerpTo(b, 0.5f);
	TEST_CHECK_FLOAT(mid.getX(), 5.0f, 0.001f, "lerp 0.5 X");
	TEST_CHECK_FLOAT(mid.getY(), 10.0f, 0.001f, "lerp 0.5 Y");
	Vec2D start = a.lerpTo(b, 0.0f);
	TEST_CHECK_FLOAT(start.getX(), 0.0f, 0.001f, "lerp 0.0 returns start");
	Vec2D end = a.lerpTo(b, 1.0f);
	TEST_CHECK_FLOAT(end.getX(), 10.0f, 0.001f, "lerp 1.0 returns end");
}

TEST(Vec2D_CreateNormal) {
	Vec2D n0 = Vec2D::createNormal(0.0f);
	TEST_CHECK_FLOAT(n0.getX(), 1.0f, 0.001f, "0 deg normal is (1,0)");
	TEST_CHECK_FLOAT(n0.getY(), 0.0f, 0.001f, "0 deg normal is (1,0)");
	Vec2D n90 = Vec2D::createNormal(90.0f);
	TEST_CHECK_FLOAT(n90.getX(), 0.0f, 0.001f, "90 deg normal is (0,1)");
	TEST_CHECK_FLOAT(n90.getY(), 1.0f, 0.001f, "90 deg normal is (0,1)");
}

TEST(Vec2D_Rotate) {
	Vec2D v(1.0f, 0.0f);
	v.rotateRadians(DEG2RAD(90.0f));
	TEST_CHECK_FLOAT(v.getX(), 0.0f, 0.01f, "after 90 deg rotate X ~ 0");
	TEST_CHECK_FLOAT(v.getY(), 1.0f, 0.01f, "after 90 deg rotate Y ~ 1");
}

TEST(Vec2D_DegreeBetween) {
	Vec2D a = Vec2D::createNormal(0.0f);
	Vec2D b = Vec2D::createNormal(45.0f);
	TEST_CHECK_FLOAT(a.degreesBetween(b), 45.0f, 0.1f, "angle between 0 and 45 is 45");
}

// ================================================================
// BBox2D
// ================================================================

TEST(BBox2D_PointInside) {
	BBox2D box;
	box.addPoint(0, 0);
	box.addPoint(10, 10);
	TEST_CHECK(box.isInside(Vec2D(5, 5)) == true, "center point is inside");
	TEST_CHECK(box.isInside(Vec2D(0, 0)) == true, "corner is inside (inclusive)");
	TEST_CHECK(box.isInside(Vec2D(10, 10)) == true, "corner is inside (inclusive)");
	TEST_CHECK(box.isInside(Vec2D(-1, 5)) == false, "outside left");
	TEST_CHECK(box.isInside(Vec2D(11, 5)) == false, "outside right");
	TEST_CHECK(box.isInside(Vec2D(5, -1)) == false, "outside bottom");
	TEST_CHECK(box.isInside(Vec2D(5, 11)) == false, "outside top");
}

TEST(BBox2D_Empty) {
	BBox2D box;
	TEST_CHECK(box.isInside(Vec2D(0, 0)) == false, "empty box contains nothing");
}

TEST(BBox2D_Intersects) {
	BBox2D a;
	a.addPoint(0, 0);
	a.addPoint(10, 10);
	BBox2D b;
	b.addPoint(5, 5);
	b.addPoint(15, 15);
	TEST_CHECK(a.intersectsBox(b) == true, "overlapping boxes intersect");

	BBox2D c;
	c.addPoint(20, 20);
	c.addPoint(30, 30);
	TEST_CHECK(a.intersectsBox(c) == false, "far-apart boxes don't intersect");
}

TEST(BBox2D_SizeAndCenter) {
	BBox2D box;
	box.addPoint(2, 4);
	box.addPoint(8, 10);
	Vec2D sz = box.getSize();
	TEST_CHECK_FLOAT(sz.getX(), 6.0f, 0.001f, "width");
	TEST_CHECK_FLOAT(sz.getY(), 6.0f, 0.001f, "height");
	Vec2D ctr = box.getCenter();
	TEST_CHECK_FLOAT(ctr.getX(), 5.0f, 0.001f, "center X");
	TEST_CHECK_FLOAT(ctr.getY(), 7.0f, 0.001f, "center Y");
}

// ================================================================
// Plane2D
// ================================================================

TEST(Plane2D_DistanceTo) {
	Plane2D pl = Plane2D::createFromTwoPoints(Vec2D(0, 0), Vec2D(1, 0));
	TEST_CHECK_FLOAT(pl.distanceTo(Vec2D(5, 0)), 0.0f, 0.01f, "point on X axis is on plane");
	TEST_CHECK_FLOAT(pl.distanceTo(Vec2D(0, 1)), -1.0f, 0.01f, "point above is behind");
}

TEST(Plane2D_ProjectPoint) {
	Plane2D pl = Plane2D::createFromTwoPoints(Vec2D(0, 0), Vec2D(1, 0));
	Vec2D proj = pl.projectPoint(Vec2D(5, 7));
	TEST_CHECK_FLOAT(proj.getX(), 5.0f, 0.01f, "projected X unchanged");
	TEST_CHECK_FLOAT(proj.getY(), 0.0f, 0.01f, "projected Y on plane");
}

TEST(Plane2D_Swap) {
	Plane2D pl = Plane2D::createFromTwoPoints(Vec2D(0, 0), Vec2D(1, 0));
	float d1 = pl.distanceTo(Vec2D(0, 5));
	pl.swap();
	float d2 = pl.distanceTo(Vec2D(0, 5));
	TEST_CHECK_FLOAT(d1 + d2, 0.0f, 0.01f, "swap negates distances");
}

TEST(Plane2D_IntersectLine) {
	Plane2D pl = Plane2D::createFromTwoPoints(Vec2D(0, 0), Vec2D(1, 0));
	Vec2D res;
	TEST_CHECK(pl.intersectLine(Vec2D(3, 5), Vec2D(3, -5), &res) == true,
			   "vertical line crosses X axis");
	TEST_CHECK_FLOAT(res.getX(), 3.0f, 0.1f, "intersection X");
	TEST_CHECK_FLOAT(res.getY(), 0.0f, 0.1f, "intersection Y");
	TEST_CHECK(pl.intersectLine(Vec2D(3, 5), Vec2D(3, 1), &res) == false,
			   "line above X axis has no intersection");
}

// ================================================================
// Trace / Ray tests
// ================================================================

TEST(Trace_CircleHit) {
	Circle2D circ(Vec2D(10, 0), 2.0f);
	CTrace2D tr;
	tr.setupRay(Vec2D(0, 0), Vec2D(20, 0));
	bool hit = circ.trace(tr);
	TEST_CHECK(hit == true, "ray aimed at circle must hit");
	TEST_CHECK_FLOAT(tr.getHit().getY(), 0.0f, 0.1f, "hit Y on axis");
	TEST_CHECK(tr.getHit().getX() < 10.0f, "hit point before circle center");
}

TEST(Trace_CircleMiss) {
	Circle2D circ(Vec2D(10, 10), 2.0f);
	CTrace2D tr;
	tr.setupRay(Vec2D(0, 0), Vec2D(20, 0));
	bool hit = circ.trace(tr);
	TEST_CHECK(hit == false, "ray missing circle must not hit");
}

TEST(Trace_CapsuleHit) {
	Capsule2D cap(Vec2D(10, -5), Vec2D(10, 5), 2.0f);
	CTrace2D tr;
	tr.setupRay(Vec2D(0, 0), Vec2D(20, 0));
	bool hit = cap.trace(tr);
	TEST_CHECK(hit == true, "ray aimed at capsule must hit");
	TEST_CHECK(tr.getHit().getX() < 10.0f, "hit point before capsule center");
}

TEST(Trace_CapsuleMiss) {
	Capsule2D cap(Vec2D(10, 50), Vec2D(10, 60), 2.0f);
	CTrace2D tr;
	tr.setupRay(Vec2D(0, 0), Vec2D(20, 0));
	bool hit = cap.trace(tr);
	TEST_CHECK(hit == false, "ray missing capsule must not hit");
}

TEST(Trace_SphereVsCircle) {
	Circle2D circ(Vec2D(10, 0), 1.0f);
	CTrace2D tr;
	tr.setupSphere(Vec2D(0, 0), Vec2D(20, 0), 3.0f);
	bool hit = circ.trace(tr);
	TEST_CHECK(hit == true, "sphere trace aimed at circle must hit");
	// sphere expands the target, so hit should be earlier
	TEST_CHECK(tr.getHit().getX() < 8.0f, "sphere trace hits earlier than ray");
}

// ================================================================
// Collision symmetry (A vs B == B vs A)
// ================================================================

TEST(Symmetry_CircleCircle) {
	Circle2D a(Vec2D(0, 0), 5.0f);
	Circle2D b(Vec2D(3, 4), 5.0f);
	TEST_CHECK(a.intersects(&b) == b.intersects(&a), "circle-circle is symmetric");
}

TEST(Symmetry_CapsuleCapsule) {
	Capsule2D a(Vec2D(0, 0), Vec2D(5, 0), 2.0f);
	Capsule2D b(Vec2D(3, 1), Vec2D(8, 1), 2.0f);
	TEST_CHECK(a.intersects(&b) == b.intersects(&a), "capsule-capsule is symmetric");
}

TEST(Symmetry_CircleCapsule) {
	Circle2D circ(Vec2D(5, 1), 2.0f);
	Capsule2D cap(Vec2D(0, 0), Vec2D(10, 0), 2.0f);
	TEST_CHECK(circ.intersects(&cap) == cap.intersects(&circ), "circle-capsule is symmetric");
}

// ================================================================
// Collision normal direction
// ================================================================

TEST(Normal_CircleCircle) {
	Circle2D a(Vec2D(0, 0), 5.0f);
	Circle2D b(Vec2D(3, 0), 5.0f);
	CCollision col;
	a.intersects(&b, &col);
	// normal should roughly point from a to b (positive X)
	TEST_CHECK(col.normal.getX() > 0.0f, "normal points from A toward B");
	TEST_CHECK_FLOAT(col.normal.length(), 1.0f, 0.01f, "normal is unit length");
}

TEST(Normal_CapsuleCapsule) {
	Capsule2D a(Vec2D(0, 0), Vec2D(10, 0), 2.0f);
	Capsule2D b(Vec2D(0, 3), Vec2D(10, 3), 2.0f);
	CCollision col;
	a.intersects(&b, &col);
	// capsules offset in Y, normal should point in +Y
	TEST_CHECK(col.normal.getY() > 0.0f, "normal points toward B in Y");
	TEST_CHECK_FLOAT(col.normal.length(), 1.0f, 0.01f, "normal is unit length");
}

// ================================================================
// Hull vs Hull
// ================================================================

TEST(HullHull_Overlap) {
	Hull2D a;
	a.fromPoly(Vec2D(0, 0), Vec2D(5, 0), Vec2D(5, 5), Vec2D(0, 5));
	Hull2D b;
	b.fromPoly(Vec2D(3, 3), Vec2D(8, 3), Vec2D(8, 8), Vec2D(3, 8));
	CCollision col;
	TEST_CHECK(a.intersects(&b, &col) == true, "Overlapping hulls must intersect");
	TEST_CHECK(col.distance > 0, "Penetration distance must be positive");
}

TEST(HullHull_NoOverlap) {
	Hull2D a;
	a.fromPoly(Vec2D(0, 0), Vec2D(5, 0), Vec2D(5, 5), Vec2D(0, 5));
	Hull2D b;
	b.fromPoly(Vec2D(10, 10), Vec2D(15, 10), Vec2D(15, 15), Vec2D(10, 15));
	TEST_CHECK(a.intersects(&b) == false, "Far-apart hulls must not intersect");
}

// ================================================================
// Edge cases
// ================================================================

TEST(CircleCircle_Concentric) {
	// Circles sharing the same center
	Circle2D a(Vec2D(5, 5), 3.0f);
	Circle2D b(Vec2D(5, 5), 2.0f);
	CCollision col;
	TEST_CHECK(a.intersects(&b, &col) == true, "Concentric circles must intersect");
	TEST_CHECK(col.distance > 0, "Penetration should be positive");
}

TEST(CapsuleCapsule_Collinear) {
	// Two capsules on the same line, overlapping in endpoint
	Capsule2D a(Vec2D(0, 0), Vec2D(5, 0), 1.0f);
	Capsule2D b(Vec2D(4, 0), Vec2D(9, 0), 1.0f);
	TEST_CHECK(a.intersects(&b) == true, "Collinear overlapping capsules must intersect");
}

TEST(CapsuleCapsule_EndToEnd) {
	// Two capsules touching at endpoints
	Capsule2D a(Vec2D(0, 0), Vec2D(5, 0), 1.0f);
	Capsule2D b(Vec2D(7, 0), Vec2D(12, 0), 1.0f);
	// Gap between endpoints is 2 (5 to 7), radius sum is 2 -> touching
	TEST_CHECK(a.intersects(&b) == true, "Capsules touching at endpoints should intersect");
}

TEST(CircleHull_EdgeTouch) {
	// Circle overlapping the edge of the hull
	Hull2D hull;
	hull.fromPoly(Vec2D(0, 0), Vec2D(10, 0), Vec2D(10, 10), Vec2D(0, 10));
	Circle2D circ(Vec2D(-0.5f, 5), 1.0f);
	// circle center at x=-0.5, radius=1 -> overlaps hull edge at x=0 by 0.5
	TEST_CHECK(circ.intersects(&hull) == true, "Circle overlapping hull edge should intersect");
}

TEST(Capsule_ZeroLength) {
	// Degenerate capsule (point A = point B) behaves like a circle
	Capsule2D cap(Vec2D(5, 5), Vec2D(5, 5), 3.0f);
	Circle2D circ(Vec2D(5, 5), 1.0f);
	TEST_CHECK(cap.intersects(&circ) == true,
			   "Zero-length capsule at same pos as circle must overlap");
}

TEST(Hull_Triangle) {
	Hull2D hull;
	hull.fromPoly(Vec2D(0, 0), Vec2D(10, 0), Vec2D(5, 10));
	Circle2D inside(Vec2D(5, 3), 0.5f);
	Circle2D outside(Vec2D(20, 20), 0.5f);
	TEST_CHECK(inside.intersects(&hull) == true, "Circle inside triangle hull");
	TEST_CHECK(outside.intersects(&hull) == false, "Circle outside triangle hull");
}
