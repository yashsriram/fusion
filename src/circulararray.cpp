#include "element.cpp"

struct CircularArray {
    int CAPACITY;
    Element **elements;
    int size;

    CircularArray() : CAPACITY(0), elements(nullptr), size(0) {}

    void set(const int CAPACITY, const int size) {
        this->CAPACITY = CAPACITY;
        elements = new Element *[CAPACITY];
        for (int i = 0; i < CAPACITY; ++i) {
            elements[i] = nullptr;
        }
    }
};