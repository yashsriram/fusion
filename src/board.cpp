#include <simplecpp>
#include "utils.cpp"
#include "rtc.cpp"
#include "io.cpp"

class Board {
    const Vector2d CENTER = Vector2d(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2.);
    const Vector2d EXIT_CENTER = Vector2d(WINDOW_SIDE_LENGTH / 12., WINDOW_SIDE_LENGTH - WINDOW_SIDE_LENGTH / 12.);
    const int MAX_ELEMENTS;
    const Color TEXT_COLOR = COLOR(0, 0, 0);

    // state
    int score;
    int maxAtomicNumberAchieved;
    RoundTableConference rtc;

    // graphics
    Text *noElementsTextView;
    Text *highestNumberTextView;
    Text *scoreTextView;

    // meta data
    string userName;

    void refreshStatsBoard() {
        noElementsTextView->reset(WINDOW_SIDE_LENGTH - WINDOW_SIDE_LENGTH / 12.,
                                  WINDOW_SIDE_LENGTH - WINDOW_SIDE_LENGTH / 12., rtc.getCount());
        int currentHighest = rtc.getHighest();
        if (currentHighest > maxAtomicNumberAchieved) { maxAtomicNumberAchieved = currentHighest; }
        highestNumberTextView->reset(WINDOW_SIDE_LENGTH / 12., WINDOW_SIDE_LENGTH / 12., maxAtomicNumberAchieved);
        scoreTextView->reset(WINDOW_SIDE_LENGTH - WINDOW_SIDE_LENGTH / 12., WINDOW_SIDE_LENGTH / 12., score);
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
        rtc.place(theta);
    }

    void exitGame() {
        storeScore(userName, score);
        exit(true);
    }

public:
    Board() : MAX_ELEMENTS(12),
              score(0),
              maxAtomicNumberAchieved(0) {
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
        base.setColor(COLOR(187, 222, 251)).setFill();
        base.imprint();

        Circle outerCircle(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2. - 10);
        outerCircle.setColor(COLOR(250, 250, 250)).setFill();
        outerCircle.imprint();

        Circle innerCircle(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 10.);
        innerCircle.setColor(COLOR(224, 247, 250)).setFill();
        innerCircle.imprint();

        Text exitText(WINDOW_SIDE_LENGTH / 12., WINDOW_SIDE_LENGTH * 11 / 12., "EXIT");
        exitText.setColor(TEXT_COLOR).imprint();

        // Text Views
        noElementsTextView = new Text(WINDOW_SIDE_LENGTH * 11 / 12.,
                                      WINDOW_SIDE_LENGTH * (11 / 12.) - WINDOW_SIDE_LENGTH / 50.,
                                      "ELEMENTS ON BOARD");
        noElementsTextView->setColor(TEXT_COLOR).imprint();

        highestNumberTextView = new Text(WINDOW_SIDE_LENGTH / 12., WINDOW_SIDE_LENGTH / 12. - WINDOW_SIDE_LENGTH / 50.,
                                         "HIGHEST ELEMENT ATOMIC No.");
        highestNumberTextView->setColor(TEXT_COLOR).imprint();

        scoreTextView = new Text(WINDOW_SIDE_LENGTH * 11 / 12., WINDOW_SIDE_LENGTH / 12. - WINDOW_SIDE_LENGTH / 50.,
                                 "SCORE");
        scoreTextView->setColor(TEXT_COLOR).imprint();
    }

    void startGameLoop() {
        refreshStatsBoard();

        while (true) {
            rtc.spawn();
            placeNewElement();
            randomVar = (randomVar + 1) % 5;
            score += rtc.fuse();
            // rtc.print();
            refreshStatsBoard();

            if (rtc.getCount() > MAX_ELEMENTS) {
                Text gameOverTextView(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2. - 10, "Game Over o_0");
                gameOverTextView.setColor(TEXT_COLOR);
                wait(2);

                {
                    Text yourScoreIsTextView(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2. + 10, "Your Score is");
                    yourScoreIsTextView.setColor(TEXT_COLOR);
                    wait(2);
                }

                Text finalScoreTextView(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2. + 10, score);
                finalScoreTextView.setColor(TEXT_COLOR);
                wait(2);

                exitGame();
            }
        }
    }
};
