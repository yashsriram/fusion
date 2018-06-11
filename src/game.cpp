#include<simplecpp>
#include<iostream>
#include<fstream>
#include"vector2d.h"
#include<string>

using namespace std;

int dimension;// dimension of Square Canvas
int maxE;// Maximum number of elements on Board (excluding the center one)
double gloX = 0, gloY = 0;// Global X and Y to assign the getClick X and Y
int randomVar = 0;// random variable for color and name
int cR = 1, cG = 2, cB = 3;// random variables for colors
bool comboContinue;

// puts the X and Y value of the Click in 'gloX' , 'gloY'
void registerClick() {
    const int twoPower16 = 65536;
    int point;
    point = getClick();
    gloX = point / (double) twoPower16;
    gloY = point % twoPower16;
}

// (the order of input matters) OUTPUT -pi to pi
double signedSlope(double x1, double y1, double x2, double y2) {
    double cosineOfAngle, angle;
    if ((x2 - x1) != 0) {
        cosineOfAngle = ((x2 - x1) / sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
        angle = arccosine(cosineOfAngle);
        if (y2 - y1 >= 0)return angle;
        if (y2 - y1 < 0)return 360 - angle;
    }


    if ((x2 - x1) == 0) {
        if (y2 - y1 > 0) return 90;
        if (y2 - y1 < 0) return 270;
    }
    return 0;
}

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

    ~Element() {

    }
};

class Board {
    int noE;
    int score;
    int highestNumber;
    Element **E;
    double sectorAngle;
    string userName;
    ifstream highScoreFileInput;
    ofstream highScoreFileOutput;
    ofstream allScoresFileOutput;

public:

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
        highScoreFileInput.open(".HighScore.txt");
        if (!highScoreFileInput.is_open()) {
            cout << "Error Opening HighScore file" << endl;
            exit(true);
        }
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
            E[i] = NULL;
        }// initializes the all 'E[i]' pointers to NULL as no elements are there
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
            if (E[i] != NULL) {
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
        }

        if (noE > 0 && noE < 13) {
            Element newElement;// A new Element comes in the center
            userClick(newElement);

            randomVar = (randomVar + 1) % 5;
        }

    }// Element type var 'newElement' is "GIVEN" a pointer 'noE' ,Element.'sectorNo' and 'sectorAngle' are taken care of

    void userClick(Element newElement) {
        int nullPointerIndex;
        for (nullPointerIndex = 0; nullPointerIndex < maxE; nullPointerIndex++) {
            if (E[nullPointerIndex] == NULL) { break; }
        }
        if (nullPointerIndex == maxE) { nullPointerIndex = -1; }
        // if there is any E[i] pointer left pointing to NULL 'nullPointerIndex' has its index (or) if no such pointer is present then it is set to value -1

        // cout<<nullPointerIndex<<endl;

        E[nullPointerIndex] = new Element;
        *E[nullPointerIndex] = newElement;
        E[nullPointerIndex]->pointedByIndex = nullPointerIndex;
        // E[nullPointerIndex] is set to the new incomming Element
        // 'noE' 'sectorAngle' Element.'sectorNo' are TO BE re-assigned

        if (noE == 0) {
            sectorAngle = 360;
        }

        registerClick();
        Vector2d click(gloX, gloY);
        Vector2d center(dimension / 12, dimension - dimension / 12);

        if (~(center - click) <= dimension / 10) {
            resetHighScore();
        }
        // High Score Insertion >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        double theta;
        theta = signedSlope(dimension / 2, dimension / 2, gloX, gloY);// theta has the 0 to 359 value
        int SectorNoSource;

        SectorNoSource = (theta / sectorAngle);
        E[nullPointerIndex]->sectorNo = SectorNoSource;

        for (int i = 0; i < maxE; i++) {
            if (E[i] != NULL && i != nullPointerIndex) {
                if (E[i]->sectorNo > E[nullPointerIndex]->sectorNo) {
                    E[i]->sectorNo++;
                }
            }
        }

        if (noE != 0) {
            E[nullPointerIndex]->sectorNo++;
        }// Increases all the Element.'sectorNo' by 1 whose value is greater than E[nullPointerIndex]->sectorNo And
        // Also increases E[nullPointerIndex]->sectorNo by 1 for the noE != 0 case

        noE++;
        if (noE == 0) {
            sectorAngle = 360;
        }
        if (noE > 0) {
            sectorAngle = 360.0 / noE;
        }
        if (noE < 0) {
            cout << "Some Problem with the Code" << endl;
        }

        arrTheElements();
    }// A pointer pointing to NULL is declared to point 'Element' type in heap and the 'newElement' is assigned to it
    // noE is also increased ,Element.'sectorNo' is assigned and 'pointedByIndex' and 'sectorAngle' is taken care of

    void anyCombo() {
        Element *Dup[maxE];
        for (int i = 0; i < maxE; i++) {
            Dup[i] = NULL;
        }// All the 'Dup'pointers point to 'NULL'

        for (int i = 0; i < maxE; i++) {
            if (E[i] != NULL) {
                Dup[E[i]->sectorNo] = E[i];
            }
        }// 'Dup' Pointers set such that Dup[i] points to E[j] whose E[i]->sectorNo is i
        // 'Dup' Pointers are stored in stack and points to heap 
        // As 'E' pointers already point to same Elements in heap ,the heap memeory pointed to at by 'Dup' pointers must NOT be freed(deleted)

        int comboChecker = 0;
        int startingPoint;
        for (int i = 0; i < noE; i++) {
            if (noE < 3) {
                comboChecker = 0;
                break;
            }

            if ((Dup[i]->name) % 5 == 1) {
                startingPoint = i;
                comboChecker = 1;
                while (true) {
                    int CW = (startingPoint + comboChecker) % noE;
                    int ACW = (startingPoint - comboChecker) % noE;
                    if (ACW < 0) {
                        ACW = ACW + noE;
                    }
                    // 'CW' and 'ACW' go round the circle 

                    if ((abs(CW - ACW) == 1 || abs(CW - ACW) == noE - 1) && Dup[CW]->name == Dup[ACW]->name) {
                        break;
                    }

                    if ((Dup[CW]->name != Dup[ACW]->name) || CW == ACW) {
                        comboChecker--;
                        break;
                    }

                    if (Dup[CW]->name == Dup[ACW]->name) {
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

        }

        // This part is executed only if comboChecker > 0 ----------------------------------------------------------------------------
        Text Mixing(dimension / 2, dimension / 2, "MIXING ELEMENTS");
        Mixing.setColor(COLOR(60, 226, 10));

        int newName = 0;

        for (int i = 1; i <= comboChecker; i++) {
            int CW = (startingPoint + i) % noE;
            int ACW = (startingPoint - i) % noE;
            if (ACW < 0) {
                ACW = ACW + noE;
            }

            newName = newName + Dup[CW]->name + 1;

            /*
            E[Dup[CW]->pointedByIndex]->circle.scale(1.5);
            E[Dup[ACW]->pointedByIndex]->circle.scale(1.5);
            wait(0.3);
                            
            E[Dup[CW]->pointedByIndex]->circle.scale(2/3);
            E[Dup[ACW]->pointedByIndex]->circle.scale(2/3);
            wait(0.3);*/

            bubblingEffect(E[Dup[CW]->pointedByIndex], E[Dup[ACW]->pointedByIndex]);

            E[Dup[CW]->pointedByIndex] = NULL;
            delete Dup[CW];
            Dup[CW] = NULL;

            E[Dup[ACW]->pointedByIndex] = NULL;
            delete Dup[ACW];
            Dup[ACW] = NULL;

            score = score + 1;
        }// 'score' taken care of
        // startingPoint.'name' is increased as 'newName' but not assigned
        // all combo 'E[i]' memory is freed ,set to point 'NULL' , corresponding 'Dup[j]' is set to 'NULL'

        {
            Dup[(startingPoint) % noE]->updateNameAndColor(newName);
        }// startingPoint.'name' is taken care of

        if (Dup[0] != NULL) {
            // cout<<"Dup[0] != NULL case"<<endl;
            if (startingPoint == 0) {
                for (int i = 1; true; i++) {
                    int point = (startingPoint + comboChecker + i) % noE;
                    if (Dup[point] == NULL)break;
                    if (point == 0)cout << "Some Problem with the Code" << endl;
                    Dup[point]->sectorNo = Dup[point]->sectorNo - comboChecker;
                    if (point == (noE - 1))cout << "Some Problem with the Code" << endl;
                }
            } else if (startingPoint > 0) {
                int SectorNoChanger = Dup[startingPoint]->sectorNo;
                SectorNoChanger = SectorNoChanger - comboChecker;
                Dup[startingPoint]->sectorNo = SectorNoChanger;

                for (int i = 1; true; i++) {
                    int point = (startingPoint + comboChecker + i) % noE;
                    if (point == 0)break;
                    Dup[point]->sectorNo = Dup[point]->sectorNo - 2 * comboChecker;
                    if (point == (noE - 1))break;
                }
            }
        } else if (Dup[0] == NULL) {
            // cout<<"Dup[0] == NULL case"<<endl;
            Dup[startingPoint]->sectorNo = 0;

            int noTimes = noE - 2 * comboChecker - 1;
            for (int i = 1; i <= noTimes; i++) {
                int point = (startingPoint + comboChecker + i) % noE;
                Dup[point]->sectorNo = i % (noE - 2 * comboChecker);
            }
        }// Element.'sectorNo' is taken care of

        noE = noE - 2 * comboChecker;// 'noE' is taken care of

        if (noE == 0) {
            sectorAngle = 360;
        }
        if (noE > 0) {
            sectorAngle = 360.0 / noE;
        }
        if (noE < 0) {
            cout << "Some Problem with the Code" << endl;
        }// 'sectorAngle' is taken care of

        comboTheElements();


    }

    void arrTheElements() {
        for (int i = 0; i < maxE; i++) {
            if (E[i] != NULL) {
                E[i]->resetTheElementUsingSectorNo(sectorAngle);
            }
        }
    }

    void comboTheElements() {
        for (int i = 0; i < maxE; i++) {
            if (E[i] != NULL) {
                E[i]->resetTheElementUsingSectorNo(sectorAngle);
            }
        }
        // cout<<"out of loop"<<endl;
        if (comboContinue)anyCombo();
    }

    int notNullCount() {
        int count = 0;
        for (int i = 0; i < maxE; i++) {
            if (E[i] != NULL) {
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

        allScoresFileOutput.open(".allScoresFileOutput.txt", ios::app);
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

        double waitTime = 0.005;

        double tempRadius1 = radius1;
        double tempRadius2 = radius2;

        double range = dimension / 40;
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

int main() {
    Board A;
}
