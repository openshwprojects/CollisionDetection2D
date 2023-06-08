#ifndef __PLANE2D_H__
#define __PLANE2D_H__

#include <Line2D.h>
#include <Assertion.h>

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
enum PlaneLineRelation {
	// line intersects plane
	PLR_Intersects,
	// line is on plane
	PRL_On,
	// line is in front off plane
	PLR_Front,
	// line is in behind plane
	PLR_Behind,
};
class Plane2D {
	Vec2D normal;
	float distance;
public:
    // Constructors
    Plane2D() : normal(Vec2D()), distance(0.0f) {}
    Plane2D(const Vec2D& newNorm, float distanceVal) : normal(newNorm), distance(distanceVal) {}

	bool compare(const Plane2D &o, float epsD, float epsN) const {
		if(abs(o.distance-this->distance)>epsD)
			return false;
		if(normal.compare(o.normal,epsN)==false)
			return false;
		return true;
	}
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
	
	PlaneLineRelation intersectLineExt(const Vec2D& segmentStart, const Vec2D& segmentEnd, Vec2D *res) const {
		float distanceStart = this->distanceTo(segmentStart);
		float distanceEnd = this->distanceTo(segmentEnd);
		// on front of the plane
		if(distanceStart > 0 && distanceEnd > 0)
			return PLR_Front;
		// behind the plane
		if(distanceStart < 0 && distanceEnd < 0)
			return PLR_Behind;
		float eps = 0.001f;
		// on plane
		if(fabs(distanceStart) < eps && fabs(distanceEnd)) {
			return PRL_On;
		}
		// intersection
		float t = distanceStart / (distanceStart - distanceEnd);
		*res = segmentStart + (segmentEnd - segmentStart) * t;

		return PLR_Intersects;
	}
	bool intersectLine(const Vec2D& segmentStart, const Vec2D& segmentEnd, Vec2D *res) const {
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
				// also check second method
#if 1
				Vec2D verify;
				ASSERT_TRUTH(PLR_Intersects==this->intersectLineExt(segmentStart, segmentEnd, &verify),"Both plane vs line gives the same result");
				ASSERT_FLOAT_EQUALS(res->distanceTo(verify),0.0f, 0.1f, "Both plane vs line gives the same result");
#endif
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
    PlaneSide getPointPlaneRelation(const Vec2D& pointVal, float *outDist = 0, float epsilon = 0.001f) const {
        float dist = distanceTo(pointVal);
		if(outDist)
			*outDist = dist;
        if (dist < epsilon && dist > -epsilon) {
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

#endif
