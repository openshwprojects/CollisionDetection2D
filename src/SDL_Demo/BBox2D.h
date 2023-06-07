#ifndef __BBOX2D_H__
#define __BBOX2D_H__

#include <Vec2.h>
#include <Plane2D.h>

class BBox2D {
	Vec2D mins, maxs;
public:

	BBox2D() { clear(); }
	void clear() {
		float inf = 999999.0f;
		mins.set(inf,inf);
		maxs.set(-inf,-inf);
	}
	bool isInside(const Vec2D &p) const {
		return p.getX() >= mins.getX() && p.getX() <= maxs.getX() &&
			   p.getY() >= mins.getY() && p.getY() <= maxs.getY();
	}
	void addPoint(const Vec2D &p) {
		if (p.getX() < mins.getX())
			mins.setX(p.getX());
		if (p.getX() > maxs.getX())
			maxs.setX(p.getX());
		if (p.getY() < mins.getY())
			mins.setY(p.getY());
		if (p.getY() > maxs.getY())
			maxs.setY(p.getY());
	}
	void addPoint(float x, float y) {
		if (x < mins.getX())
			mins.setX(x);
		if (x > maxs.getX())
			maxs.setX(x);
		if (y < mins.getY())
			mins.setY(y);
		if (y > maxs.getY())
			maxs.setY(y);
	}
	void addBox(const BBox2D &b) {
		addPoint(b.mins);
		addPoint(b.maxs);
	}
	bool intersectsBox(const BBox2D &b) const {
		return (maxs.getX() >= b.mins.getX() && mins.getX() <= b.maxs.getX() &&
				maxs.getY() >= b.mins.getY() && mins.getY() <= b.maxs.getY());
	}
	void resetTo(const Vec2D &p) {
		mins = maxs = p;
	}
	Vec2D getSize() const {
		return Vec2D(maxs.getX() - mins.getX(), maxs.getY() - mins.getY());
	}
	float getSizeX() const {
		return maxs.getX() - mins.getX();
	}
	float getSizeY() const {
		return maxs.getY() - mins.getY();
	}
	float getMaxExtentFromCenter() const {
		float a = maxs.getAbsoluteSingle();
		float b = mins.getAbsoluteSingle();
		if(a>b)
			return a;
		return b;
	}
    Vec2D getCorner(int cornerIndex) const {
        switch (cornerIndex) {
            case 0:
                return mins;
            case 1:
                return Vec2D(mins.getX(), maxs.getY());
            case 2:
                return maxs;
            case 3:
                return Vec2D(maxs.getX(), mins.getY());
            default:
                return Vec2D(0.0f, 0.0f);
        }
    }
	Plane2D getPlane(int sideIndex) const {
		int ni = (sideIndex + 1) % 4;
		Plane2D pl;
		pl.fromTwoPoints(getCorner(ni),getCorner(sideIndex));
		return pl;
	}
	float getLowestCornerDot(const Vec2D &v) const {
		float best = getCorner(0).dot(v);
		for(int i = 1; i < 4; i++) {
			float d = getCorner(i).dot(v);
			if(d < best) {
				best = d;
			}
		}
		return best;
	}
};

#endif // __BBOX2D_H__
