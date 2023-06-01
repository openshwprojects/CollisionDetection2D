

#include <Common.h>
#include <Vec2.h>
#include <BBox2D.h>
#include <Line2D.h>
#include <Plane2D.h>
#include <Polygon2D.h>
#include <PlaneSet2D.h>

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


