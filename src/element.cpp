#include <simplecpp>
#include "config.cpp"
#include "periodictable.cpp"

class ElementGraphics {
    const Color TEXT_COLOR = COLOR(0, 0, 0);
    const Color COLOR_POOL[120] = {
            COLOR(186, 104, 200), COLOR(77, 182, 172), COLOR(255, 183, 77), COLOR(229, 115, 115), COLOR(161, 136, 127), COLOR(64, 23, 16), COLOR(234, 182, 116), COLOR(204, 108, 19), COLOR(93, 94, 148), COLOR(84, 166, 156), COLOR(224, 59, 110), COLOR(12, 213, 2), COLOR(116, 157, 20), COLOR(45, 252, 174), COLOR(77, 15, 170), COLOR(155, 159, 155), COLOR(146, 193, 96), COLOR(166, 154, 27), COLOR(73, 244, 243), COLOR(80, 4, 213), COLOR(233, 115, 162), COLOR(244, 8, 20), COLOR(112, 185, 216), COLOR(46, 208, 56), COLOR(47, 55, 234), COLOR(105, 166, 91), COLOR(238, 45, 41), COLOR(219, 255, 87), COLOR(29, 25, 247), COLOR(244, 213, 252), COLOR(109, 3, 139), COLOR(136, 37, 212), COLOR(90, 38, 185), COLOR(10, 126, 43), COLOR(223, 240, 79), COLOR(219, 228, 176), COLOR(205, 113, 178), COLOR(121, 54, 240), COLOR(81, 248, 203), COLOR(79, 86, 82), COLOR(3, 36, 234), COLOR(84, 162, 174), COLOR(110, 217, 110), COLOR(52, 12, 147), COLOR(234, 219, 86), COLOR(133, 26, 33), COLOR(132, 70, 100), COLOR(74, 120, 253), COLOR(150, 15, 170), COLOR(65, 149, 168), COLOR(119, 57, 1), COLOR(194, 51, 97), COLOR(195, 166, 211), COLOR(18, 39, 72), COLOR(141, 143, 110), COLOR(62, 100, 82), COLOR(250, 74, 224), COLOR(142, 48, 79), COLOR(106, 19, 195), COLOR(77, 35, 181), COLOR(223, 211, 17), COLOR(163, 93, 94), COLOR(84, 243, 239), COLOR(137, 217, 98), COLOR(135, 54, 89), COLOR(231, 42, 36), COLOR(162, 43, 25), COLOR(255, 174, 252), COLOR(121, 225, 152), COLOR(61, 29, 172), COLOR(10, 58, 11), COLOR(149, 161, 206), COLOR(75, 178, 177), COLOR(56, 235, 63), COLOR(124, 209, 94), COLOR(158, 24, 26), COLOR(133, 132, 248), COLOR(199, 236, 40), COLOR(37, 164, 187), COLOR(125, 169, 249), COLOR(155, 66, 84), COLOR(28, 26, 177), COLOR(156, 59, 179), COLOR(30, 37, 58), COLOR(243, 54, 148), COLOR(202, 110, 227), COLOR(239, 71, 41), COLOR(82, 104, 136), COLOR(17, 105, 105), COLOR(37, 164, 212), COLOR(24, 32, 235), COLOR(109, 129, 9), COLOR(105, 227, 119), COLOR(74, 200, 190), COLOR(137, 216, 114), COLOR(10, 190, 182), COLOR(254, 137, 238), COLOR(131, 62, 31), COLOR(36, 179, 6), COLOR(33, 229, 237), COLOR(45, 176, 30), COLOR(251, 184, 24), COLOR(138, 64, 184), COLOR(205, 43, 149), COLOR(248, 175, 126), COLOR(82, 229, 149), COLOR(119, 186, 81), COLOR(66, 170, 177), COLOR(155, 253, 241), COLOR(149, 13, 193), COLOR(65, 89, 36), COLOR(187, 223, 138), COLOR(113, 100, 175), COLOR(141, 188, 253), COLOR(149, 49, 23), COLOR(241, 142, 182), COLOR(218, 100, 29), COLOR(212, 243, 87), COLOR(166, 154, 250), COLOR(84, 43, 252), 
    };

    Color colorOf(int atomicNumber) {
        if (atomicNumber == 0) { return COLOR(233, 30, 99); }
        else { return COLOR_POOL[atomicNumber % (sizeof(COLOR_POOL) / sizeof(*COLOR_POOL))]; }
    }

    string symbolOf(int atomicNumber) {
        if (atomicNumber == 0) { return "+"; }
        else { return periodicTableSymbolOf(atomicNumber); }
    }

public:
    Circle *circle;
    Text *text;

    ElementGraphics(double radius, int atomicNumber) :
            circle(new Circle(WINDOW_SIDE_LENGTH / 2.0, WINDOW_SIDE_LENGTH / 2.0, radius)) {
        // circle
        circle->setFill();
        circle->setColor(colorOf(atomicNumber));
        // text
        text = new Text(WINDOW_SIDE_LENGTH / 2.0, WINDOW_SIDE_LENGTH / 2.0, symbolOf(atomicNumber));
        text->setColor(TEXT_COLOR);
    }

    ~ElementGraphics() {
        delete text;
        delete circle;
    }

    void render(double x, double y, int atomicNumber) {
        // circle
        circle->setColor(colorOf(atomicNumber));
        // text
        text->reset(x, y, symbolOf(atomicNumber));
    }

    void render(double x, double y) {
        // circle
        circle->moveTo(x, y);
        // text
        text->moveTo(x, y);
    }

};

class Element {
    // state
    double radius;
    double x, y;
    int atomicNumber;

    // graphics
    ElementGraphics graphics;

public:
    explicit Element(int atomicNumber) : radius(WINDOW_SIDE_LENGTH * 4 / 50.0),
                                         x(WINDOW_SIDE_LENGTH / 2.0),
                                         y(WINDOW_SIDE_LENGTH / 2.0),
                                         atomicNumber(atomicNumber),
                                         graphics(WINDOW_SIDE_LENGTH * 4 / 50.0, atomicNumber) {}

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
        return atomicNumber == 0;
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
            e1->graphics.circle->reset(x1, y1, tempRadius1);
            e2->graphics.circle->reset(x2, y2, tempRadius2);
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
