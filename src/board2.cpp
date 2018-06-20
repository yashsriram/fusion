#include <simplecpp>
#include <fstream>
#include "utils.cpp"
#include "rtc.cpp"

struct Board {
    /**
     * Max number of stable elements are MAX_ELEMENTS - 1
     * A transition of MAX_ELEMENTS elements is possible if it creates a combo and fusion occurs
     * else the game ends on MAX_ELEMENTS elements
     * excluding the center one
     * */
    const Vector2d CENTER = Vector2d(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2.);
    const Vector2d EXIT_CENTER = Vector2d(WINDOW_SIDE_LENGTH / 12., WINDOW_SIDE_LENGTH - WINDOW_SIDE_LENGTH / 12.);
    const int MAX_ELEMENTS;
    const Color COLOR_CHAURESTE = COLOR(60, 226, 10);

    // state
    int noElements;
    int score;
    double currentSectorAngle;
    bool comboContinue;
    RoundTableConference rtc;

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
              comboContinue(false) {
    }


    ~Board() {
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

    void startGameLoop() {
        while (true) {
            noElementsTextView->reset(WINDOW_SIDE_LENGTH - WINDOW_SIDE_LENGTH / 12.,
                                      WINDOW_SIDE_LENGTH - WINDOW_SIDE_LENGTH / 12., noElements);
            highestNumberTextView->reset(WINDOW_SIDE_LENGTH / 12., WINDOW_SIDE_LENGTH / 12., "TODO");
            scoreTextView->reset(WINDOW_SIDE_LENGTH - WINDOW_SIDE_LENGTH / 12., WINDOW_SIDE_LENGTH / 12., score);

            rtc.spawn();
            placeNewElement();
            randomVar = (randomVar + 1) % 5;
            score += rtc.fuse();
            rtc.print();

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

    void placeNewElement() {
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
        rtc.placeNewElement(theta);
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
};
