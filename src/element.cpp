#include<simplecpp>
#include "config.cpp"

struct Element {
    // state
    double radius;
    double x, y;

    // graphics
    Circle circle;
    Text text;
    int name; // > 0

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

    bool isFusingElement() {
        return name % 5 == 1;
    }

    static void bubblingEffect(Element *E1, Element *E2) {
        double x1 = E1->x;
        double y1 = E1->y;

        double x2 = E2->x;
        double y2 = E2->y;

        double radius1 = E1->radius;
        double radius2 = E2->radius;

        const float waitTime = 0.0005;
        const double range = WINDOW_SIDE_LENGTH / 100.;
        int radiusStep = 1;

        double tempRadius1 = radius1;
        double tempRadius2 = radius2;

        int halfCycleCounter = 0;
        while (true) {
            E1->circle.reset(x1, y1, tempRadius1);
            E2->circle.reset(x2, y2, tempRadius2);
            tempRadius1 = tempRadius1 + radiusStep;
            tempRadius2 = tempRadius2 + radiusStep;
            if (tempRadius1 >= radius1 + range || tempRadius2 >= radius2 + range) {
                radiusStep = -radiusStep;
                halfCycleCounter++;
            }
            if (tempRadius1 <= radius1 - range || tempRadius1 <= radius2 - range) {
                radiusStep = -radiusStep;
                halfCycleCounter++;
            }
            if (halfCycleCounter == 2 && (tempRadius1 == radius1 || tempRadius2 == radius2))break;
            wait(waitTime);
        }
    }

};
