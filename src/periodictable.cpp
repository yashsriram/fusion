#include <string>

const std::string ELEMENT_SYMBOL[] = {
        "H", "He", "Li"
};

std::string getSymbolOfElement(int atomicNumber) {
    int tableSize = sizeof(ELEMENT_SYMBOL) / sizeof(*ELEMENT_SYMBOL);
    return ELEMENT_SYMBOL[atomicNumber % tableSize];
}
