#include <simplecpp>
#include <fstream>
#include "element.cpp"
#include "utils.cpp"

int dimension;// dimension of Square Canvas
int maxE;// Maximum number of elements on Board (excluding the center one)
int randomVar = 0;// random variable for color and name
int cR = 1, cG = 2, cB = 3;// random variables for colors
bool comboContinue;

struct Board {
    int noE;
    int score;
    int highestNumber;
    Element **E;
    double sectorAngle;
    string userName;
    ifstream highScoreFileInput;
    ofstream highScoreFileOutput;
    ofstream allScoresFileOutput;

    Board() {
        initialize();

        Text noEDisplayBox(dimension - dimension / 12, dimension - dimension / 12 - dimension / 50,
                           "ELEMENTS ON BOARD");
        noEDisplayBox.setColor(COLOR(60, 226, 10));

        Text highestNumberDisplayBox(dimension / 12, dimension / 12 - dimension / 50, "HIGHEST NUMBER ACHIEVED");
        highestNumberDisplayBox.setColor(COLOR(60, 226, 10));

        Text scoreDisplayBox(dimension - dimension / 12, dimension / 12 - dimension / 50, "SCORE");
        scoreDisplayBox.setColor(COLOR(60, 226, 10));

        Circle exitBox(dimension / 12, dimension - dimension / 12, dimension / 10);
        Text Exit(dimension / 12, dimension - dimension / 12, "EXIT");
        Exit.setColor(COLOR(60, 226, 10));

        while (true) {
            Text noEDisplay(dimension - dimension / 12, dimension - dimension / 12, noE);
            noEDisplay.setColor(COLOR(60, 226, 10));

            Text highestNumberDisplay(dimension / 12, dimension / 12, highestNumber);
            highestNumberDisplay.setColor(COLOR(60, 226, 10));

            Text scoreDisplay(dimension - dimension / 12, dimension / 12, score);
            scoreDisplay.setColor(COLOR(60, 226, 10));

            randomNewElement();
            anyCombo();
            resetHighestNumber();

            if (noE >= 13) {
                Text GameOver(dimension / 2, dimension / 2 - 10, "Game Over o_0");
                GameOver.setColor(COLOR(60, 226, 10));
                wait(2);
                {
                    Text FinalScoreMessage(dimension / 2, dimension / 2 + 10, "Your Score is");
                    FinalScoreMessage.setColor(COLOR(60, 226, 10));
                    wait(2);
                }
                Text FinalScore(dimension / 2, dimension / 2 + 10, score);
                FinalScore.setColor(COLOR(60, 226, 10));

                wait(2);

                resetHighScore();
            }
        }
    }

    void initialize() {
        noE = 0;
        score = 0;
        highestNumber = 0;
        maxE = 13;
        sectorAngle = 0;
        //
        // cout<<"Enter the dimension"<<endl;
        // cin>>dimension;
        dimension = 1000;
        cout << "Enter your Name (No spaces)" << endl;
        cin >> userName;
        initCanvas("Game0n", dimension, dimension);
        // square canvas of sidelenght 'dimension'

        E = new Element *[maxE];
        for (int i = 0; i < maxE; i++) {
            E[i] = nullptr;
        }// initializes the all 'E[i]' pointers to nullptr as no elements are there
        Rectangle B0(dimension / 2, dimension / 2, dimension, dimension);
        B0.setColor(COLOR(0, 0, 0));
        B0.setFill(1);
        B0.imprint();
        Circle B1(dimension / 2, dimension / 2, dimension / 2 - 10);
        B1.setColor(COLOR(20, 20, 20));
        B1.setFill(1);
        B1.imprint();
        Circle B2(dimension / 2, dimension / 2, dimension / 10);
        B2.setColor(COLOR(5, 5, 5));
        B2.setFill(1);
        B2.imprint();

        // B1 B2 are boundary circles

    }// inititalizes the 'Board' part

    void resetHighestNumber() {
        for (int i = 0; i < maxE; i++) {
            if (E[i] != nullptr) {
                if (E[i]->name > highestNumber) highestNumber = E[i]->name;
            }
        }
    }

    void randomNewElement() {
        // Max number of elements are 12. A transition of 13 is possible if it Decreases 'noE' (or) the Game Ends then
        if (noE == 0) {
            Element newElement;// A new Element Comes in the center
            userClick(newElement);

            randomVar = (randomVar + 1) % 5;
            return;
        }

        if (noE > 0 && noE < 13) {
            Element newElement;// A new Element comes in the center
            userClick(newElement);

            randomVar = (randomVar + 1) % 5;
            return;
        }
    }// Element type var 'newElement' is "GIVEN" a pointer 'noE' ,Element.'sector' and 'sectorAngle' are taken care of

    void userClick(Element newElement) {
        int nullPointerIndex;
        for (nullPointerIndex = 0; nullPointerIndex < maxE; nullPointerIndex++) {
            if (E[nullPointerIndex] == nullptr) { break; }
        }
        if (nullPointerIndex == maxE) { nullPointerIndex = -1; }
        // if there is any E[i] pointer left pointing to nullptr 'nullPointerIndex' has its index (or) if no such pointer is present then it is set to value -1

        // cout<<nullPointerIndex<<endl;

        E[nullPointerIndex] = new Element;
        *E[nullPointerIndex] = newElement;
        E[nullPointerIndex]->pointedByIndex = nullPointerIndex;
        // E[nullPointerIndex] is set to the new incomming Element
        // 'noE' 'sectorAngle' Element.'sector' are TO BE re-assigned

        if (noE == 0) {
            sectorAngle = 360;
        }

        Vector2d pointOfClick;
        registerClick(&pointOfClick);
        Vector2d center(dimension / 12., dimension - dimension / 12.);
        double distance = Vector2d().setDiffOf(&center, &pointOfClick)->mod();

        if (distance <= dimension / 10.) {
            resetHighScore();
        }

        // High Score Insertion
        double theta;
        theta = rayAngle(dimension / 2., dimension / 2., pointOfClick.x, pointOfClick.y); // theta has the 0 to 359 value
        int sectorNoSource;

        sectorNoSource = (theta / sectorAngle);
        E[nullPointerIndex]->sector = sectorNoSource;

        for (int i = 0; i < maxE; i++) {
            if (E[i] != nullptr && i != nullPointerIndex) {
                if (E[i]->sector > E[nullPointerIndex]->sector) {
                    E[i]->sector++;
                }
            }
        }

        if (noE != 0) {
            E[nullPointerIndex]->sector++;
        }// Increases all the Element.'sector' by 1 whose value is greater than E[nullPointerIndex]->sector And
        // Also increases E[nullPointerIndex]->sector by 1 for the noE != 0 case

        noE++;
        if (noE == 0) {
            sectorAngle = 360;
        }
        if (noE > 0) {
            sectorAngle = 360. / noE;
        }
        if (noE < 0) {
            cout << "Some Problem with the Code" << endl;
        }

        arrTheElements();
    }// A pointer pointing to nullptr is declared to point 'Element' type in heap and the 'newElement' is assigned to it
    // noE is also increased ,Element.'sector' is assigned and 'pointedByIndex' and 'sectorAngle' is taken care of

    void anyCombo() {
        Element *dupElements[maxE];
        for (int i = 0; i < maxE; i++) {
            dupElements[i] = nullptr;
        }// All the 'Dup'pointers point to 'nullptr'

        for (int i = 0; i < maxE; i++) {
            if (E[i] != nullptr) {
                dupElements[E[i]->sector] = E[i];
            }
        }// 'Dup' Pointers set such that Dup[i] points to E[j] whose E[i]->sector is i
        // 'Dup' Pointers are stored in stack and points to heap
        // As 'E' pointers already point to same Elements in heap ,the heap memeory pointed to at by 'Dup' pointers must NOT be freed(deleted)

        int comboChecker = 0;
        int startingPoint;
        for (int i = 0; i < noE; i++) {
            if (noE < 3) {
                comboChecker = 0;
                break;
            }

            if ((dupElements[i]->name) % 5 == 1) {
                startingPoint = i;
                comboChecker = 1;
                while (true) {
                    int CW = (startingPoint + comboChecker) % noE;
                    int ACW = (startingPoint - comboChecker) % noE;
                    if (ACW < 0) {
                        ACW = ACW + noE;
                    }
                    // 'CW' and 'ACW' go round the circle

                    if ((abs(CW - ACW) == 1 || abs(CW - ACW) == noE - 1) &&
                        dupElements[CW]->name == dupElements[ACW]->name) {
                        break;
                    }

                    if ((dupElements[CW]->name != dupElements[ACW]->name) || CW == ACW) {
                        comboChecker--;
                        break;
                    }

                    if (dupElements[CW]->name == dupElements[ACW]->name) {
                        comboChecker++;
                    }
                }
            }
            if (comboChecker > 0) {
                comboContinue = true;
                break;
            }
        }// comboChecker has the number of mixings for the first-comming name == 'plus(5)' Element only

        if (comboChecker == 0) {
            return;
        }

        // This part is executed only if comboChecker > 0 ----------------------------------------------------------------------------
        Text Mixing(dimension / 2, dimension / 2, "MIXING ELEMENTS");
        Mixing.setColor(COLOR(60, 226, 10));

        int newName = 0;

        // 'score' taken care of
        // startingPoint.'name' is increased as 'newName' but not assigned
        // all combo 'E[i]' memory is freed ,set to point 'nullptr' , corresponding 'Dup[j]' is set to 'nullptr'
        for (int i = 1; i <= comboChecker; i++) {
            int CW = (startingPoint + i) % noE;
            int ACW = (startingPoint - i) % noE;
            if (ACW < 0) {
                ACW = ACW + noE;
            }

            newName = newName + dupElements[CW]->name + 1;

            /*
            E[Dup[CW]->pointedByIndex]->circle.scale(1.5);
            E[Dup[ACW]->pointedByIndex]->circle.scale(1.5);
            wait(0.3);

            E[Dup[CW]->pointedByIndex]->circle.scale(2/3);
            E[Dup[ACW]->pointedByIndex]->circle.scale(2/3);
            wait(0.3);*/

            bubblingEffect(E[dupElements[CW]->pointedByIndex], E[dupElements[ACW]->pointedByIndex]);

            E[dupElements[CW]->pointedByIndex] = nullptr;
            delete dupElements[CW];
            dupElements[CW] = nullptr;

            E[dupElements[ACW]->pointedByIndex] = nullptr;
            delete dupElements[ACW];
            dupElements[ACW] = nullptr;

            score = score + 1;
        }

        dupElements[(startingPoint) % noE]->updateNameAndColor(newName);

        if (dupElements[0] != nullptr) {
            // cout<<"Dup[0] != nullptr case"<<endl;
            if (startingPoint == 0) {
                for (int i = 1; true; i++) {
                    int point = (startingPoint + comboChecker + i) % noE;
                    if (dupElements[point] == nullptr)break;
                    if (point == 0)cout << "Some Problem with the Code" << endl;
                    dupElements[point]->sector = dupElements[point]->sector - comboChecker;
                    if (point == (noE - 1))cout << "Some Problem with the Code" << endl;
                }
            } else if (startingPoint > 0) {
                int SectorNoChanger = dupElements[startingPoint]->sector;
                SectorNoChanger = SectorNoChanger - comboChecker;
                dupElements[startingPoint]->sector = SectorNoChanger;

                for (int i = 1; true; i++) {
                    int point = (startingPoint + comboChecker + i) % noE;
                    if (point == 0)break;
                    dupElements[point]->sector = dupElements[point]->sector - 2 * comboChecker;
                    if (point == (noE - 1))break;
                }
            }
        } else if (dupElements[0] == nullptr) {
            // cout<<"Dup[0] == nullptr case"<<endl;
            dupElements[startingPoint]->sector = 0;

            int noTimes = noE - 2 * comboChecker - 1;
            for (int i = 1; i <= noTimes; i++) {
                int point = (startingPoint + comboChecker + i) % noE;
                dupElements[point]->sector = i % (noE - 2 * comboChecker);
            }
        }// Element.'sector' is taken care of

        noE = noE - 2 * comboChecker;// 'noE' is taken care of

        if (noE == 0) {
            sectorAngle = 360;
        }
        if (noE > 0) {
            sectorAngle = 360. / noE;
        }
        if (noE < 0) {
            cout << "Some Problem with the Code" << endl;
        }// 'sectorAngle' is taken care of

        comboTheElements();


    }

    void arrTheElements() {
        for (int i = 0; i < maxE; i++) {
            if (E[i] != nullptr) {
                E[i]->setSector(sectorAngle);
            }
        }
    }

    void comboTheElements() {
        for (int i = 0; i < maxE; i++) {
            if (E[i] != nullptr) {
                E[i]->setSector(sectorAngle);
            }
        }
        if (comboContinue)anyCombo();
    }

    int notNullCount() {
        int count = 0;
        for (int i = 0; i < maxE; i++) {
            if (E[i] != nullptr) {
                count++;
            }
        }
        return count;
    }

    void resetHighScore() {
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

    ~Board() {
        delete[] E;
        closeCanvas();
    }
};
