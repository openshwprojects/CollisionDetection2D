#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_ttf.h>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_ttf.lib")

#include <Common.h>
#include <Vec2.h>
#include <BBox2D.h>
#include <Line2D.h>
#include <Plane2D.h>
#include <Polygon2D.h>
#include <PlaneSet2D.h>
#include <cstdarg>

void doBasicSelfTests() {

	for(float degOfs = 15.0f; degOfs < 150.0f; degOfs += 15.0f) {
		for(float deg = 0; deg < 720.0f; deg += 15.0f) {
			Vec2D a = Vec2D::createNormal(deg-degOfs);
			Vec2D b = Vec2D::createNormal(deg);
			Vec2D c = Vec2D::createNormal(deg+degOfs);
#if 0
			float b_to_a = b.degreesBetween(a);
			loat b_to_c = b.degreesBetween(c);
			printf("b_to_a %f, b_to_c %f\n",b_to_a,b_to_c);
#endif
			ASSERT_FLOAT_EQUALS(b.degreesBetween(a),-degOfs, 0.1f, "degreesBetween must be able to tell in which direction is each vector");
			ASSERT_FLOAT_EQUALS(b.degreesBetween(c),degOfs, 0.1f, "degreesBetween must be able to tell in which direction is each vector");
		}
	}
	{
		Plane2D pl = Plane2D::createFromTwoPoints(Vec2D(0,0), Vec2D(1,0));
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(-1,0)),0.0f, 0.1f, "This plane is on X axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(-2,0)),0.0f, 0.1f, "This plane is on X axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(1,0)),0.0f, 0.1f, "This plane is on X axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(2,0)),0.0f, 0.1f, "This plane is on X axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(0,1)),-1.0f, 0.1f, "And normal is facing down");
		pl.rotateAroundRadians(Vec2D(0,1), DEG2RAD(180));
		// now it should be on X+2 axis
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(1,2)),0.0f, 0.1f, "After rotate, the plane is on X+2 axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(2,2)),0.0f, 0.1f, "After rotate, the plane is on X+2 axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(-1,2)),0.0f, 0.1f, "After rotate, the plane is on X+2 axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(-2,2)),0.0f, 0.1f, "After rotate, the plane is on X+2 axis");
		pl.rotateAroundRadians(Vec2D(0,1), DEG2RAD(180));
		// now it should be on X axis
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(-1,0)),0.0f, 0.1f, "This plane is on X axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(-2,0)),0.0f, 0.1f, "This plane is on X axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(1,0)),0.0f, 0.1f, "This plane is on X axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(2,0)),0.0f, 0.1f, "This plane is on X axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(0,1)),-1.0f, 0.1f, "And normal is facing down");

	}
	{
		BBox2D box;
		float max = 10000.0f;
		for(int i = 0; i < 100; i++) {
			Vec2D t;
			t.setRandom(max);
			ASSERT_TRUTH(box.isInside(t)==false,"empty BBOX does not contain any point");
		}
		// make bbox [5,5] to [7, 9]
		box.addPoint(5,9);
		box.addPoint(7,5);
		for(int i = 0; i < 100; i++) {
			Vec2D t;
			t.setRandom(5,7, 5,9);
			ASSERT_TRUTH(box.isInside(t)==true,"this point should be inside");
		}
		for(int i = 0; i < 100; i++) {
			Vec2D t;
			t.setRandom(0, 1000);
			t.addX(7);
			t.addY(9);
			ASSERT_TRUTH(box.isInside(t)==false,"this point should be outside");
		}
	
	}
	{
		// some manual plane vs line intersections, can be easily checked on a piece of paper
		Plane2D pl = Plane2D::createFromTwoPoints(Vec2D(5,5), Vec2D(-5,-5));
		
		{
			Vec2D res;
			
			// test 1
			ASSERT_TRUTH(pl.intersectLine(Vec2D(0,2), Vec2D(2,0), &res),"There is intersection");
			ASSERT_FLOAT_EQUALS(res.distanceTo(Vec2D(1,1)),0.0f, 0.1f, "Intersection with line test");
			// test 2
			ASSERT_TRUTH(pl.intersectLine(Vec2D(0,7), Vec2D(4,3), &res),"There is intersection");
			ASSERT_FLOAT_EQUALS(res.distanceTo(Vec2D(3.5f,3.5f)),0.0f, 0.1f, "Intersection with line test");
			// test 3 (this line ends before plane)
			ASSERT_TRUTH(false==pl.intersectLine(Vec2D(0,7), Vec2D(3,4), &res),"There is no intersection");
		}
		{	
			Vec2D res;
			
			// test 1
			ASSERT_TRUTH(PLR_Intersects==pl.intersectLineExt(Vec2D(0,2), Vec2D(2,0), &res),"There is intersection");
			ASSERT_FLOAT_EQUALS(res.distanceTo(Vec2D(1,1)),0.0f, 0.1f, "Intersection with line test");
			// test 2
			ASSERT_TRUTH(PLR_Intersects==pl.intersectLineExt(Vec2D(0,7), Vec2D(4,3), &res),"There is intersection");
			ASSERT_FLOAT_EQUALS(res.distanceTo(Vec2D(3.5f,3.5f)),0.0f, 0.1f, "Intersection with line test");
			// test 3 (this line ends before plane)
			ASSERT_TRUTH(PLR_Front==pl.intersectLineExt(Vec2D(0,7), Vec2D(3,4), &res),"There is no intersection");
		}
		

	}
	{
		Plane2D pl = Plane2D::createFromTwoPoints(Vec2D(0,0), Vec2D(1,0));
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(-1,0)),0.0f, 0.1f, "This plane is on X axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(-2,0)),0.0f, 0.1f, "This plane is on X axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(1,0)),0.0f, 0.1f, "This plane is on X axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(2,0)),0.0f, 0.1f, "This plane is on X axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(0,1)),-1.0f, 0.1f, "And normal is facing down");

		Vec2D res;
		ASSERT_TRUTH(pl.intersectLine(Vec2D(123,5), Vec2D(123,-5), &res),"There is intersection");
		ASSERT_FLOAT_EQUALS(res.distanceTo(Vec2D(123,0)),0.0f, 0.1f, "Intersection with line test");
		// plane is on X axis, at Y = 0, so it is not reached by this line
		ASSERT_TRUTH(false==pl.intersectLine(Vec2D(123,5), Vec2D(123,1), &res),"There is no intersection");
		ASSERT_TRUTH(false==pl.intersectLine(Vec2D(123,-5), Vec2D(123,-1), &res),"There is no intersection");
		ASSERT_TRUTH(false==pl.intersectLine(Vec2D(-123,5), Vec2D(123,1), &res),"There is no intersection");
		ASSERT_TRUTH(false==pl.intersectLine(Vec2D(123,-5), Vec2D(-123,-1), &res),"There is no intersection");
	
		for(float x = -10; x < 10; x += 3.0f) {
			Vec2D p(x,1);
			Vec2D pro = pl.projectPoint(p);
			Vec2D expected(x,0);
			ASSERT_FLOAT_EQUALS(pl.distanceTo(pro),0.0f, 0.1f, "After projection, point is on plane");
			ASSERT_FLOAT_EQUALS(expected.distanceTo(pro),0.0f, 0.1f, "After projection, point is as expected");

		}


		pl.rotateAroundRadians(Vec2D(0,0), DEG2RAD(90));
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(0,-1)),0.0f, 0.1f, "After rotate, the plane is on Y axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(0,-2)),0.0f, 0.1f, "After rotate, the plane is on Y axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(0,1)),0.0f, 0.1f, "After rotate, the plane is on Y axis");
		ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(0,2)),0.0f, 0.1f, "After rotate, the plane is on Y axis");

	}
	for(float deg = 0; deg < 720.0f; deg += 15.0f) {
		if(deg > 360.0f) {
			// second roll gets extra random values
			deg += (rand()%1000)*0.01f;
		}
		Vec2D n = Vec2D::createNormal(deg);
		Vec2D perp = n.getPerpendicular();

		ASSERT_FLOAT_EQUALS(n.degreesBetween(perp),90.0f, 0.1f, "Perpendicular vectors have 90 degrees angle");
		ASSERT_FLOAT_EQUALS(n.length(),1.0f, 0.001f, "Normal vectors have by default length 1");
		ASSERT_FLOAT_EQUALS(perp.length(),1.0f, 0.001f, "Perpendicular vectors to normals with len 1 have by default length 1");
		ASSERT_FLOAT_EQUALS(perp.dot(n), 0.0f, 0.001f, "Perpendicular vectors have dot product 0");

		for(float dist = 25.0f; dist < 500.0f; dist += 123.0f) {
			Vec2D point = n * dist;
			Vec2D point_twice_as_far = n * dist * 2;
			Plane2D pl = Plane2D::createFromPointAndNormal(point, n);

			ASSERT_FLOAT_EQUALS(pl.distanceTo(point),0.0f, 0.1f, "Plane created from point has this point lying on itself");
			ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(0,0)),-dist, 0.1f, "For this plane, origin is behind, distance is a negative dist value used");
			ASSERT_FLOAT_EQUALS(pl.distanceTo(point_twice_as_far),dist, 0.1f, "Point twice as far should be in front of the plane");

			for(float testOfs = -123; testOfs < 123; testOfs += 32.0f) {
				Vec2D some_other_point_on_this_plane = point + n.getPerpendicular() * testOfs;
				ASSERT_FLOAT_EQUALS(pl.distanceTo(some_other_point_on_this_plane), 0, 0.1f, "The plane creation point after shifting by vector perpendicular to plane normal should still be on the plane");
			}

			// line on plane
			Line2D line;
			pl.getLine(line, 100.0f);

			ASSERT_FLOAT_EQUALS(pl.distanceTo(line.getA()),0.0f, 0.1f, "First point of line that is on plane is also on plane");
			ASSERT_FLOAT_EQUALS(pl.distanceTo(line.getB()),0.0f, 0.1f, "Second point of line that is on plane is also on plane");


			pl.swap();

			ASSERT_FLOAT_EQUALS(pl.distanceTo(point),0.0f, 0.1f, "After swapping, point is still on plane");
			ASSERT_FLOAT_EQUALS(pl.distanceTo(Vec2D(0,0)),dist, 0.1f, "After swapping, distances are negated");
			ASSERT_FLOAT_EQUALS(pl.distanceTo(point_twice_as_far),-dist, 0.1f, "After swapping, distances are negated");

			Polygon2D poly_inf;
			poly_inf.fromPlane(pl);
		}

	}
	{
		// this is a rectangle rotated by 45 degrees
		// smth like
		//        |
		//        |
		//        | /\
		//        | \/
		// ------- -----------------
		//        |
		//        |
		//        |
		PlaneSet2D ps1;
		ps1.fromFourPoints(Vec2D(5,3),  Vec2D(3,5), Vec2D(1,3), Vec2D(3,1));
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(1, 3.25f)), "This point should be outside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(1, 2.75f)), "This point should be outside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(2, 1)), "This point should be outside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(2, 1.75f)), "This point should be outside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(4, 1.75f)), "This point should be outside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(5, 2.75f)), "This point should be outside");
		for(float f = 0.1f; f < 6.0f; f+= 0.123f) {
			ASSERT_TRUTH(false==ps1.isInside(Vec2D(6, f)), "This point should be outside");
			ASSERT_TRUTH(false==ps1.isInside(Vec2D(0.75f, f)), "This point should be outside");
		}
		for(float f = 1.25f; f < 4.75f; f+= 0.123f) {
			ASSERT_TRUTH(true==ps1.isInside(Vec2D(f, 3.0f)), "This point should be inside");
		}
		for(float f = 2.1f; f < 3.9f; f+= 0.123f) {
			ASSERT_TRUTH(true==ps1.isInside(Vec2D(f, f)), "This point should be inside");
		};
		{
			Polygon2D poly_first;
			poly_first.fromPlanes(ps1);
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(5,3),0.001f), "Poly should have this vertex");
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(3,5),0.001f), "Poly should have this vertex");
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(1,3),0.001f), "Poly should have this vertex");
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(3,1),0.001f), "Poly should have this vertex");
			ASSERT_TRUTH(poly_first.size()==4, "Poly should have 4 vertices");
		}
		
		// if we rotate it around origin by 180, it should be like before but with negative coords
		ps1.rotateAroundRadians(Vec2D(0,0),DEG2RAD(180));

		{
			Polygon2D poly_first;
			poly_first.fromPlanes(ps1);
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(-5,-3),0.001f), "Poly should have this vertex");
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(-3,-5),0.001f), "Poly should have this vertex");
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(-1,-3),0.001f), "Poly should have this vertex");
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(-3,-1),0.001f), "Poly should have this vertex");
			ASSERT_TRUTH(poly_first.size()==4, "Poly should have 4 vertices");
		}
		// if we rotate it again by 90, we expect:
		//
		//        |  
		//        |
		//        |
		// ------- -----------------
		//        |/\
		//        |\/
		//        |
		ps1.rotateAroundRadians(Vec2D(0,0),DEG2RAD(90));
		{
			Polygon2D poly_first;
			poly_first.fromPlanes(ps1);
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(5,-3),0.001f), "Poly should have this vertex");
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(3,-5),0.001f), "Poly should have this vertex");
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(1,-3),0.001f), "Poly should have this vertex");
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(3,-1),0.001f), "Poly should have this vertex");
			ASSERT_TRUTH(poly_first.size()==4, "Poly should have 4 vertices");


			Plane2D cut;
			cut.fromTwoPoints(Vec2D(3,-5),Vec2D(3,-1));
			poly_first.clipByPlane(cut);
			// it is now
			//        |  
			//        |
			//        |
			// ------- -----------------
			//        |/|
			//        |\|
			//        |
			ASSERT_TRUTH(poly_first.size()==3, "Poly should have 3 vertices");
			ASSERT_TRUTH(false==poly_first.hasVertex(Vec2D(5,-3),0.001f), "Poly should NOT have this vertex");
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(3,-5),0.001f), "Poly should have this vertex");
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(1,-3),0.001f), "Poly should have this vertex");
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(3,-1),0.001f), "Poly should have this vertex");



			cut.fromTwoPoints(Vec2D(-20,-3),Vec2D(20,-3));
			poly_first.clipByPlane(cut);
			// it is now
			//        |  
			//        |
			//        |
			// ------- -----------------
			//        |/|
			//        |--
			//        |
			ASSERT_TRUTH(false==poly_first.hasVertex(Vec2D(5,-3),0.001f), "Poly should NOT have this vertex");
			ASSERT_TRUTH(false==poly_first.hasVertex(Vec2D(3,-5),0.001f), "Poly should NOT have this vertex");
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(1,-3),0.001f), "Poly should have this vertex");
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(3,-1),0.001f), "Poly should have this vertex");
			ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(3,-3),0.001f), "Poly should have this vertex");

		}
	}
	{
		PlaneSet2D ps1;
		// x is [0,1], y is [0,1], CCW
		//
		//
		//        |
		//        |
		//        | --
		//        |   |
		// ------- -----------------
		//        |
		//        |
		//        |
		ps1.fromFourPoints(Vec2D(1,0),  Vec2D(1,1), Vec2D(0,1), Vec2D(0,0));
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(-1,-1)), "This point should be outside");
		ASSERT_TRUTH(true==ps1.isInside(Vec2D(0.1,0.1)), "This point should be inside");
		ASSERT_TRUTH(true==ps1.isInside(Vec2D(0.7,0.7)), "This point should be inside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(1.2,0.5)), "This point should be outside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(0.5,1.5)), "This point should be outside");
		Polygon2D poly_first;
		poly_first.fromPlanes(ps1);
		ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(1,0),0.001f), "Poly should have this vertex");
		ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(0,0),0.001f), "Poly should have this vertex");
		ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(0,1),0.001f), "Poly should have this vertex");
		ASSERT_TRUTH(true==poly_first.hasVertex(Vec2D(0,0),0.001f), "Poly should have this vertex");
		ASSERT_TRUTH(poly_first.size()==4, "Poly should have 4 vertices");

		// if we rotate it around origin by 180, it should be within x [-1,0] and y [-1,0]
		ps1.rotateAroundRadians(Vec2D(0,0),DEG2RAD(180));
		ASSERT_TRUTH(true==ps1.isInside(Vec2D(-0.2f,-0.2f)), "This point should be inside");
		ASSERT_TRUTH(true==ps1.isInside(Vec2D(-0.5f,-0.5f)), "This point should be inside");
		ASSERT_TRUTH(true==ps1.isInside(Vec2D(-0.7f,-0.7f)), "This point should be inside");
		ASSERT_TRUTH(true==ps1.isInside(Vec2D(-0.2f,-0.7f)), "This point should be inside");
		ASSERT_TRUTH(true==ps1.isInside(Vec2D(-0.7f,-0.2f)), "This point should be inside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(0.5,0.5)), "This point should be outside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(-0.5,0.5)), "This point should be outside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(0.5,-0.5)), "This point should be outside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(-1.1,-0.5)), "This point should be outside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(-1.1,-0.1)), "This point should be outside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(0.1,-0.1)), "This point should be outside");

	}
	{
		PlaneSet2D ps1;
		// x is [4,5], y is [0,1], CCW
		ps1.fromFourPoints(Vec2D(5,0),  Vec2D(5,1), Vec2D(4,1), Vec2D(4,0));
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(-1,-1)), "This point should be outside");
		ASSERT_TRUTH(true==ps1.isInside(Vec2D(4.1,0.1)), "This point should be inside");
		ASSERT_TRUTH(true==ps1.isInside(Vec2D(4.7,0.7)), "This point should be inside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(1.2,0.5)), "This point should be outside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(0.5,1.5)), "This point should be outside");
	}

}
class IBaseDemo {
protected:
	class IDemoRenderer *r;
public:
	virtual const char *getName() const = 0;
	virtual void onMouseEvent(int x, int y, int button, bool bDown) = 0;
	virtual void onKeyEvent(int key, bool bDown) = 0;
	virtual bool onQuit() = 0;
	virtual void runFrame() = 0;
	virtual void processMyEvent(int code) = 0;

	void setRenderer(IDemoRenderer *ren) {
		this->r = ren;
	}
};
class IDemoRenderer { 
public:
	virtual void setColor(byte r, byte g, byte b, byte a = 255) = 0;
	virtual void drawLine(const Vec2D &a, const Vec2D &b) = 0;
	virtual void drawLine(const Vec2D &a, const Vec2D &b, int width) = 0;
	virtual void beginFrame(byte r, byte g, byte b, byte a = 255) = 0;
	virtual int drawText(int x, int y, const char *s, byte r, byte g, byte b) = 0;
	virtual void endFrame() = 0;
};

class CBaseDemo : public IBaseDemo {
protected:
    class CDemoContainer* container;
public:
	void setContainer(CDemoContainer *cc) {
		this->container = cc;
	}
	virtual void processMyEvent(int code) {

	}
	virtual void onDemoInit() {

	}
	virtual const char *getName() const {
		return "UnnamedDemo";
	}
	void drawPoly(const Polygon2D &p, byte r, byte g, byte b, byte a = 255) {
		this->r->setColor(r,g,b);
		for(int i = 0; i < p.size(); i++) {
			int n = (i+1)%p.size();
			this->r->drawLine(p[i],p[n]);
		}
	}
	virtual void onKeyEvent(int key, bool bDown) {

	}
};


#include "DemoSortVertices.h"
#include "DemoContainer.h"


CDemoSortVertices::CDemoSortVertices() {
    bSortVertices = true;
    bDrawRaw = true;
    bDrawAABB = true;
    bDrawConvex = true;
}

void CDemoSortVertices::onDemoInit() {
    container->addSetting(&bSortVertices, "Sort vertices", -1);
    container->addSetting(&bDrawRaw, "Draw user-drawn poly", -1);
    container->addSetting(&bDrawConvex, "Draw regenerated convex", -1);
    container->addSetting(&bDrawAABB, "Draw AABB", -1);
    container->addSetting(101, "Remove last vertex", -1);
}

void CDemoSortVertices::processMyEvent(int code) {
	if(code == 101) {
		poly.removeLastVertex();
	}
}
const char* CDemoSortVertices::getName() const {
    return "Polygon from points -> planes from polygon -> polygon from planes";
}

void CDemoSortVertices::onMouseEvent(int x, int y, int button, bool bDown) {
    if (bDown) {
        poly.addVertex(x, y);
    }
}

bool CDemoSortVertices::onQuit() {
    return true;
}

void CDemoSortVertices::runFrame() {
    r->beginFrame(150, 150, 255);
    container->drawDebugTexts();
    if (bDrawRaw) {
        drawPoly(poly, 255, 0, 0);
    }
    planes.fromPoly(poly.getPoints(), bSortVertices);
    poly2.fromPlanes(planes);
    if (bDrawConvex) {
        drawPoly(poly2, 0, 255, 0);
    }
	if(bDrawAABB) {
		Polygon2D tmp;
		BBox2D box;
		tmp.addToBB(box);
		for(int i = 0; i < 4; i++){
			tmp.addVertex(box.getCorner(i));
		}
        drawPoly(tmp, 0, 0, 255);
	}
    r->endFrame();
}


class CDemoClipByPlane : public CBaseDemo {
	Polygon2D poly;
	PlaneSet2D planes;
	Vec2D start;
public:
	CDemoClipByPlane() {
		poly.addVertex(200,200);
		poly.addVertex(400,200);
		poly.addVertex(400,400);
		poly.addVertex(200,400);

	}
	virtual void onMouseEvent(int x, int y, int button, bool bDown) {
		if(bDown){
			start.set(x,y);
		}
	}
	virtual bool onQuit() {
		return true;
	}
	virtual void runFrame() {
		r->beginFrame(255,255,0);
		drawPoly(poly,255,0,0);

		r->endFrame();
	}
};


#include "DemoContainer.h"

enum {
	MY_EVENT_FIRST = 10,
	MY_EVENT_NEXT_DEMO,
	MY_EVENT_PREV_DEMO,
};
CDemoContainer::CDemoContainer() {
    settingsKeys = "qwertyuiopasdfghjklzxcvbnm";
    nextSettingsKey = 0;
    current = 0;
    addDemo(new CDemoSortVertices());
}

void CDemoContainer::addDemo(CBaseDemo* d) {
    demos.push_back(d);
	d->setContainer(this);
    d->setRenderer(r);
	settings.clear();
	addSetting(MY_EVENT_NEXT_DEMO,"",'q');
	addSetting(MY_EVENT_PREV_DEMO,"",'w');
	d->onDemoInit();
}

void CDemoContainer::resetDebugText() {
    textH = 30;
}

void CDemoContainer::drawDebugTexts() {
    resetDebugText();
	drawDebugText("Demo %i/%i, use Q and W to change", current,demos.size());
    drawDebugText(demos[current]->getName());
    for (int i = 0; i < settings.size(); i++) {
        const SDemoSetting& s = settings[i];
		if(s.name.size()) {
			if(s.boolean) {
				drawDebugText(" - %s = %i - press %c to change", s.name.c_str(), *s.boolean, s.key);
			} else {
				drawDebugText(" - %s = press %c", s.name.c_str(), s.key);
			}
		}
    }
}

void CDemoContainer::processMyEvent(int code) {
	if(code == MY_EVENT_NEXT_DEMO) {
		printf("nxt");
	} else if(code == MY_EVENT_PREV_DEMO) {
		printf("prv");
	}
	demos[current]->processMyEvent(code);
}
void CDemoContainer::onKeyEvent(int key, bool bDown) {
    if (bDown) {
        for (int i = 0; i < settings.size(); i++) {
            SDemoSetting& s = settings[i];
            if (s.key == key) {
				if(s.boolean) {
					*s.boolean = !*s.boolean;
				}
				if(s.eventIndex) {
					processMyEvent(s.eventIndex);
				}
            }
        }
    }
    demos[current]->onKeyEvent(key, bDown);
}

void CDemoContainer::drawDebugText(const char* s, ...) {
    char buffer[256];
    va_list args;
    va_start(args, s);
    vsprintf(buffer, s, args);
    va_end(args);

    textH = r->drawText(20, textH, buffer, 255, 250, 250);
}
bool CDemoContainer::isKeyInUse(int key) const {
	for(int i = 0; i < settings.size(); i++) {
		if(settings[i].key == key)
			return true;
	}
	return false;
}
void CDemoContainer::prepareSettingKey(int *key) {
    if (*key == -1) {
		do {
			*key = settingsKeys[nextSettingsKey];
			nextSettingsKey++;
		} while(isKeyInUse(*key));
    }
}

void CDemoContainer::addSetting(int targetEvent, const char* name, int key) {
    prepareSettingKey(&key);
    SDemoSetting set;
    set.boolean = 0;
	set.eventIndex = targetEvent;
    set.key = key;
    set.name = name;
    settings.push_back(set);
}
void CDemoContainer::addSetting(bool* targetBool, const char* name, int key) {
    prepareSettingKey(&key);
    SDemoSetting set;
    set.boolean = targetBool;
	set.eventIndex = 0;
    set.key = key;
    set.name = name;
    settings.push_back(set);
}

void CDemoContainer::runDemo(IBaseDemo* d) {

}

const char* CDemoContainer::getName() const {
    return "";
}

void CDemoContainer::setRenderer(IDemoRenderer* ren) {
    for (int i = 0; i < demos.size(); i++) {
        demos[i]->setRenderer(ren);
    }
    IBaseDemo::setRenderer(ren);
}

void CDemoContainer::onMouseEvent(int x, int y, int button, bool bDown) {
    demos[current]->onMouseEvent(x, y, button, bDown);
}



bool CDemoContainer::onQuit() {
    return demos[current]->onQuit();
}

void CDemoContainer::runFrame() {
    demos[current]->runFrame();
}



class SDLDemoRenderer : public IDemoRenderer {
	SDL_Renderer* renderer;
    SDL_Window* window;
	class IBaseDemo *demo ;
	int running;
	TTF_Font* font;
public:
	SDLDemoRenderer() {
		font = 0;
	}
	void drawText() {

	}
	void setDemo(IBaseDemo *demo) {
		this->demo = demo;
	}
	void createWindow() {
		// Initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		// Create a window
		window = SDL_CreateWindow("SDL Rectangle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

		// Create a renderer
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		///SDL_RenderSetScale(renderer,3,3);

		if ( TTF_Init() < 0 ) {
			printf("TTF_Init failed\n");
		}
		// Load font
		font = TTF_OpenFont("fonts/open-sans/OpenSans-Regular.ttf", 15);
		if ( !font ) {
			printf("Font load failed\n");
		}

	}
	virtual int drawText(int x, int y, const char *s, byte r, byte g, byte b) {

		SDL_Rect dest;
		SDL_Color foreground = { r, g, b };

		SDL_Surface* text_surf = TTF_RenderText_Solid(font, s, foreground);
		if(text_surf == 0) {
			return y;
		}
		SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, text_surf);
		if(text == 0) {
			SDL_FreeSurface(text_surf);
			return y;
		}
		dest.x = x;
		dest.y = y;
		dest.w = text_surf->w;
		dest.h = text_surf->h;
		SDL_RenderCopy(renderer, text, NULL, &dest);

		SDL_DestroyTexture(text);
		SDL_FreeSurface(text_surf);
		return y + dest.h;
	}
	virtual void setColor(byte r, byte g, byte b, byte a = 255) {
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
	}
	virtual void drawLine(const Vec2D &a, const Vec2D &b, int width) {
		Vec2D v = (a-b);
		v.normalize();
		v = v.getPerpendicular();
		for(float i = -width; i < width; i+=1.0f) {
			Vec2D a2 = a + v * i;
			Vec2D b2 = b + v * i;
			drawLine(a2,b2);
		}
	}
	virtual void drawLine(const Vec2D &a, const Vec2D &b) {
		SDL_RenderDrawLineF(renderer, a.getX(),a.getY(),b.getX(),b.getY());
	}
	virtual bool processEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
				if(demo->onQuit()) {
					running = 0;
					return true;
				}
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				demo->onMouseEvent(event.button.x,event.button.y,event.button.button, true);
			} else if (event.type == SDL_MOUSEBUTTONUP) {
				demo->onMouseEvent(event.button.x,event.button.y,event.button.button, false);
			} else if(event.type == SDL_KEYDOWN) {
				demo->onKeyEvent(event.key.keysym.sym, true);
			} else if(event.type == SDL_KEYUP) {
				demo->onKeyEvent(event.key.keysym.sym, false);
			}
        }
		return false;
	}
	virtual void shutdown() {
		// Cleanup and quit SDL
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	virtual void beginFrame(byte r, byte g, byte b, byte a) {
		// Set the background color to green
		SDL_SetRenderDrawColor(renderer, r, g, b, a);

        // Clear the renderer with the background color
        SDL_RenderClear(renderer);

	}
	virtual void endFrame() {
        // Update the window
        SDL_RenderPresent(renderer);

        SDL_Delay(10);  // Add a small delay to control the frame rate
	}
};



int main() {

	doBasicSelfTests();


	CDemoContainer *demo = new CDemoContainer();
	SDLDemoRenderer *ren = new SDLDemoRenderer();
	demo->setRenderer(ren);
	ren->createWindow();
	ren->setDemo(demo);
	while(!ren->processEvents()) {
		demo->runFrame();
	}
	ren->shutdown();

	return 0;
}
