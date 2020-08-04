#include"board.cpp"

int main() {
    string username;
    cout << "Your name please (but with no spaces)" << endl;
    cin >> username;

    initCanvas("Fusion", WINDOW_SIDE_LENGTH, WINDOW_SIDE_LENGTH);

    Board board = Board(username, 12);
    board.render();
    board.startGameLoop();

    /* closeCanvas(); */
}
