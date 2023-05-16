#define SDL_MAIN_HANDLED

#include <SDL.h>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")


#include <math.h>

#define DEG2RAD(deg) ((deg) * M_PI / 180.0)
#define RAD2DEG(rad) ((rad) * 180.0 / M_PI)


#include <cstdio>

void Test_OnAssertionFailed_Float(float real, float expected, float marg, const char* comment, const char* functionName, int lineNumber, const char* fileName)
{
    printf("Assertion failed: %s\n", comment);
    printf("Expected: %f, Actual: %f, Margin: %f\n", expected, real, marg);
    printf("Location: %s:%d - %s\n", fileName, lineNumber, functionName);
    system("pause");
}

void Test_OnAssertionFailed_Truth(const char *expStr, const char* comment, const char* functionName, int lineNumber, const char* fileName)
{
    printf("Assertion failed: %s\n", comment);
    printf("Expected: %s to be true\n", expStr);
    printf("Location: %s:%d - %s\n", fileName, lineNumber, functionName);
    system("pause");
}


#define ASSERT_FLOAT_EQUALS(real, expected, marg, comment) \
	if(fabs((real)-(expected))>marg) { \
		Test_OnAssertionFailed_Float((real),(expected),(marg), (comment),__FUNCTION__, __LINE__, __FILE__); \
	} \


#define ASSERT_TRUTH(expectedTruth, comment) \
	if(!expectedTruth) { \
		Test_OnAssertionFailed_Truth(#expectedTruth, (comment),__FUNCTION__, __LINE__, __FILE__); \
	} \

class Vec2D {
	float x, y;
public:
    Vec2D() : x(0.0f), y(0.0f) {}
    Vec2D(float xVal, float yVal) : x(xVal), y(yVal) {}

    float length() const {
        return sqrt(x * x + y * y);
    }

    void normalize() {
        float len = length();
        if (len != 0.0f) {
            x /= len;
            y /= len;
        }
    }

    float getX() const {
        return x;
    }

    float getY() const {
        return y;
    }

    void set(float xVal, float yVal) {
        x = xVal;
        y = yVal;
    }

    void setX(float xVal) {
        x = xVal;
    }

    void setY(float yVal) {
        y = yVal;
    }
    float distanceTo(const Vec2D& p) const {
		Vec2D tmp = *this - p;
		return tmp.length();
    }

    // Method to get the perpendicular vector
    Vec2D getPerpendicular() const {
        if (x == 0 && y == 0) {
            // Handle the case of zero vector
            return Vec2D(0, 0);
        } else if (x == 0) {
            // Handle the case of vertical vector
			if(y > 0) {
				return Vec2D(-1, 0);
			} else {
				return Vec2D(1, 0);
			}
        } else {
            // Calculate the perpendicular vector
            return Vec2D(-y, x);
        }
    }

    float dot(const Vec2D& other) const {
        return x * other.x + y * other.y;
    }

    float cross(const Vec2D& other) const {
        return x * other.y - y * other.x;
    }

    Vec2D operator+(const Vec2D& other) const {
        return Vec2D(x + other.x, y + other.y);
    }

    Vec2D operator-(const Vec2D& other) const {
        return Vec2D(x - other.x, y - other.y);
    }

    Vec2D operator*(float scalar) const {
        return Vec2D(x * scalar, y * scalar);
    }

    Vec2D operator/(float scalar) const {
        if (scalar != 0.0f) {
            return Vec2D(x / scalar, y / scalar);
        }
        return Vec2D();
    }

    Vec2D& operator+=(const Vec2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2D& operator-=(const Vec2D& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vec2D& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
	void rotateRadians(float angle){
        // Rotate the translated normal
        float cosine = cos(angle);
        float sine = sin(angle);
        float nX = this->x * cosine - this->y * sine;
        float nY = this->x * sine + this->y * cosine;
		this->set(nX,nY);
	}

    Vec2D& operator/=(float scalar) {
        if (scalar != 0.0f) {
            x /= scalar;
            y /= scalar;
        }
        return *this;
    }
    static Vec2D createNormal(float angleDegrees, float length = 1.0f) {
        // Convert the angle from degrees to radians
        float angleRadians = DEG2RAD(angleDegrees);

        // Calculate the x and y components of the vector
        float xComponent = length * cos(angleRadians);
        float yComponent = length * sin(angleRadians);

        // Create and return a new Vec2D object with the calculated components
        return Vec2D(xComponent, yComponent);
    }
    // Returns the angle between two normal vectors in radians
    float angleBetween(const Vec2D& other) const {
        float dotProduct = dot(other);
        float magProduct = length() * other.length();

        float angle = acos(dotProduct / magProduct);

        // Determine the sign of the angle based on the cross product of the two vectors
        float crossProduct = x * other.y - y * other.x;
        if (crossProduct < 0) {
            angle = -angle; // Angle is counter-clockwise, so negate the value
        }

        return angle;
    }
    // Returns the angle between two normal vectors in radians
    float degreesBetween(const Vec2D& other) const {
        float rad = angleBetween(other);
		return RAD2DEG(rad);
    }
};

#include <vector>

template<typename T>
class Array : public std::vector<T>
{
public:
};

class Line2D {
	Vec2D a, b;
public:

	void set(const Vec2D &pa, const Vec2D &pb) {
		this->a = pa;
		this->b = pb;
	}
	const Vec2D &getA() const {
		return a;
	}
	const Vec2D &getB() const {
		return b;
	}
};
enum PlaneSide {
    PS_ON,
    PS_FRONT,
    PS_BACK
};

enum PlaneRelation {
    PR_Parallel,
    PR_Coincident,
    PR_Intersecting
};
class Plane2D {
	Vec2D normal;
	float distance;
public:
    // Constructors
    Plane2D() : normal(Vec2D()), distance(0.0f) {}
    Plane2D(const Vec2D& newNorm, float distanceVal) : normal(newNorm), distance(distanceVal) {}

    Vec2D projectPoint(const Vec2D& point) const {
        float t = -(normal.dot(point) + distance) /(normal.dot(normal));
        return point + normal * t;
    }
	void getLine(Line2D &out, float halfLen) const {
		Vec2D perp = normal.getPerpendicular() * halfLen;
		Vec2D c = normal * -distance;
		
		ASSERT_FLOAT_EQUALS(distanceTo(c),0,0.01f,"Plane center point is on plane ");
		out.set(c-perp, c+perp);
	}
	
	bool intersectLine(const Vec2D& segmentStart, const Vec2D& segmentEnd, Vec2D *res) {
		Vec2D segmentDirection = segmentEnd - segmentStart;
		Vec2D segmentToPlane = segmentStart - normal * distance;

		float dotProduct = segmentToPlane.getX() * normal.getX() + segmentToPlane.getY() * normal.getY();
		float directionDotProduct = segmentDirection.getX() * normal.getX() + segmentDirection.getY() * normal.getY();

		// Check if the segment is parallel to the plane
		if (directionDotProduct == 0.0f) {
			// The segment is parallel to the plane, no intersection
			return false;
		}

		float t = -(dotProduct / directionDotProduct);

		// Check if the intersection point lies within the segment
		if (t >= 0.0f && t <= 1.0f) {
			// Intersection point lies within the segment    
			if(res) {
				*res = segmentStart + segmentDirection * t;
			}
			return true;
		}

		// No intersection
		return false;
	}
	PlaneRelation intersectPlane(const Plane2D& other, Vec2D &res) const {
        float dotProduct = normal.dot(other.normal);

        if (dotProduct == 1.0f) {
            // The planes are parallel
            return PR_Parallel;
        } else {
            // Calculate the intersection point            
			float determinant = normal.getX() * other.normal.getY() - normal.getY() * other.normal.getX();
            float x = (other.normal.getY() * -distance - normal.getY() * -other.distance) / determinant;
            float y = (normal.getX() * -other.distance - other.normal.getX() * -distance) / determinant;
            Vec2D intersectionPoint(x, y);

            // Check if the planes are coincident     
			if (intersectionPoint.getX() * normal.getX() + intersectionPoint.getY() * normal.getY() == distance &&
                intersectionPoint.getX() * other.normal.getX() + intersectionPoint.getY() * other.normal.getY() == other.distance) {
                res = (intersectionPoint);
                return PR_Coincident;
            } else {
                res = (intersectionPoint);
                return PR_Intersecting;
            }
        }
    }
    float distanceTo(const Vec2D& p) const {
		return p.getX()*normal.getX() + p.getY()*normal.getY() + distance;
    }
	void swap() {
		distance *= -1;
		normal *= -1;
	}
	static Plane2D createFromPointAndNormal(const Vec2D &p, const Vec2D &norm) {
		return Plane2D(norm,-p.dot(norm));
	}
	void fromPointAndNormal(const Vec2D &p, const Vec2D &norm) {
		this->normal = norm;
		this->distance = -p.dot(norm);
	}
	static Plane2D createFromTwoPoints(const Vec2D &a, const Vec2D &b) {
		Plane2D r;
		r.fromTwoPoints(a,b);
		return r;
	}
	void fromTwoPoints(const Vec2D &a, const Vec2D &b) {
		Vec2D delta = a - b;
		delta.normalize();
		Vec2D norm = delta.getPerpendicular();
		fromPointAndNormal(a,norm);
	}

    void rotateAroundRadians(const Vec2D &center, float angle) {
		// get nearest point on plane
		Vec2D nrst = projectPoint(center);
		// then rotate it around center
		Vec2D local = nrst - center;
		
		local.rotateRadians(angle);


		// rotate normal
		normal.rotateRadians(angle);

		// recreate plane
		fromPointAndNormal(local+center,normal);
    }
    // Get relation of a point to the plane with a distance epsilon
    PlaneSide getPointPlaneRelation(const Vec2D& pointVal, float epsilon = 0.001f) const {
        float dist = distanceTo(pointVal);
        if (dist < epsilon) {
            return PS_ON;
        } else if (dist > epsilon) {
            return PS_FRONT;
        }
        return PS_BACK;
    }

    // Getter methods
    const Vec2D& getNormal() const {
        return normal;
    }

    float getDistance() const {
        return distance;
    }

    // Setter methods
    void setNormal(const Vec2D& normalVal) {
        normal = normalVal;
    }

    void setDistance(float distanceVal) {
        distance = distanceVal;
    }

};


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
class Polygon2D {
	Array<Vec2D> points;
public:


	int size() const {
		return points.size();
	}
	const Vec2D &operator[](int idx) const {
		return points[idx];
	}
	Vec2D &operator[](int idx) {
		return points[idx];
	}
	// setup infinite polygon capped by one plane
	// normal facing outwards
	void fromPlane(const Plane2D &pl, float maxWorldSize = 9999.0f) {
		points.clear();
		Line2D l;
		pl.getLine(l,maxWorldSize);
		points.push_back(l.getA());
		points.push_back(l.getB());
		// normal faces outwards, so ground direction is negative normal
		points.push_back(l.getB() - pl.getNormal() * maxWorldSize);
		points.push_back(l.getA() - pl.getNormal() * maxWorldSize);
		
		ASSERT_FLOAT_EQUALS(pl.distanceTo(points[2]),-maxWorldSize,0.1f,"Far infinite point of poly to plane must be at maxWorldSize dist")
		ASSERT_FLOAT_EQUALS(pl.distanceTo(points[3]),-maxWorldSize,0.1f,"Far infinite point of poly to plane must be at maxWorldSize dist")
	}
};
class Convex2D {
public:

};
int main() {

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
		// some manual plane vs line intersections, can be easily checked on a piece of paper
		Plane2D pl = Plane2D::createFromTwoPoints(Vec2D(5,5), Vec2D(-5,-5));
		

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
		PlaneSet2D ps1;
		// x is [0,1], y is [0,1], CCW
		ps1.fromFourPoints(Vec2D(1,0),  Vec2D(1,1), Vec2D(0,1), Vec2D(0,0));
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(-1,-1)), "This point should be outside");
		ASSERT_TRUTH(true==ps1.isInside(Vec2D(0.1,0.1)), "This point should be inside");
		ASSERT_TRUTH(true==ps1.isInside(Vec2D(0.7,0.7)), "This point should be inside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(1.2,0.5)), "This point should be outside");
		ASSERT_TRUTH(false==ps1.isInside(Vec2D(0.5,1.5)), "This point should be outside");
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


    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create a window
    SDL_Window* window = SDL_CreateWindow("SDL Rectangle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Set the background color to green
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    // Initialize rectangle properties
    SDL_Rect rect;
    rect.x = 100;
    rect.y = 100;
    rect.w = 200;
    rect.h = 150;

    int delta = 1;  // Amount to enlarge the rectangle by each frame

    // Game loop
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

		// Set the background color to green
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        // Clear the renderer with the background color
        SDL_RenderClear(renderer);

        // Set the rectangle color to red
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        // Enlarge the rectangle
        rect.x -= delta;
        rect.y -= delta;
        rect.w += 2 * delta;
        rect.h += 2 * delta;

        // Render the rectangle
        //SDL_RenderFillRect(renderer, &rect);
        // Draw the rectangle using four lines
        SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y);
        SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x, rect.y + rect.h);
        SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);
        SDL_RenderDrawLine(renderer, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h);


        // Update the window
        SDL_RenderPresent(renderer);

        SDL_Delay(10);  // Add a small delay to control the frame rate
    }

    // Cleanup and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}
