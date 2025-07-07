#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
#include <vector>
#include <random>

using namespace std;
using namespace std::chrono_literals;

#pragma region board Settings
int height = 20;
int width = 20;
float spawnProbability = 0.5;

vector<vector<bool>> board(height, vector<bool>(width, false));
#pragma endregion

#pragma region Game Settings
bool limitRounds = false;   //Limits the amount of rounds
int maxRounds = 30;         //Amount of Rounds, when limitRounds == true
float delayAmount = 0.5;    //Delay in Seconds

int currentRounds = 0;
#pragma endregion

#pragma region Zufallszahlen //Verwendung mit Coinflip(rng) -> gibt true oder false zurück
std::random_device rd;
std::mt19937 rng(rd());
std::bernoulli_distribution coinflip(spawnProbability);  // 50% Wahrscheinlichkeit
#pragma endregion

#pragma region Funktionen
void fillboard();
void placeGlider(int x, int y);
void placeMirroredGlider(int x, int y);
int countNeighbours(int row, int cell);
bool manageSurvival(int neighbours, bool isAlive);
void drawBoard();
void startDelay();
#pragma endregion


int main() {
    #pragma region Console Settings
    SetConsoleOutputCP(CP_UTF8);  // Ausgabe-Encoding auf UTF-8 setzen
    SetConsoleCP(CP_UTF8);        // Eingabe-Encoding (optional)
    #pragma endregion

    //initial Board
    fillboard();
    //placeGlider(3, 3);
    //placeMirroredGlider(3, width - 5);
    drawBoard();
    currentRounds++;
    startDelay();

    while (true) {
        if (limitRounds && currentRounds >= maxRounds) {
            break;
        }
        else {
            vector<vector<bool>> newBoard(height, vector<bool>(width, false));

            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    int neighbours = countNeighbours(i, j);
                    newBoard[i][j] = manageSurvival(neighbours, board[i][j]);
                }
            }

            board = newBoard;

            drawBoard();
            currentRounds++;
            startDelay();
        }
    }
}

void fillboard() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            board[i][j] = coinflip(rng);
        }
    }
}

void placeGlider(int x, int y) {
    board[x][y + 1] = true;board[x + 1][y + 2] = true;
    board[x + 2][y] = true;
    board[x + 2][y + 1] = true;
    board[x + 2][y + 2] = true;
}

void placeMirroredGlider(int x, int y) {
    // Setze die gespiegelten Glider-Zellen für die Position (x, y)
    board[x][y - 1] = true;
    board[x + 1][y - 2] = true;
    board[x + 2][y] = true;
    board[x + 2][y - 1] = true;
    board[x + 2][y - 2] = true;
}


int countNeighbours(int row, int cell) {
    int amount = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (!(i == 0 && j == 0)){   //Nicht sich selbst prüfen
                int newRow = (row + i + height) % height;
                int newCell = (cell + j + width) % width;

                if (newRow >= 0 && newRow < height && newCell >= 0 && newCell < width) {
                    if (board[newRow][newCell]) {
                        amount++;
                    }
                }
            }
        }
    }

    return amount;
}

bool manageSurvival(int neighbours, bool isAlive) {
    if (isAlive) {
        return neighbours == 2 || neighbours == 3; //überlebt
    }
    else {
        return neighbours == 3; //wird geboren
    }
}

void drawBoard() {
    cout << "Round: " << currentRounds << endl;

    for (vector<bool> rows : board) {
        for (bool cell : rows) {
            cout << (cell ? "⬛" : "⬜");
        }
        cout << endl;
    }
    cout << endl;
}

void startDelay() {
    this_thread::sleep_for(delayAmount * 1s);
}