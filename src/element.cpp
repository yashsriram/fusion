#include <simplecpp>
#include "config.cpp"
#include "periodictable.cpp"

class ElementGraphics {
    const Color TEXT_COLOR = COLOR(0, 0, 0);
    const Color COLOR_POOL[5] = {
            COLOR(186, 104, 200), COLOR(77, 182, 172), COLOR(255, 183, 77), COLOR(229, 115, 115), COLOR(161, 136, 127)
    };

public:
    Circle circle;
    Text text;

    ElementGraphics(double radius, int atomicNumber) : circle(
            Circle(WINDOW_SIDE_LENGTH / 2.0, WINDOW_SIDE_LENGTH / 2.0, radius)) {
        // circle
        circle.setFill();
        Color color = COLOR_POOL[atomicNumber % (sizeof(COLOR_POOL) / sizeof(*COLOR_POOL))];
        circle.setColor(color);
        // text
        text = Text(WINDOW_SIDE_LENGTH / 2.0, WINDOW_SIDE_LENGTH / 2.0, getSymbolOfElement(atomicNumber));
        text.setColor(TEXT_COLOR);
    }

    void render(double x, double y, int atomicNumber) {
        // circle
        Color color = COLOR_POOL[atomicNumber % (sizeof(COLOR_POOL) / sizeof(*COLOR_POOL))];
        circle.setColor(color);
        // text
        text.reset(x, y, getSymbolOfElement(atomicNumber));
    }

    void render(double x, double y) {
        // circle
        circle.moveTo(x, y);
        // text
        text.moveTo(x, y);
    }

};

class Element {
    // state
    double radius;
    double x, y;
    int atomicNumber; // > 0

    // graphics
    ElementGraphics graphics;

public:
    Element() : radius(WINDOW_SIDE_LENGTH * 4 / 50.0),
                x(WINDOW_SIDE_LENGTH / 2.0),
                y(WINDOW_SIDE_LENGTH / 2.0),
                atomicNumber(randomVar + 1),
                graphics(WINDOW_SIDE_LENGTH * 4 / 50.0, atomicNumber) {
    }

    int getAtomicNumber() { return atomicNumber; }

    void setSector(int sectorNo, double sectorAngle) {
        double theta = sectorAngle * sectorNo;
        x = WINDOW_SIDE_LENGTH / 2.0 + (WINDOW_SIDE_LENGTH * 2.0 / 5) * cosine(theta);
        y = WINDOW_SIDE_LENGTH / 2.0 + (WINDOW_SIDE_LENGTH * 2.0 / 5) * sine(theta);
        graphics.render(x, y);
    }

    void setAtomicNumber(int newAtomicNumber) {
        atomicNumber = newAtomicNumber;
        graphics.render(x, y, newAtomicNumber);
    }

    bool isFusingElement() {
        return atomicNumber % 5 == 1;
    }

    static void bubblingEffect(Element *e1, Element *e2) {
        double x1 = e1->x;
        double y1 = e1->y;

        double x2 = e2->x;
        double y2 = e2->y;

        double radius1 = e1->radius;
        double radius2 = e2->radius;

        const float waitTime = 0.0005;
        const double range = WINDOW_SIDE_LENGTH / 100.;
        int radiusStep = 1;

        double tempRadius1 = radius1;
        double tempRadius2 = radius2;

        int halfCycleCounter = 0;
        while (true) {
            e1->graphics.circle.reset(x1, y1, tempRadius1);
            e2->graphics.circle.reset(x2, y2, tempRadius2);
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
            if (halfCycleCounter == 2 && (tempRadius1 == radius1 || tempRadius2 == radius2)) { break; }
            wait(waitTime);
        }
    }

};
