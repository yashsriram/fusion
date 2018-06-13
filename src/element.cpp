#include<simplecpp>

extern const int dimension;
extern int randomVar;
extern int cR, cG, cB;

struct Element {
    // state
    double radius;
    double x, y;

    // graphics
    Circle circle;
    Text text;

    // meta data
    int number; // appears on the screen and > 0
    int sector; // the sector number which it is in
    int pointedByIndex; // has the index of the Board.'elements' pointer which points to this in the heap

    Element() {
        // init state
        sector = -1;
        pointedByIndex = -1; // not Pointed by any Board.'elements[i]' pointer
        radius = dimension * 4 / 50.0;
        number = randomVar + 1; // randomness comes from here
        x = dimension / 2.0;
        y = dimension / 2.0;

        // render
        circle = Circle(dimension / 2.0, dimension / 2.0, radius);
        setBgColor();
        text = Text(dimension / 2.0, dimension / 2.0, number);
        text.setColor(COLOR(60, 226, 10));
    }

    // 'x' and 'y' reset using 'sector' and Board.'sectorAngle'
    void setSector(double sectorAngle) {
        x = dimension / 2.0 + (dimension * 2.0 / 5) * cosine(sectorAngle * sector);
        y = dimension / 2.0 + (dimension * 2.0 / 5) * sine(sectorAngle * sector);
        moveElementToXY();
    }

    // 'circle' 'text' are movedTo new position according to new 'x' 'y'
    void moveElementToXY() {
        text.moveTo(x, y);
        circle.moveTo(x, y);
        circle.setFill();
    }

    void updateNameAndColor(int newName) {
        number = newName;
        Text source(x, y, number);
        text = source;
        text.setColor(COLOR(60, 226, 10));
        setBgColor();
    }

    void setBgColor() {
        if (number % 5 == 1) {
            circle.setColor(COLOR(20 + cR, 50 + cG, number * cB));
        } else if (number % 5 == 2) {
            circle.setColor(COLOR(number * cR, 20 + cG, 50 + cB));
        } else if (number % 5 == 3) {
            circle.setColor(COLOR(20 + cR, number * cG, 50 + cB));
        } else if (number % 5 == 4) {
            circle.setColor(COLOR(50 + cR, 20 + cG, number * cB));
        } else if (number % 5 == 0) {
            circle.setColor(COLOR(number * cR, 50 + cG, 20 + cB));
        }
    }

};
