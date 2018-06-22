#include"board.cpp"

int main() {
    string username;
    cout << "Enter your number (No spaces):" << endl;
    cin >> username;

    initCanvas("Game0n", WINDOW_SIDE_LENGTH, WINDOW_SIDE_LENGTH);

    Board board = Board(username, 12);
    board.render();
    board.startGameLoop();

//    closeCanvas();
}
