#include "common_def.h"
#include <fstream>

const string HIGHSCORE_FILENAME = "../data/highscore.fusion";
const string ALLSCORES_FILENAME = "../data/allscores.fusion";

void storeScore(const string &username, int score) {
    ifstream highScoreFileInput;
    ofstream highScoreFileOutput;
    ofstream allScoresFileOutput;

    highScoreFileInput.open(HIGHSCORE_FILENAME);
    int highScore = -1;
    if (highScoreFileInput.is_open()) {
        string line;
        highScoreFileInput >> line;
        highScore = stoi(line);
    }
    highScoreFileInput.close();

    if (score > highScore) {
        highScoreFileOutput.open(HIGHSCORE_FILENAME);
        if (!highScoreFileOutput.is_open()) {
            printf("Error opening high score file");
            highScoreFileOutput.close();
            return;
        }
        highScoreFileOutput << score << " " << username << endl;
        highScoreFileOutput.close();
    }

    allScoresFileOutput.open(ALLSCORES_FILENAME, ios::app);
    if (!allScoresFileOutput.is_open()) {
        printf("Error opening all scores file");
        allScoresFileOutput.close();
        return;
    }
    allScoresFileOutput << username << " " << score << endl;
    allScoresFileOutput.close();
}