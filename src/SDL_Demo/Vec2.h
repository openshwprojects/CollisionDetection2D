#ifndef __VEC2_H__
#define __VEC2_H__

#include <Common.h>


class Vec2D {
	float x, y;
public:
    Vec2D() : x(0.0f), y(0.0f) {}
    Vec2D(float xVal, float yVal) : x(xVal), y(yVal) {}

    float length() const {
        return sqrt(x * x + y * y);
    }
    float lengthSq() const {
        return (x * x + y * y);
    }

	Vec2D lerpTo(const Vec2D &b, float f) const {
		float newX = x + (b.x - x) * f;
		float newY = y + (b.y - y) * f;
		return Vec2D(newX, newY);
	}
	void addX(float f){
		x+=f;
	}
	void addY(float f){ 
		y+=f;
	}
	bool isWithin(const Vec2D &p, float marg) const {
		return distanceToSq(p) < marg*marg;
	}
	void setRandom(float max) {
		x = RandomFloatRange(-max,max);
		y = RandomFloatRange(-max,max);
	}
	void setRandom(float min, float max) {
		x = RandomFloatRange(min,max);
		y = RandomFloatRange(min,max);
	}
	void setRandom(float minX, float maxX, float minY, float maxY) {
		x = RandomFloatRange(minX,maxX);
		y = RandomFloatRange(minY,maxY);
	}
    float normalize() {
        float len = length();
        if (len != 0.0f) {
            x /= len;
            y /= len;
        }
		return len;
    }

    float getX() const {
        return x;
    }

    float getY() const {
        return y;
    }

	Vec2D getAbsolute() const {
		return Vec2D(fabs(x),fabs(y));
	}
	float getAbsoluteSingle() const {
		float a = fabs(x);
		float b = fabs(y);
		if(a>b)
			return a;
		return b;
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
    float distanceToSq(const Vec2D& p) const {
		Vec2D tmp = *this - p;
		return tmp.lengthSq();
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
	float getAngleRad() const {
		return atan2(y,x);
	}
	float getAngleDeg() const {
		return RAD2DEG(getAngleRad());
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


#endif
