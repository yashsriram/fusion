#include <simplecpp>
#include <fstream>
#include "element.cpp"
#include "utils.cpp"

const int dimension = 1000; // dimension of Square Canvas
int randomVar = 0; // random variable for color and number
int cR = 1, cG = 2, cB = 3; // random variables for colors

struct Board {
    /**
     * Max number of stable elements are MAX_ELEMENTS - 1
     * A transition of MAX_ELEMENTS elements is possible if it creates a combo and fusion occurs
     * else the game ends on MAX_ELEMENTS elements
     * excluding the center one
     * */
    const int MAX_ELEMENTS;
    const Color COLOR_CHAURESTE = COLOR(60, 226, 10);

    // state
    int noElements;
    int score;
    double currentSectorAngle;
    bool comboContinue;
    Element **elements;

    // graphics
    Text *noElementsTextView;
    Text *highestNumberTextView;
    Text *scoreTextView;

    // meta data
    string userName;
    ifstream highScoreFileInput;
    ofstream highScoreFileOutput;
    ofstream allScoresFileOutput;

    Board() : MAX_ELEMENTS(13),
              noElements(0),
              score(0),
              currentSectorAngle(360),
              comboContinue(false),
              elements(new Element *[MAX_ELEMENTS]) {

        // initializes elements to nullptr
        for (int i = 0; i < MAX_ELEMENTS; i++) {
            elements[i] = nullptr;
        }
    }


    ~Board() {
        delete[] elements;
        delete noElementsTextView;
        delete highestNumberTextView;
        delete scoreTextView;
        closeCanvas();
    }

    void render() {
        cout << "Enter your number (No spaces):" << endl;
        cin >> userName;
        initCanvas("Game0n", dimension, dimension);

        Rectangle base(dimension / 2., dimension / 2., dimension, dimension);
        base.setColor(COLOR(0, 0, 0)).setFill();
        base.imprint();

        Circle outerCircle(dimension / 2., dimension / 2., dimension / 2. - 10);
        outerCircle.setColor(COLOR(20, 20, 20)).setFill();
        outerCircle.imprint();

        Circle innerCircle(dimension / 2., dimension / 2., dimension / 10.);
        innerCircle.setColor(COLOR(5, 5, 5)).setFill();
        innerCircle.imprint();

        Text exitText(dimension / 12., dimension * 11 / 12., "EXIT");
        exitText.setColor(COLOR_CHAURESTE).imprint();

        // Text Views
        noElementsTextView = new Text(dimension * 11 / 12., dimension * (11 / 12.) - dimension / 50.,
                                      "ELEMENTS ON BOARD");
        noElementsTextView->setColor(COLOR_CHAURESTE).imprint();

        highestNumberTextView = new Text(dimension / 12., dimension / 12. - dimension / 50., "HIGHEST NUMBER ACHIEVED");
        highestNumberTextView->setColor(COLOR_CHAURESTE).imprint();

        scoreTextView = new Text(dimension * 11 / 12., dimension / 12. - dimension / 50., "SCORE");
        scoreTextView->setColor(COLOR_CHAURESTE).imprint();
    }

    int getHighestNumber() {
        int highestNumber = 0;
        for (int i = 0; i < MAX_ELEMENTS; i++) {
            if (elements[i] != nullptr) {
                if (elements[i]->number > highestNumber) { highestNumber = elements[i]->number; }
            }
        }
        return highestNumber;
    }

    void startGameLoop() {
        while (true) {
            noElementsTextView->reset(dimension - dimension / 12., dimension - dimension / 12., noElements);
            highestNumberTextView->reset(dimension / 12., dimension / 12., getHighestNumber());
            scoreTextView->reset(dimension - dimension / 12., dimension / 12., score);

            int newElementIndex = spawnNewElement();
            placeNewElement(newElementIndex);
            randomVar = (randomVar + 1) % 5;
            anyCombo();

            if (noElements >= 13) {
                Text GameOver(dimension / 2., dimension / 2. - 10, "Game Over o_0");
                GameOver.setColor(COLOR_CHAURESTE);
                wait(2);
                {
                    Text FinalScoreMessage(dimension / 2., dimension / 2. + 10, "Your Score is");
                    FinalScoreMessage.setColor(COLOR_CHAURESTE);
                    wait(2);
                }
                Text FinalScore(dimension / 2., dimension / 2. + 10, score);
                FinalScore.setColor(COLOR_CHAURESTE);

                wait(2);

                exitGame();
            }
        }
    }

    int spawnNewElement() {
        int newElementIndex = -1;
        if (noElements >= 0 && noElements < MAX_ELEMENTS) {
            for (newElementIndex = 0; newElementIndex < MAX_ELEMENTS; newElementIndex++) {
                if (elements[newElementIndex] == nullptr) { break; }
            }
            if (newElementIndex == MAX_ELEMENTS) { newElementIndex = -1; }

            elements[newElementIndex] = new Element();
            elements[newElementIndex]->pointedByIndex = newElementIndex;
        }
        return newElementIndex;
    }


    void placeNewElement(int newElementIndex) {
        Vector2d pointOfClick;
        registerClick(&pointOfClick);

        // exit pressed ?
        Vector2d exitCenter(dimension / 12., dimension - dimension / 12.);
        double distance = Vector2d().setDiffOf(&exitCenter, &pointOfClick)->mod();
        if (distance <= dimension / 10.) {
            exitGame();
        }

        // get sector of new element
        // theta has the 0 to 359 value
        double theta = rayAngle(dimension / 2., dimension / 2., pointOfClick.x, pointOfClick.y);
        int sectorNoSource = theta / currentSectorAngle;
        elements[newElementIndex]->sector = sectorNoSource;
        // push all the next elements to their next sectors
        for (int i = 0; i < MAX_ELEMENTS; i++) {
            if (elements[i] != nullptr && i != newElementIndex) {
                if (elements[i]->sector > elements[newElementIndex]->sector) {
                    elements[i]->sector++;
                }
            }
        }
        // Push the new element to next sector for the noElements != 0 case
        if (noElements != 0) {
            elements[newElementIndex]->sector++;
        }
        noElements++;

        if (noElements == 0) {
            currentSectorAngle = 360;
        } else if (noElements > 0) {
            currentSectorAngle = 360. / noElements;
        }

        for (int i = 0; i < MAX_ELEMENTS; i++) {
            if (elements[i] != nullptr) {
                elements[i]->setSector(currentSectorAngle);
            }
        }
    }

    void anyCombo() {
        Element *dupElements[MAX_ELEMENTS];
        for (int i = 0; i < MAX_ELEMENTS; i++) {
            dupElements[i] = nullptr;
        }// All the 'Dup'pointers point to 'nullptr'

        for (int i = 0; i < MAX_ELEMENTS; i++) {
            if (elements[i] != nullptr) {
                dupElements[elements[i]->sector] = elements[i];
            }
        }// 'Dup' Pointers set such that Dup[i] points to elements[j] whose elements[i]->sector is i
        // 'Dup' Pointers are stored in stack and points to heap
        // As 'elements' pointers already point to same Elements in heap ,the heap memeory pointed to at by 'Dup' pointers must NOT be freed(deleted)

        int comboChecker = 0;
        int startingPoint;
        for (int i = 0; i < noElements; i++) {
            if (noElements < 3) {
                comboChecker = 0;
                break;
            }

            if ((dupElements[i]->number) % 5 == 1) {
                startingPoint = i;
                comboChecker = 1;
                while (true) {
                    int CW = (startingPoint + comboChecker) % noElements;
                    int ACW = (startingPoint - comboChecker) % noElements;
                    if (ACW < 0) {
                        ACW = ACW + noElements;
                    }
                    // 'CW' and 'ACW' go round the circle

                    if ((abs(CW - ACW) == 1 || abs(CW - ACW) == noElements - 1) &&
                        dupElements[CW]->number == dupElements[ACW]->number) {
                        break;
                    }

                    if ((dupElements[CW]->number != dupElements[ACW]->number) || CW == ACW) {
                        comboChecker--;
                        break;
                    }

                    if (dupElements[CW]->number == dupElements[ACW]->number) {
                        comboChecker++;
                    }
                }
            }
            if (comboChecker > 0) {
                comboContinue = true;
                break;
            }
        }// comboChecker has the number of mixings for the first-comming number == 'plus(5)' Element only

        if (comboChecker == 0) {
            return;
        }

        // This part is executed only if comboChecker > 0 ----------------------------------------------------------------------------
        Text Mixing(dimension / 2., dimension / 2., "MIXING ELEMENTS");
        Mixing.setColor(COLOR_CHAURESTE);

        int newName = 0;

        // 'score' taken care of
        // startingPoint.'number' is increased as 'newName' but not assigned
        // all combo 'elements[i]' memory is freed ,set to point 'nullptr' , corresponding 'Dup[j]' is set to 'nullptr'
        for (int i = 1; i <= comboChecker; i++) {
            int CW = (startingPoint + i) % noElements;
            int ACW = (startingPoint - i) % noElements;
            if (ACW < 0) {
                ACW = ACW + noElements;
            }

            newName = newName + dupElements[CW]->number + 1;

            /*
            elements[Dup[CW]->pointedByIndex]->circle.scale(1.5);
            elements[Dup[ACW]->pointedByIndex]->circle.scale(1.5);
            wait(0.3);

            elements[Dup[CW]->pointedByIndex]->circle.scale(2/3);
            elements[Dup[ACW]->pointedByIndex]->circle.scale(2/3);
            wait(0.3);*/

            bubblingEffect(elements[dupElements[CW]->pointedByIndex], elements[dupElements[ACW]->pointedByIndex]);

            elements[dupElements[CW]->pointedByIndex] = nullptr;
            delete dupElements[CW];
            dupElements[CW] = nullptr;

            elements[dupElements[ACW]->pointedByIndex] = nullptr;
            delete dupElements[ACW];
            dupElements[ACW] = nullptr;

            score = score + 1;
        }

        dupElements[(startingPoint) % noElements]->updateNameAndColor(newName);

        if (dupElements[0] != nullptr) {
            // cout<<"Dup[0] != nullptr case"<<endl;
            if (startingPoint == 0) {
                for (int i = 1; true; i++) {
                    int point = (startingPoint + comboChecker + i) % noElements;
                    if (dupElements[point] == nullptr)break;
                    if (point == 0)cout << "Some Problem with the Code" << endl;
                    dupElements[point]->sector = dupElements[point]->sector - comboChecker;
                    if (point == (noElements - 1))cout << "Some Problem with the Code" << endl;
                }
            } else if (startingPoint > 0) {
                int SectorNoChanger = dupElements[startingPoint]->sector;
                SectorNoChanger = SectorNoChanger - comboChecker;
                dupElements[startingPoint]->sector = SectorNoChanger;

                for (int i = 1; true; i++) {
                    int point = (startingPoint + comboChecker + i) % noElements;
                    if (point == 0)break;
                    dupElements[point]->sector = dupElements[point]->sector - 2 * comboChecker;
                    if (point == (noElements - 1))break;
                }
            }
        } else if (dupElements[0] == nullptr) {
            // cout<<"Dup[0] == nullptr case"<<endl;
            dupElements[startingPoint]->sector = 0;

            int noTimes = noElements - 2 * comboChecker - 1;
            for (int i = 1; i <= noTimes; i++) {
                int point = (startingPoint + comboChecker + i) % noElements;
                dupElements[point]->sector = i % (noElements - 2 * comboChecker);
            }
        }// Element.'sector' is taken care of

        noElements = noElements - 2 * comboChecker;// 'noElements' is taken care of

        if (noElements == 0) {
            currentSectorAngle = 360;
        }
        if (noElements > 0) {
            currentSectorAngle = 360. / noElements;
        }
        if (noElements < 0) {
            cout << "Some Problem with the Code" << endl;
        }// 'currentSectorAngle' is taken care of

        comboTheElements();


    }

    void comboTheElements() {
        for (int i = 0; i < MAX_ELEMENTS; i++) {
            if (elements[i] != nullptr) {
                elements[i]->setSector(currentSectorAngle);
            }
        }
        if (comboContinue)anyCombo();
    }

    void exitGame() {
        string line;
        highScoreFileInput >> line;
        int length = line.length();
        int HighScore = 0;

        for (int i = 0; i < length; i++) {
            if (line[i] == '0')HighScore = HighScore * 10 + 0;
            else if (line[i] == '1')HighScore = HighScore * 10 + 1;
            else if (line[i] == '2')HighScore = HighScore * 10 + 2;
            else if (line[i] == '3')HighScore = HighScore * 10 + 3;
            else if (line[i] == '4')HighScore = HighScore * 10 + 4;
            else if (line[i] == '5')HighScore = HighScore * 10 + 5;
            else if (line[i] == '6')HighScore = HighScore * 10 + 6;
            else if (line[i] == '7')HighScore = HighScore * 10 + 7;
            else if (line[i] == '8')HighScore = HighScore * 10 + 8;
            else if (line[i] == '9')HighScore = HighScore * 10 + 9;
        }
        highScoreFileInput.close();
        // cout<<HighScore<<endl;

        allScoresFileOutput.open("AllScores.txt", ios::app);
        if (!allScoresFileOutput.is_open()) {
            cout << "Error Opening All Scores file" << endl;
            exit(true);
        }
        string UserNamePrint = "  -  " + userName;
        allScoresFileOutput << score << UserNamePrint << endl;
        allScoresFileOutput.close();

        if (score > HighScore) {
            highScoreFileOutput.open("HighScore.txt");
            if (!highScoreFileOutput.is_open()) {
                cout << "Error Opening HighScore file" << endl;
                exit(true);
            }
            highScoreFileOutput << score << UserNamePrint << endl;
            highScoreFileOutput.close();
        }
        exit(true);
    }


    void bubblingEffect(Element *E1, Element *E2) {
        double x1 = E1->x;
        double y1 = E1->y;

        double x2 = E2->x;
        double y2 = E2->y;

        double radius1 = E1->radius;
        double radius2 = E2->radius;

        float waitTime = 0.0005;

        double tempRadius1 = radius1;
        double tempRadius2 = radius2;

        double range = dimension / 40.;
        int radChanger = 1;
        int halfCycleCounter = 0;
        while (true) {
            E1->circle.reset(x1, y1, tempRadius1);
            E2->circle.reset(x2, y2, tempRadius2);
            tempRadius1 = tempRadius1 + radChanger;
            tempRadius2 = tempRadius2 + radChanger;
            if (tempRadius1 >= radius1 + range || tempRadius2 >= radius2 + range) {
                radChanger = -radChanger;
                halfCycleCounter++;
            }
            if (tempRadius1 <= radius1 - range || tempRadius1 <= radius2 - range) {
                radChanger = -radChanger;
                halfCycleCounter++;
            }
            if (halfCycleCounter == 2 && (tempRadius1 == radius1 || tempRadius2 == radius2))break;
            wait(waitTime);
        }
    }

};
