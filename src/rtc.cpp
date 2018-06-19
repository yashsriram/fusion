#include "element.cpp"

class RoundTableConference {
    class Chair {
        // TODO Destructor

    public:
        Element *person;
        Chair *next;
        Chair *prev;

        Chair(Element *person, Chair *next, Chair *prev) : person(person), next(next), prev(prev) {}
    };

    Chair *head, *tail;
    Element *newlySpawned;
    int noChairs;

    /**
     * sectorNo >= 0
     * */
    void addChairAtSector(int sectorNo) {
        if (newlySpawned == nullptr) { throw "Null argument exception"; }
        if (sectorNo < 0) { throw "Illegal argument exception"; }

        Chair *justBeforePerson = head;
        for (int i = 0; i < sectorNo; ++i) {
            justBeforePerson = justBeforePerson->next;
        }
        Chair *justAfterPerson = justBeforePerson == nullptr ? nullptr : justBeforePerson->next;

        // add chair
        Chair *newChair = new Chair(newlySpawned, justAfterPerson, justBeforePerson);
        if (justBeforePerson != nullptr) { justBeforePerson->next = newChair; }
        if (justAfterPerson != nullptr) { justAfterPerson->prev = newChair; }

        // reset head and tail
        if (newChair->next == nullptr) { tail = newChair; }
        if (newChair->prev == nullptr) { head = newChair; }
        noChairs++;
        newlySpawned = nullptr;
    }

    double getCurrentSectorAngle() {
        return noChairs == 0 ? 360.0 : 360.0 / noChairs;
    }

    Chair *getNext(const Chair *chair) {
        if (chair == nullptr) { throw "Null argument exception"; }
        return chair->next == nullptr ? head : chair->next;
    }

    Chair *getPrev(const Chair *chair) {
        if (chair == nullptr) { throw "Null argument exception"; }
        return chair->prev == nullptr ? tail : chair->prev;
    }

public:
    RoundTableConference() : head(nullptr), tail(nullptr), newlySpawned(nullptr), noChairs(0) {}

    // TODO Destructor

    void print() {
        Chair *it = head;
        while (it != nullptr) {
            printf("%d ", it->person->name);
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
    void placeNewElement(double theta) {
        int sectorNo = theta / getCurrentSectorAngle();
        addChairAtSector(sectorNo);

        Chair *it = head;
        double sectorAngleAfterPlacing = getCurrentSectorAngle();
        int i = 0;
        while (it != nullptr) {
            it->person->setSector(i, sectorAngleAfterPlacing);
            it = it->next;
            i++;
        }
    }
};
