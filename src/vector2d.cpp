#include <cmath>

struct Vector2d {
    double x, y;

    Vector2d() {
        x = 0;
        y = 0;
    }

    Vector2d(double x, double y) {
        this->x = x;
        this->y = y;
    }

    Vector2d* operator-(Vector2d b) {
        return new Vector2d(x - b.x, y - b.y);
    }

    double operator~() {
        return sqrt(x * x + y * y);
    }
};
