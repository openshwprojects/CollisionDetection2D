#include <Vec2.h>

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
};
