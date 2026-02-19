#include <cassert>
#include <cmath>

class vec3 {
    
public:
    float x, y, z;
    vec3() : x(0), y(0), z(0) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    vec3 operator+(const vec3& b) const {
        return vec3(x + b.x, y + b.y, z + b.z);
    }

    vec3 operator-(const vec3& b) const {
        return vec3(x - b.x, y - b.y, z - b.z);
    }

    vec3 operator/(float a) const {
        assert(a != 0);
        return vec3(x/a, y/a, z/a);
    }

    vec3 operator*(float a) const {
        return vec3(x * a, y * a, z * a);
    }

    vec3& operator+=(const vec3& b) {
        x+=b.x;
        y+=b.y;
        z+=b.z;
        return *this;
    }

    vec3& operator-=(const vec3& b) {
        x-=b.x;
        y-=b.y;
        z-=b.z;
        return *this;
    }

    vec3& operator*=(float b) {
        x*=b;
        y*=b;
        z*=b;
        return *this;
    }

    vec3& operator/=(float b) {
        assert(b != 0);
        x/=b;
        y/=b;
        z/=b;
        return *this;
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    float lengthSquared() const {
        return (x * x + y * y + z * z);
    }

    friend vec3 operator*(float b, const vec3& a) {
        return a * b;
    }

    float dot(const vec3& a) const {
        return (x * a.x + y * a.y + z * a.z);
    }

    vec3 cross(const vec3& a) const {
        return vec3(y * a.z - z * a.y, -(x * a.z - z * a.x), x * a.y - y * a.x);
    }

    vec3 normalized() const {
        float len = length();
        return len > 0 ? *this / len : vec3();
    }

    void normalize() {
        *this = normalized();
    }
};

class ray {
public:
    vec3 origin;
    vec3 direction;
    ray() : origin(), direction() {} 

    ray(const vec3& a, const vec3& b) : origin(a), direction(b) {
        direction.normalize();
    }

    vec3 pointAtDist(float dist) const {
        //calculate relstive vector at dist
        vec3 ref = dist * direction;

        return ref + origin;
    }
};

class sphere {
    vec3 center;
    float radius;

public:
    sphere(const vec3& a, float b) : center(a), radius(b) {}
    sphere() : center(), radius{0} {}

    bool rayIntersect(const ray& a, vec3& hit) {
        // Solution of quadratic from ray–sphere intersection:
        //
        // t = (-b ± sqrt(b*b - 4*a*c)) / (2*a)
        //
        // where:
        // a = D·D
        // b = 2 * D·(O - C)
        // c = (O - C)·(O - C) - r*r
        //
        float b = a.direction.dot(a.origin - center);
        float c = (a.origin - center).dot(a.origin - center) - radius * radius;
        float det = b * b - c;

        if (det < 0) {
            return false;
        }
        else if (det == 0) {
            hit = a.pointAtDist(- b);
            return true;
        }
        else {
            float t1 = - b + std::sqrt(det);
            float t2 = -b - std::sqrt(det);
            if (t1 > t2) {
                hit = a.pointAtDist(t2);
                return true;
            }
            else {
                hit = a.pointAtDist(t1);
                return true;
            }
        }
    }
};

#include <iostream>

int main() {
    // Create a sphere at origin with radius 1
    sphere s(vec3(0,10,10), 5.0f);

    // Ray starting at z = -5 pointing toward origin
    ray r(vec3(0,0,0), vec3(0,0,1));

    vec3 hit;

    if (s.rayIntersect(r, hit)) {
        std::cout << "Hit!\n";
        std::cout << "Intersection point: "
                  << hit.x << ", "
                  << hit.y << ", "
                  << hit.z << "\n";
    } else {
        std::cout << "No intersection\n";
    }

    return 0;
}
