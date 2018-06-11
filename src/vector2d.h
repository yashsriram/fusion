#include <cmath>

class Vector2d {
public:
    double x, y;

    Vector2d() {
        x = 0;
        y = 0;
    }

    Vector2d(int X, int Y) {
        x = X;
        y = Y;
    }

    Vector2d operator-(Vector2d B) {
        Vector2d Difference;
        Difference.x = x - B.x;
        Difference.y = y - B.y;
        return Difference;
    }

    double operator~() {
        double modulus;
        modulus = sqrt(x * x + y * y);
        return modulus;
    }
};
