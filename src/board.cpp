#include <simplecpp>
#include <fstream>
#include "element.cpp"
#include "utils.cpp"

const int WINDOW_SIDE_LENGTH = 1000; // WINDOW_SIDE_LENGTH of Square Canvas
const Vector2d CENTER = Vector2d(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2.);
const Vector2d EXIT_CENTER = Vector2d(WINDOW_SIDE_LENGTH / 12., WINDOW_SIDE_LENGTH - WINDOW_SIDE_LENGTH / 12.);

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
        initCanvas("Game0n", WINDOW_SIDE_LENGTH, WINDOW_SIDE_LENGTH);

        Rectangle base(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH, WINDOW_SIDE_LENGTH);
        base.setColor(COLOR(0, 0, 0)).setFill();
        base.imprint();

        Circle outerCircle(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2. - 10);
        outerCircle.setColor(COLOR(20, 20, 20)).setFill();
        outerCircle.imprint();

        Circle innerCircle(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 10.);
        innerCircle.setColor(COLOR(5, 5, 5)).setFill();
        innerCircle.imprint();

        Text exitText(WINDOW_SIDE_LENGTH / 12., WINDOW_SIDE_LENGTH * 11 / 12., "EXIT");
        exitText.setColor(COLOR_CHAURESTE).imprint();

        // Text Views
        noElementsTextView = new Text(WINDOW_SIDE_LENGTH * 11 / 12.,
                                      WINDOW_SIDE_LENGTH * (11 / 12.) - WINDOW_SIDE_LENGTH / 50.,
                                      "ELEMENTS ON BOARD");
        noElementsTextView->setColor(COLOR_CHAURESTE).imprint();

        highestNumberTextView = new Text(WINDOW_SIDE_LENGTH / 12., WINDOW_SIDE_LENGTH / 12. - WINDOW_SIDE_LENGTH / 50.,
                                         "HIGHEST NUMBER ACHIEVED");
        highestNumberTextView->setColor(COLOR_CHAURESTE).imprint();

        scoreTextView = new Text(WINDOW_SIDE_LENGTH * 11 / 12., WINDOW_SIDE_LENGTH / 12. - WINDOW_SIDE_LENGTH / 50.,
                                 "SCORE");
        scoreTextView->setColor(COLOR_CHAURESTE).imprint();
    }

    int getHighestNumber() {
        int highestNumber = 0;
        for (int i = 0; i < MAX_ELEMENTS; i++) {
            if (elements[i] != nullptr) {
                if (elements[i]->name > highestNumber) { highestNumber = elements[i]->name; }
            }
        }
        return highestNumber;
    }

    void startGameLoop() {
        while (true) {
            noElementsTextView->reset(WINDOW_SIDE_LENGTH - WINDOW_SIDE_LENGTH / 12.,
                                      WINDOW_SIDE_LENGTH - WINDOW_SIDE_LENGTH / 12., noElements);
            highestNumberTextView->reset(WINDOW_SIDE_LENGTH / 12., WINDOW_SIDE_LENGTH / 12., getHighestNumber());
            scoreTextView->reset(WINDOW_SIDE_LENGTH - WINDOW_SIDE_LENGTH / 12., WINDOW_SIDE_LENGTH / 12., score);

            int newElementIndex = spawnNewElement();
            placeNewElement(newElementIndex);
            randomVar = (randomVar + 1) % 5;
            checkForCombo();

            if (noElements >= 13) {
                Text GameOver(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2. - 10, "Game Over o_0");
                GameOver.setColor(COLOR_CHAURESTE);
                wait(2);
                {
                    Text FinalScoreMessage(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2. + 10, "Your Score is");
                    FinalScoreMessage.setColor(COLOR_CHAURESTE);
                    wait(2);
                }
                Text FinalScore(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2. + 10, score);
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
        double distance = Vector2d().setDiffOf(&EXIT_CENTER, &pointOfClick)->length();
        if (distance <= WINDOW_SIDE_LENGTH / 10.) {
            exitGame();
        }

        // get sectorNo of new element
        // theta has the 0 to 359 value
        double theta = rayAngle(&CENTER, &pointOfClick);
        int sectorNoSource = theta / currentSectorAngle;
        elements[newElementIndex]->sectorNo = sectorNoSource;
        // push all the next elements to their next sectors
        for (int i = 0; i < MAX_ELEMENTS; i++) {
            if (elements[i] != nullptr && i != newElementIndex) {
                if (elements[i]->sectorNo > elements[newElementIndex]->sectorNo) {
                    elements[i]->sectorNo++;
                }
            }
        }
        // Push the new element to next sectorNo for the noElements != 0 case
        if (noElements != 0) {
            elements[newElementIndex]->sectorNo++;
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

    void checkForCombo() {
        // elementsBySector are such that elementsBySector[i] points to elements[j] whose elements[i]->sectorNo is i
        // elementsBySector are stored in stack and points to heap
        // As elements pointers already point to same Elements in heap, the heap memory pointed to at by elementsBySector pointers must NOT be freed
        Element *elementsBySector[MAX_ELEMENTS];
        for (int i = 0; i < MAX_ELEMENTS; i++) {
            elementsBySector[i] = nullptr;
        }
        for (int i = 0; i < MAX_ELEMENTS; i++) {
            if (elements[i] != nullptr) {
                elementsBySector[elements[i]->sectorNo] = elements[i];
            }
        }

        int comboChecker = 0;
        int startingPoint;
        for (int i = 0; i < noElements; i++) {
            if (noElements < 3) {
                comboChecker = 0;
                break;
            }

            if (elementsBySector[i]->name % 5 == 1) {
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
                        elementsBySector[CW]->name == elementsBySector[ACW]->name) {
                        break;
                    }

                    if ((elementsBySector[CW]->name != elementsBySector[ACW]->name) || CW == ACW) {
                        comboChecker--;
                        break;
                    }

                    if (elementsBySector[CW]->name == elementsBySector[ACW]->name) {
                        comboChecker++;
                    }
                }
            }
            if (comboChecker > 0) {
                comboContinue = true;
                break;
            }
        }
        // comboChecker should have the number of mixes for the first-coming number == 'plus(5)' Element only

        if (comboChecker == 0) {
            return;
        }

        Text mixingTextView(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2., "MIXING ELEMENTS");
        mixingTextView.setColor(COLOR_CHAURESTE);

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

            newName = newName + elementsBySector[CW]->name + 1;

            bubblingEffect(elements[elementsBySector[CW]->pointedByIndex], elements[elementsBySector[ACW]->pointedByIndex]);

            elements[elementsBySector[CW]->pointedByIndex] = nullptr;
            delete elementsBySector[CW];
            elementsBySector[CW] = nullptr;

            elements[elementsBySector[ACW]->pointedByIndex] = nullptr;
            delete elementsBySector[ACW];
            elementsBySector[ACW] = nullptr;

            score = score + 1;
        }

        elementsBySector[(startingPoint) % noElements]->set(newName);

        if (elementsBySector[0] != nullptr) {
            // cout<<"Dup[0] != nullptr case"<<endl;
            if (startingPoint == 0) {
                for (int i = 1; true; i++) {
                    int point = (startingPoint + comboChecker + i) % noElements;
                    if (elementsBySector[point] == nullptr)break;
                    if (point == 0)cout << "Some Problem with the Code" << endl;
                    elementsBySector[point]->sectorNo = elementsBySector[point]->sectorNo - comboChecker;
                    if (point == (noElements - 1))cout << "Some Problem with the Code" << endl;
                }
            } else if (startingPoint > 0) {
                int SectorNoChanger = elementsBySector[startingPoint]->sectorNo;
                SectorNoChanger = SectorNoChanger - comboChecker;
                elementsBySector[startingPoint]->sectorNo = SectorNoChanger;

                for (int i = 1; true; i++) {
                    int point = (startingPoint + comboChecker + i) % noElements;
                    if (point == 0)break;
                    elementsBySector[point]->sectorNo = elementsBySector[point]->sectorNo - 2 * comboChecker;
                    if (point == (noElements - 1))break;
                }
            }
        } else if (elementsBySector[0] == nullptr) {
            // cout<<"Dup[0] == nullptr case"<<endl;
            elementsBySector[startingPoint]->sectorNo = 0;

            int noTimes = noElements - 2 * comboChecker - 1;
            for (int i = 1; i <= noTimes; i++) {
                int point = (startingPoint + comboChecker + i) % noElements;
                elementsBySector[point]->sectorNo = i % (noElements - 2 * comboChecker);
            }
        }// Element.'sectorNo' is taken care of

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

        for (int i = 0; i < MAX_ELEMENTS; i++) {
            if (elements[i] != nullptr) {
                elements[i]->setSector(currentSectorAngle);
            }
        }
        if (comboContinue)checkForCombo();
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

        const float waitTime = 0.0005;
        const double range = WINDOW_SIDE_LENGTH / 40.;
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
