// Tic-Tac-Toe 2.0
// Joga o jogo da velha contra um oponente humano
// Usa ponteiros em vez de referências para parâmetros de função

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// global constants

const char X = 'X';
const char O = 'O';
const char EMPTY = ' ';
const char TIE = 'T';
const char NO_ONE = 'N';

// funtion prototypes
void instructions();
char askYesNo(string question);
int askNumber(string question, int high, int low = 0);
char humanPiece();
char opponent(char piece);
void displayBoard(const vector<char>* const pBoard);
char winner(const vector<char>* const pBoard);
bool isLegal(const vector<char>* const pBoard, int move);
int humanMove(const vector<char>* const pBoard, char human);
int computerMove(vector<char> board, char computer);
void announceWinner(char winner, char computer, char human);

// main funtion
int main(){
    int move;
    const int NUM_SQUARES = 9;
    vector<char> board(NUM_SQUARES, EMPTY);

    instructions();
    char human = humanPiece();
    char computer = opponent(human);
    char turn = X;
    displayBoard(&board);

    while (winner(&board) == NO_ONE)
    {
        if (turn == human)
        {
            move = humanMove(&board, human);
            board[move] = human;
        }
        else
        {
            move = computerMove(board, computer);
            board[move] = computer;
        }
        displayBoard(&board);
        turn = opponent(turn);
    }

    announceWinner(winner(&board), computer, human);
    return 0;
    
}

// functions

void instructions(){
    cout << "Bem-vindo ao confronto homem-maquina final: Tic-Tac-Toe.\n";
    cout << "--onde o cerebro humano eh colocado contra o processador de silicio\n\n";

    cout << "Torne sua jogada conhecida digitando um numero, 0 - 8. O numero\n";
    cout << "corresponde a posição desejada do tabuleiro, conforme ilustrado:\n\n";

    cout << "         0 | 1 | 2\n";
    cout << "         ---------\n";
    cout << "         3 | 4 | 5\n";
    cout << "         ---------\n";
    cout << "         6 | 7 | 8\n\n";

    cout << "Prepare-se, humano. A batalha esta prestes a começar.\n\n";
    
}

char askYesNo(string question){
    char response;
    do{
        cout << question << " (y/n): ";
        cin >> response;
    } while (response != 'y' && response != 'n');
    return response;
}

int askNumber(string question, int high, int low){
    int number;
    do
    {
        cout << question << " (" << low << " - " << high << "): ";
        cin >> number;
    } while (number > high || number < low);
    return number;
}

char humanPiece(){
    char go_first = askYesNo("Você precisa do primeiro movimento?");
    if (go_first == 'y'){
        cout << "\nEntao de o primeiro passo. Você vai precisar dele.\n";
        return X;
    }
    else{
        cout << "\nSua bravura sera sua ruina... eu irei primeiro.\n";
        return 0;
    }


}

char opponent(char piece){
    if(piece == X){
        return O;
    }else{
        return X;
    }
}

void displayBoard(const vector<char>* const pBoard){
    cout << "\n\t" << (*pBoard)[0] << " | " << (*pBoard)[1] << " | " << (*pBoard)
    [2];
    cout << "\n\t" << "---------";
    cout << "\n\t" << (*pBoard)[3] << " | " << (*pBoard)[4] << " | " << (*pBoard)
    [5];
    cout << "\n\t" << "---------";
    cout << "\n\t" << (*pBoard)[6] << " | " << (*pBoard)[7] << " | " << (*pBoard)
    [8];
    cout << "\n\n";
}

char winner(const vector<char>* const pBoard){
    // all posssible winning rows
    const int WINNING_ROWS[8][3] = {{0, 1, 2},
                                    {3, 4, 5},
                                    {6, 7, 8},
                                    {0, 3, 6},
                                    {1, 4, 7},
                                    {2, 5, 8},
                                    {0, 4, 8},
                                    {2, 4, 6}};

    const int TOTAL_ROWS = 8;

    // if any winning row has three values that are the same (and not EMPTY),
    // thes we have a winner
    for(int row = 0; row < TOTAL_ROWS; ++row){

        if ( ((*pBoard)[WINNING_ROWS[row][0]] != EMPTY) && 
            ((*pBoard)[WINNING_ROWS[row][0]] == (*pBoard)[WINNING_ROWS[row][1]]) &&
            ((*pBoard)[WINNING_ROWS[row][1]] == (*pBoard)[WINNING_ROWS[row][2]]))
        {
        return (*pBoard)[WINNING_ROWS[row][0]];
        }
    }

    // since nobody won, check for a tie (no empty squeres left)
    if (count(pBoard->begin(), pBoard->end(), EMPTY) == 0)
        return TIE;

    // since nobody has won  and it isn'tc a tie, the game ain't over
    return NO_ONE;
}

inline bool isLegal(int move, const vector<char>* pBoard){
    return ((*pBoard)[move] == EMPTY);
}

int humanMove(const vector<char>* const pBoard, char human){
    int move = askNumber("Onde você vai se mudar?", (pBoard->size() - 1));
    while (!isLegal(move, pBoard))
    {
        cout << "\n Essa praça já está ocupada, humano tolo.\n";
        move = askNumber("Onde você vai se mudar? ", (pBoard->size() - 1));
    }
    cout << "Multar...\n";
    return move;
}

int computerMove(vector<char> board, char computer){
    unsigned int move = 0;
    bool found = false;

    // if computer con win on next move, thats the move to make
    while (!found && move < board.size()){
       if (isLegal(move, &board)){
        // try move
        board[move] = computer;
        //test for winner
        found = winner(&board) == computer;
        //undo move
        board[move] = EMPTY;
       }

       if (!found){
            ++move;
       }
    }

    // otherwise, if opponent can win on next move, that's the move to make
    if (!found){
        move = 0;
        char human = opponent(computer);
        while (!found && move < board.size()){
            if (isLegal(move, &board)){
                // try move
                board[move] = human;
                //test for winner
                found = winner(&board) == human;
                //undo move
                board[move] = EMPTY;
                }

            if (!found){
                ++move;
            } 
        }
    }

    // otherwise, moving to the best open square is the move to make
    if (!found){
        move = 0;
        unsigned int i = 0;

        const int BEST_MOVES[] = {4, 0, 2, 6, 8, 1, 3, 5, 7};
        //pick best open square

        while (!found && i < board.size()){
            move = BEST_MOVES[i];
            if (isLegal(move, &board)){
                found = true;
            }
            ++i;
        }
    }

    cout << "vou pegar o número quadrado "<< move << endl;
    return move;
}

void announceWinner(char winner, char computer, char human){
    if (winner == computer){
        cout << winner << " está ganho!\n";
        cout << "Como previ, humano, estou triunfante mais uma vez - prova\n";
        cout << "Que os computadores são superiores aos humanos em todos os aspectos.\n";
    }
    
    else if(winner == human){
        cout << winner << " está ganho!\n";
        cout << "Não não! Não pode ser! De alguma forma você me enganou, humano.\n";
        cout << "Mas nunca mais! o computador, então juro!\n";
    }
    
    else{
        cout << "É um empate.\n";
        cout <<" Você teve muita sorte, humano, e de alguma forma conseguiu me amarrar.\n";
        cout << "Comemore... pois isso é o que você jamais alcançará.\n";
    }
}
    




