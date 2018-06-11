#include<simplecpp>

extern int dimension;
extern int randomVar;
extern int cR;
extern int cG;
extern int cB;

struct Element {
    // state
    double radius;
    double x, y;

    // graphics
    Circle circle;
    Text text;

    // meta data
    int name;// name that appears on the screen
    int sectorNo;// the sector number which it is in
    int pointedByIndex;// has the index of the Board.'E' pointer which points to this in the heap

    Element() {
        // initialize
        sectorNo = -1;
        pointedByIndex = -1;// not Pointed by any Board.'E[i]' pointer
        radius = dimension / 10.0 - dimension / 50.0;
        name = 0;
        setName();
        x = dimension / 2.0;
        y = dimension / 2.0;
        renderElement();
    }

    // graphically creates the 'Element' for the first time
    void renderElement() {
        Circle source1(dimension / 2, dimension / 2, radius);
        circle = source1;
        colorSet();
        Text source2(dimension / 2, dimension / 2, name);
        text = source2;
        text.setColor(COLOR(60, 226, 10));
    }

    // 'x' and 'y' reset using 'sectorNo' and Board.'sectorAngle'
    void resetTheElementUsingSectorNo(double SectorAngle) {
        x = dimension / 2 + (2 * dimension / 5) * cosine(SectorAngle * sectorNo);
        y = dimension / 2 + (2 * dimension / 5) * sine(SectorAngle * sectorNo);
        moveElementToXY();
    }

    // 'circle' 'text' are movedTo new position according to new 'x' 'y'
    void moveElementToXY() {
        text.moveTo(x, y);
        circle.moveTo(x, y);
        circle.setFill(1);
    }

    void updateNameAndColor(int newName) {
        name = newName;
        Text source(x, y, name);
        text = source;
        text.setColor(COLOR(60, 226, 10));
        colorSet();
    }

    void setName() {
        if (randomVar == 0) {
            name = 1;
        }
        if (randomVar == 1) {
            name = 2;
        }
        if (randomVar == 2) {
            name = 3;
        }
        if (randomVar == 3) {
            name = 4;
        }
        if (randomVar == 4) {
            name = 5;
        }
    }

    void colorSet() {
        if (name % 5 == 1) {
            circle.setColor(COLOR(20 + cR, 50 + cG, name * cB));
            // circle.setFill(1);
        }
        if (name % 5 == 2) {
            circle.setColor(COLOR(name * cR, 20 + cG, 50 + cB));
            // circle.setFill(1);
        }
        if (name % 5 == 3) {
            circle.setColor(COLOR(20 + cR, name * cG, 50 + cB));
            // circle.setFill(1);
        }
        if (name % 5 == 4) {
            circle.setColor(COLOR(50 + cR, 20 + cG, name * cB));
            // circle.setFill(1);
        }
        if (name % 5 == 0) {
            circle.setColor(COLOR(name * cR, 50 + cG, 20 + cB));
            // circle.setFill(1);
        }
    }
};
