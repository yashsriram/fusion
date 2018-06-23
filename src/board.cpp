#include <simplecpp>
#include "utils.cpp"
#include "rtc.cpp"
#include "io.cpp"

class BoardGraphics {
    Text *noElementsTextView;
    Text *highestNumberTextView;
    Text *scoreTextView;

    const Color TEXT_COLOR = COLOR(0, 0, 0);

public:
    BoardGraphics() : noElementsTextView(nullptr),
                      highestNumberTextView(nullptr),
                      scoreTextView(nullptr) {}

    ~BoardGraphics() {
        delete noElementsTextView;
        delete highestNumberTextView;
        delete scoreTextView;
    }

    void render(const string &username) {
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

    void render(int noElements, int maxAtomicNumberAchieved, int score) {
        noElementsTextView->reset(WINDOW_SIDE_LENGTH - WINDOW_SIDE_LENGTH / 12.,
                                  WINDOW_SIDE_LENGTH - WINDOW_SIDE_LENGTH / 12., noElements);
        highestNumberTextView->reset(WINDOW_SIDE_LENGTH / 12., WINDOW_SIDE_LENGTH / 12., maxAtomicNumberAchieved);
        scoreTextView->reset(WINDOW_SIDE_LENGTH - WINDOW_SIDE_LENGTH / 12., WINDOW_SIDE_LENGTH / 12., score);
    }

    void displayFinalScore(int score) {
        Circle circle(WINDOW_SIDE_LENGTH / 2.0, WINDOW_SIDE_LENGTH / 2.0, WINDOW_SIDE_LENGTH * 4 / 50.0);
        circle.setColor(COLOR(250, 250, 250)).setFill();

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
    }
};

class Board {
    const Vector2d CENTER = Vector2d(WINDOW_SIDE_LENGTH / 2., WINDOW_SIDE_LENGTH / 2.);
    const Vector2d EXIT_CENTER = Vector2d(WINDOW_SIDE_LENGTH / 12., WINDOW_SIDE_LENGTH - WINDOW_SIDE_LENGTH / 12.);
    const int MAX_ELEMENTS;
    default_random_engine generator;

    // state
    int score;
    int maxAtomicNumberAchieved;
    RoundTableConference rtc;

    // graphics
    BoardGraphics graphics;

    // meta data
    string username;

    void refreshStatsBoard() {
        int currentHighest = rtc.getHighest();
        if (currentHighest > maxAtomicNumberAchieved) { maxAtomicNumberAchieved = currentHighest; }
        graphics.render(rtc.getCount(), maxAtomicNumberAchieved, score);
    }

    int generateNewAtomicNumber() {
        // + H He at starting of game
        if (maxAtomicNumberAchieved < 3) { return rand() % 3; }

        // 50% chance of + element if count is > MAX_ELEMENTS - 2
        if (rtc.getCount() > MAX_ELEMENTS - 2 && rand() % 2 == 0) { return 0; }

        // normal distribution
        float mean = maxAtomicNumberAchieved * 0.50f;
        float stddev = 1;
        normal_distribution<float> distribution(mean, stddev);
        int newAtomicNumber = static_cast<int>(distribution(generator));

        // 20% chance of + element in every spawn
        newAtomicNumber = rand() % 10 < 2 ? 0 : newAtomicNumber;

        // 0 <= newAtomicNumber <= maxAtomicNumberAchieved
        if (newAtomicNumber < 0) { return 0; }
        else if (newAtomicNumber > maxAtomicNumberAchieved) { return maxAtomicNumberAchieved; }
        
        return newAtomicNumber;
    }

public:
    Board(string &username, int maxElements) : MAX_ELEMENTS(maxElements),
                                               score(0),
                                               maxAtomicNumberAchieved(0),
                                               username(username) {
        srand(time(NULL));
    }

    void render() {
        graphics.render(username);
    }

    void startGameLoop() {
        refreshStatsBoard();

        Vector2d pointOfClick;
        while (true) {
            rtc.spawn(generateNewAtomicNumber());

            registerClick(&pointOfClick);
            // exit pressed ?
            double distance = Vector2d().setDiffOf(&EXIT_CENTER, &pointOfClick)->length();
            if (distance <= WINDOW_SIDE_LENGTH / 10.) {
                storeScore(username, score);
                break;
            }
            double theta = rayAngle(&CENTER, &pointOfClick);

            rtc.place(theta);
            score += rtc.fuse();
            // rtc.print();
            refreshStatsBoard();
            if (rtc.getCount() > MAX_ELEMENTS) {
                graphics.displayFinalScore(score);
                storeScore(username, score);
                break;
            }
        }
    }
};
