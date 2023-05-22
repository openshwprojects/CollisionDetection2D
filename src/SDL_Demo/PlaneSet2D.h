#ifndef __PLANESET2D_H__
#define __PLANESET2D_H__

#include <Plane2D.h>

// space defined by set of planes with normals facing outwards
class PlaneSet2D {
	Array<Plane2D> planes;

public:
	void rotateAroundRadians(const Vec2D &p, float rad) {
		for(int i = 0; i < planes.size(); i++) {
			planes[i].rotateAroundRadians(p,rad);
		}
	}
	bool isInside(const Vec2D &p) const {
		for(int i = 0; i < planes.size(); i++) {
			float d = planes[i].distanceTo(p);
			if(d > 0) {
				return false;
			}
		}
		return true;
	}
	void addPlane(const Plane2D &p) {
		planes.push_back(p);
	}
	// TODO: sort points to make sure normals are ok?
	bool fromFourPoints(const Vec2D &a, const Vec2D &b, const Vec2D &c, const Vec2D &d) {
		addPlane(Plane2D::createFromTwoPoints(a,b));
		addPlane(Plane2D::createFromTwoPoints(b,c));
		addPlane(Plane2D::createFromTwoPoints(c,d));
		addPlane(Plane2D::createFromTwoPoints(d,a));

		ASSERT_FLOAT_EQUALS(planes[0].distanceTo(a),0,0.01f,"point a should be on plane[0]");
		ASSERT_FLOAT_EQUALS(planes[0].distanceTo(b),0,0.01f,"point b should be on plane[0]");

		ASSERT_FLOAT_EQUALS(planes[1].distanceTo(b),0,0.01f,"point b should be on plane[1]");
		ASSERT_FLOAT_EQUALS(planes[1].distanceTo(c),0,0.01f,"point c should be on plane[1]");

		ASSERT_FLOAT_EQUALS(planes[2].distanceTo(c),0,0.01f,"point c should be on plane[2]");
		ASSERT_FLOAT_EQUALS(planes[2].distanceTo(d),0,0.01f,"point d should be on plane[2]");

		ASSERT_FLOAT_EQUALS(planes[3].distanceTo(d),0,0.01f,"point d should be on plane[3]");
		ASSERT_FLOAT_EQUALS(planes[3].distanceTo(a),0,0.01f,"point a should be on plane[3]");

		Vec2D center = (a+b+c+d) * 0.25f;

		ASSERT_TRUTH(planes[0].getPointPlaneRelation(center)==PS_BACK,"Center of mass should be behind each plane");
		ASSERT_TRUTH(planes[1].getPointPlaneRelation(center)==PS_BACK,"Center of mass should be behind each plane");
		ASSERT_TRUTH(planes[2].getPointPlaneRelation(center)==PS_BACK,"Center of mass should be behind each plane");
		ASSERT_TRUTH(planes[3].getPointPlaneRelation(center)==PS_BACK,"Center of mass should be behind each plane");

		Vec2D col_0_1, col_1_2, col_2_3, col_3_0;
		ASSERT_TRUTH(planes[0].intersectPlane(planes[1],col_0_1)==PR_Intersecting,"Cur and next plane should intersect");
		ASSERT_TRUTH(planes[1].intersectPlane(planes[2],col_1_2)==PR_Intersecting,"Cur and next plane should intersect");
		ASSERT_TRUTH(planes[2].intersectPlane(planes[3],col_2_3)==PR_Intersecting,"Cur and next plane should intersect");
		ASSERT_TRUTH(planes[3].intersectPlane(planes[0],col_3_0)==PR_Intersecting,"Cur and next plane should intersect");
		
		ASSERT_FLOAT_EQUALS(col_0_1.distanceTo(b),0,0.01f,"intersection of planes 0 and 1 gives point b");
		ASSERT_FLOAT_EQUALS(col_1_2.distanceTo(c),0,0.01f,"intersection of planes 1 and 2 gives point c");
		ASSERT_FLOAT_EQUALS(col_2_3.distanceTo(d),0,0.01f,"intersection of planes 2 and 3 gives point d");
		ASSERT_FLOAT_EQUALS(col_3_0.distanceTo(a),0,0.01f,"intersection of planes 3 and 0 gives point a");

		return false;
	}
	int size() const {
		return planes.size();
	}
	const Plane2D &operator[](int idx) const {
		return planes[idx];
	}
	Plane2D &operator[](int idx) {
		return planes[idx];
	}
};

#endif // __PLANESET2D_H__
