#include "element.cpp"

class RoundTableConference {
    class Chair {
    public:
        Element *person;
        Chair *next;
        Chair *prev;

        Chair(Element *person, Chair *next, Chair *prev) : person(person), next(next), prev(prev) {
            if (person == nullptr) { throw "Null pointer initialization exception"; }
        }

        ~Chair() {
            // free person in this chair
            delete person;
        }
    };

    const Color COLOR_CHAURESTE = COLOR(60, 226, 10);

    Chair *head, *tail;
    Element *newlySpawned;
    int noChairs;

    /**
     * sectorNo >= 0
     * */
    void addChairAtSector(int sectorNo) {
        if (newlySpawned == nullptr) { throw "Null argument exception"; }
        if (sectorNo < 0) { throw "Illegal argument exception"; }

        Chair *justBeforeChair = head;
        for (int i = 0; i < sectorNo; ++i) {
            justBeforeChair = justBeforeChair->next;
        }
        Chair *justAfterChair = justBeforeChair == nullptr ? nullptr : justBeforeChair->next;

        // add chair
        Chair *newChair = new Chair(newlySpawned, justAfterChair, justBeforeChair);
        if (justBeforeChair != nullptr) { justBeforeChair->next = newChair; }
        if (justAfterChair != nullptr) { justAfterChair->prev = newChair; }

        // reset head and tail
        if (newChair->next == nullptr) { tail = newChair; }
        if (newChair->prev == nullptr) { head = newChair; }
        noChairs++;
        newlySpawned = nullptr;
    }

    void removeChair(Chair *chair) {
        if (chair == nullptr) { throw "Null argument exception"; }
        Chair *justAfterChair = chair->next;
        Chair *justBeforeChair = chair->prev;
        if (justAfterChair != nullptr && justBeforeChair != nullptr) {
            justBeforeChair->next = justAfterChair;
            justAfterChair->prev = justBeforeChair;
        } else if (justAfterChair == nullptr && justBeforeChair != nullptr) {
            justBeforeChair->next = justAfterChair;
            tail = justBeforeChair;
        } else if (justAfterChair != nullptr && justBeforeChair == nullptr) {
            justAfterChair->prev = justBeforeChair;
            head = justAfterChair;
        } else {
            head = justBeforeChair;
            tail = justAfterChair;
        }
        noChairs--;

        delete chair;
    }

    double getCurrentSectorAngle() {
        return noChairs == 0 ? 360.0 : 360.0 / noChairs;
    }

    Chair *getNextChair(const Chair *chair) {
        if (chair == nullptr) { throw "Null argument exception"; }
        return chair->next == nullptr ? head : chair->next;
    }

    Chair *getPrevChair(const Chair *chair) {
        if (chair == nullptr) { throw "Null argument exception"; }
        return chair->prev == nullptr ? tail : chair->prev;
    }

    void rearrangeChairs() {
        Chair *it = head;
        double sectorAngleAfterPlacing = getCurrentSectorAngle();
        int i = 0;
        while (it != nullptr) {
            it->person->setSector(i, sectorAngleAfterPlacing);
            it = it->next;
            i++;
        }
    }

public:
    RoundTableConference() : head(nullptr), tail(nullptr), newlySpawned(nullptr), noChairs(0) {}

    ~RoundTableConference() {
        // free all chairs
        Chair *it = tail;
        while (it != nullptr) {
            Chair *prev = it->prev;
            delete it;
            it = prev;
        }
    }

    int getCount() { return noChairs; }

    int getHighest() {
        int ans = 0;
        Chair *it = head;
        while (it != nullptr) {
            if (it->person->getAtomicNumber() > ans) { ans = it->person->getAtomicNumber(); }
            it = it->next;
        }
        return ans;
    }

    void print() {
        Chair *it = head;
        while (it != nullptr) {
            printf("%d ", it->person->getAtomicNumber());
            it = it->next;
        }
        printf("\n");
    }

    void spawn() {
        if (newlySpawned != nullptr) {
            throw "Over spawn exception";
        }
        newlySpawned = new Element();
    }

    /**
     * Assumption: 0 <= theta <= 359
     * */
    void place(double theta) {
        int sectorNo = theta / getCurrentSectorAngle();
        addChairAtSector(sectorNo);
        rearrangeChairs();
    }

    int fuse(int noFusionsOccured = 0) {
        if (noChairs < 3) {
            // fusion will not occur
            return noFusionsOccured;
        }

        // detect fusable combo
        int fusionCount = 0;
        Chair *it = head;
        while (it != nullptr) {
            if (it->person->isFusingElement()) {
                // look for combo on either sides
                Chair *cw = getNextChair(it);
                Chair *acw = getPrevChair(it);
                while (true) {
                    if (cw->person->getAtomicNumber() == acw->person->getAtomicNumber()) {
                        if (cw != acw) { fusionCount++; } else { break; }
                        if (getNextChair(cw) == acw) { break; }
                    } else {
                        break;
                    }

                    // cw acw go round the circle
                    cw = getNextChair(cw);
                    acw = getPrevChair(acw);
                }
            }
            if (fusionCount > 0) {
                break;
            }
            it = it->next;
        }

        if (fusionCount == 0) {
            // nothing to fuse
            return noFusionsOccured;
        }
        // printf("fusion count = %d\n", fusionCount);

        // fuse elements
        Text fusingTextView(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2., "FUSING ELEMENTS...");
        fusingTextView.setColor(COLOR_CHAURESTE);

        Chair *fusingChair = it;
        int newName = fusingChair->person->getAtomicNumber();
        Chair *cw, *acw;
        Chair *nextCw = getNextChair(it);
        Chair *nextAcw = getPrevChair(it);
        for (int i = 0; i < fusionCount; i++) {
            cw = nextCw;
            acw = nextAcw;
            // update atomicNumber
            newName += cw->person->getAtomicNumber();
            noFusionsOccured++;

            Element::bubblingEffect(cw->person, acw->person);

            nextCw = getNextChair(cw);
            nextAcw = getPrevChair(acw);

            removeChair(cw);
            removeChair(acw);
        }

        fusingChair->person->setAtomicNumber(newName);
        rearrangeChairs();

        return fuse(noFusionsOccured);
    }

};
