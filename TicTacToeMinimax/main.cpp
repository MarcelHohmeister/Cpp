#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
#include <windows.h>

using namespace std;

vector<int> occupiedCells;

#pragma region Funktionen
void printBoard(const vector<vector<string>>& board);
int mainMenu();
void makePlayerMove(bool player1, vector<vector<string>>& board);
void minimax(vector<vector<string>>& board);
int minimaxRecursive(vector<vector<string>>& board, bool isMaximizing);
void writePieceToBoard(vector<vector<string>>& board, int index, bool player1);
bool boardFull(const vector<vector<string>>& board);
int evaluate(const vector<vector<string>>& board);
#pragma endregion

int main() {
#pragma region Konsoleneinstellungen
    SetConsoleOutputCP(CP_UTF8);  // Ausgabe-Encoding auf UTF-8 setzen
    SetConsoleCP(CP_UTF8);        // Eingabe-Encoding (optional)
#pragma endregion

    vector<vector<string>> board = {
        {" ", " ", " "},
        {" ", " ", " "},
        {" ", " ", " "}
    };
    int selection = mainMenu();
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Eingabepuffer leeren

    bool player1 = true;
    int evaluationScore = 0;

    switch (selection) {
        #pragma region 2 Spieler
        case 1:
            while (true) {
                printBoard(board);
                makePlayerMove(player1, board);

                evaluationScore = evaluate(board);
                if (evaluationScore == 10 || evaluationScore == -10 || boardFull(board)) {
                    printBoard(board);
                    if (evaluationScore == 10) {
                        cout << "Spieler2 (O) hat gewonnen!" << endl;
                    } else if (evaluationScore == -10) {
                        cout << "Spieler1 (X) hat gewonnen!" << endl;
                    } else {
                        cout << "Unentschieden!" << endl;
                    }
                    break;
                }


                player1 = !player1;
            }
            break;
            #pragma endregion

        #pragma region Spiel gegen Minimax Algorithmus
        case 2:
            while (true) {
                printBoard(board);

                if (player1) {
                    makePlayerMove(player1, board);
                } else {
                    cout << "Computer denkt..." << endl;
                    minimax(board);
                }

                evaluationScore = evaluate(board);
                if (evaluationScore == 10 || evaluationScore == -10 || boardFull(board)) {
                    printBoard(board);
                    if (evaluationScore == 10) {
                        cout << "Computer (O) hat gewonnen!" << endl;
                    } else if (evaluationScore == -10) {
                        cout << "Du (X) hast gewonnen!" << endl;
                    } else {
                        cout << "Unentschieden!" << endl;
                    }
                    break;
                }


                player1 = !player1;
            }
            break;
            #pragma endregion

        case 3:
            cout << "Spiel wird beendet...";
            return 0;

        default:
            cout << "Ungültige Auswahl!\n Bitte erneut versuchen";
            break;
    }

    return 1;
}

void minimax(vector<vector<string>>& board) {
    int bestScore = numeric_limits<int>::min();
    int bestMove = -1;

    for (int i = 0; i < 9; i++) {
        int row = i / 3;
        int col = i % 3;

        if (board[row][col] == " ") {
            board[row][col] = "O"; // Computer = O
            int score = minimaxRecursive(board, false);
            board[row][col] = " ";

            if (score > bestScore) {
                bestScore = score;
                bestMove = i;
            }
        }
    }

    if (bestMove != -1) {
        writePieceToBoard(board, bestMove, false);
    }
}

int minimaxRecursive(vector<vector<string>>& board, bool isMaximizing) {
    int score = evaluate(board);

    // Endzustände: Gewinn, Verlust oder Unentschieden
    if (score == 10 || score == -10)
        return score;

    if (boardFull(board))
        return 0;

    if (isMaximizing) {
        int best = numeric_limits<int>::min();

        for (int i = 0; i < 9; i++) {
            int row = i / 3;
            int col = i % 3;

            if (board[row][col] == " ") {
                board[row][col] = "O";
                best = max(best, minimaxRecursive(board, false));
                board[row][col] = " ";
            }
        }
        return best;
    } else {
        int best = numeric_limits<int>::max();

        for (int i = 0; i < 9; i++) {
            int row = i / 3;
            int col = i % 3;

            if (board[row][col] == " ") {
                board[row][col] = "X";
                best = min(best, minimaxRecursive(board, true));
                board[row][col] = " ";
            }
        }
        return best;
    }
}


void makePlayerMove(bool player1, vector<vector<string>>& board) {
    string cell;
    int row, col;

    while (true) {
        cout << (player1 ? "Du (X): " : "Gegner (O): ") << "Welches Feld möchtest du belegen? (1-9): ";
        getline(cin, cell);

        try {
            int index = stoi(cell) - 1;

            if (index < 0 || index > 8) {
                cout << "Ungültige Eingabe. bitte wähle eine Zahl zwischen 1 und 9!\n";
                continue;
            }

            if (board[index / 3][index % 3] != " ") {
                cout << "Dieses Feld ist bereits belegt!\n";
                continue;
            }

            writePieceToBoard(board, index, player1);
            break;
        }
        catch (...) {
            cout << "Ungültige Eingabe. Bitte gib eine Zahl zwischen 1 und 9 ein\n";
        }
    }
}

void writePieceToBoard(vector<vector<string>>& board, int index, bool player1) {
    int row = index / 3;
    int col = index % 3;

    if (board[row][col] == " ") {
        board[row][col] = player1 ? "X" : "O";
        occupiedCells.push_back(1);
    }
}

int evaluate(const vector<vector<string>>& board) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != " " && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0] == "O" ? 10 : -10;

        if (board[0][i] != " " && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i] == "O" ? 10 : -10;
    }

    if (board[0][0] != " " && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0] == "O" ? 10 : -10;

    if (board[0][2] != " " && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2] == "O" ? 10 : -10;

    return 0;  // Noch kein Gewinner
}


bool boardFull(const vector<vector<string>>& board) {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            if (cell == " ")
                return false;
        }
    }
    return true;
}


int mainMenu() {
    cout << "Tic Tac Toe:\n\n";
    cout << "(1) - 2 Spieler\n";
    cout << "(2) - Spiel gegen Minimax Algorithmus\n";
    cout << "(3) - Beenden\n\n";
    cout << "Enter your choice: ";
    int selection;
    cin >> selection;
    return selection;
}

void printBoard(const vector<vector<string>>& board) {
    cout << "\nTic Tac Toe Board:\n\n";

    int cellNumber = 1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == " ") {
                cout << " " << cellNumber << " ";
            } else {
                cout << " " << board[i][j] << " ";
            }

            if (j < 2) cout << "|";
            cellNumber++;
        }
        cout << endl;
        if (i < 2) cout << "---|---|---\n";
    }

    cout << endl;
}
