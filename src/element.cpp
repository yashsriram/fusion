#include<simplecpp>
#include "config.cpp"

struct Element {
    // state
    double radius;
    double x, y;

    // graphics
    Circle circle;
    Text text;
    int name; // appears on the screen and > 0

    // meta data
    const Color COLOR_CHAURESTE = COLOR(60, 226, 10);

    Element() : radius(WINDOW_SIDE_LENGTH * 4 / 50.0),
                x(WINDOW_SIDE_LENGTH / 2.0),
                y(WINDOW_SIDE_LENGTH / 2.0),
                circle(Circle(WINDOW_SIDE_LENGTH / 2.0, WINDOW_SIDE_LENGTH / 2.0, radius)),
                name(randomVar + 1) {
        setBgColor();
        text = Text(WINDOW_SIDE_LENGTH / 2.0, WINDOW_SIDE_LENGTH / 2.0, name);
        text.setColor(COLOR_CHAURESTE);
    }

    void setSector(int sectorNo, double sectorAngle) {
        double theta = sectorAngle * sectorNo;
        x = WINDOW_SIDE_LENGTH / 2.0 + (WINDOW_SIDE_LENGTH * 2.0 / 5) * cosine(theta);
        y = WINDOW_SIDE_LENGTH / 2.0 + (WINDOW_SIDE_LENGTH * 2.0 / 5) * sine(theta);

        text.moveTo(x, y);
        circle.moveTo(x, y);
        circle.setFill();
    }

    void setName(int newName) {
        name = newName;
        text.reset(x, y, name);
        text.setColor(COLOR_CHAURESTE);
        setBgColor();
    }

    void setBgColor() {
        Color color;
        switch (name % 5) {
            case 0:
                color = COLOR(name * cR, 50 + cG, 20 + cB);
                break;
            case 1:
                color = COLOR(20 + cR, 50 + cG, name * cB);
                break;
            case 2:
                color = COLOR(name * cR, 20 + cG, 50 + cB);
                break;
            case 3:
                color = COLOR(20 + cR, name * cG, 50 + cB);
                break;
            case 4:
                color = COLOR(50 + cR, 20 + cG, name * cB);
                break;
            default:
                color = COLOR(cR, cG, cB);
                break;
        }
        circle.setColor(color);
    }

};
