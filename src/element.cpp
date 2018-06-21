#include <simplecpp>
#include "config.cpp"
#include "periodictable.cpp"

class Element {
    // state
    double radius;
    double x, y;

    // graphics
    Circle circle;
    Text text;
    int atomicNumber; // > 0

    const Color TEXT_COLOR = COLOR(0, 0, 0);
    const Color COLOR_POOL[3] = {
            COLOR(186, 104, 200), COLOR(77, 182, 172), COLOR(255, 183, 77)
    };

    void setBgColor() {
        Color color = COLOR_POOL[atomicNumber % (sizeof(COLOR_POOL) / sizeof(*COLOR_POOL))];
        circle.setColor(color);
    }

    string atomicSymbol() {
        return getSymbolOfElement(atomicNumber);
    }

public:
    Element() : radius(WINDOW_SIDE_LENGTH * 4 / 50.0),
                x(WINDOW_SIDE_LENGTH / 2.0),
                y(WINDOW_SIDE_LENGTH / 2.0),
                circle(Circle(WINDOW_SIDE_LENGTH / 2.0, WINDOW_SIDE_LENGTH / 2.0, radius)),
                atomicNumber(randomVar + 1) {
        circle.setFill();
        setBgColor();
        text = Text(WINDOW_SIDE_LENGTH / 2.0, WINDOW_SIDE_LENGTH / 2.0, atomicSymbol());
        text.setColor(TEXT_COLOR);
    }

    int getAtomicNumber() { return atomicNumber; }

    void setSector(int sectorNo, double sectorAngle) {
        double theta = sectorAngle * sectorNo;
        x = WINDOW_SIDE_LENGTH / 2.0 + (WINDOW_SIDE_LENGTH * 2.0 / 5) * cosine(theta);
        y = WINDOW_SIDE_LENGTH / 2.0 + (WINDOW_SIDE_LENGTH * 2.0 / 5) * sine(theta);

        text.moveTo(x, y);
        circle.moveTo(x, y);
        circle.setFill();
    }

    void setName(int newName) {
        atomicNumber = newName;
        text.reset(x, y, atomicSymbol());
        text.setColor(TEXT_COLOR);
        setBgColor();
    }

    bool isFusingElement() {
        return atomicNumber % 5 == 1;
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
