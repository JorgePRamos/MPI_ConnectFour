#include<iostream>
#include<ctime>
#include "mpi.h"
#include <windows.h>
#include"board.h"
#include<stdio.h>
#include<string.h>
#include<string.h>
#include<math.h>
#include<windows.h>
#include<conio.h>
#include<vector>
#include<map>
char check[4];
char yes[] = "yes";
int i = 0, j = 0, k, n, row, col, value, score_x = 0, score_o = 0, game_mode = 0, number, Highscore, connecter = 0, b = 0, select_sides = 0;
char board[100][100];
char read[10];
char name1[20];
char name2[20];
char save[] = "save";
char sides[] = "replace";
char red[] = "redo";
char undo[] = "undo";
char mains[] = "start";
int position[100];
int redo[100];
const int DEPTH = 6;
const int ROWS = 6;
const int COLS = 7;
Board B;
using namespace std;
boolean runningFlag = true;



int testBoardMatrix[ROWS][COLS] = {
    {0,1,0,0,0,0,0}, // O -> 1
    {0,0,0,0,0,0,0}, // X -> 2
    {0,0,0,0,0,2,0}
};

int printingMatrix[ROWS][COLS] = {
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0}
};

void boarTranslator(int **input) {
    
    for (int i = 0; i < B.rows;i++) {
        for (int j = 0; j < B.cols; j++) {
            printingMatrix[i][j] = input[i][j];
        }
    }
}

// Boards Storage Class
struct Node
{
    boolean revised = false;
    int score = 0;
    int actor = 0;
    int boardState[ROWS][COLS] = {
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0}
    };
    std::map<int, Node> subBoards;

};

std::map<int,Node> boards; // Global collection of boards

//Board printing and Movement printing
void printboard(int start, int state[ROWS][COLS]) {

    system("cls");
    system("COLOR 1A");
    int a, c;
    //Game just started Start = 1
    if (start) {
        //Init with default symbol "*"                                             
        for (a = 0; a < ROWS; a++) {
            for (c = 0; c < COLS; c++)
                board[a][c] = '*';
        }
                   
        //Board to Screen
        for (row = 0; row < ROWS; row++) {        
            for (col = 0; col < COLS; col++) {
                
                printf("| %c ", board[row][col]);
            }
            printf("|\n");
            for (a = 0; a < COLS; a++) printf("____");
            printf("\n");
        }
        for (a = -1; a < COLS-1; a++)printf("%*d", 4, a + 1);
        printf("\n");
    }
    else {

        system("cls");             
        system("COLOR 1A");    
        //Board to Screen-With player moves
        for (row = 0; row < ROWS; row++) {        
            for (col = 0; col < COLS; col++) {

                if (state[row][col] == 1) {
                    printf("| O ");
                }else if (state[row][col] == 2) {
                    printf("| X ");
                }
                else {
                    printf("| * ");
                }
                
            }
            printf("|\n");
            for (a = 0; a < COLS; a++) printf("____");
            printf("\n");
        }
        for (a = -1; a < COLS-1; a++)printf("%*d", 4, a + 1);
        printf("\n");



    }
   
}

// Player movement Input
int playerInput() {
    
    int inputColum;
    boolean incorrect = false;
    do {

        system("COLOR 1A");
        if (incorrect) {
            system("COLOR 1A");
            printf("\n\t-Sorry MR.Player, That movement is illegal.");
        }
        incorrect = true;

        printf("\n\t-MR.Player, Introduce the colum of your next move.\n\t->");
        scanf_s("%d", &inputColum);
    
    } while (!B.MoveLegal(inputColum));
      

    system("COLOR 1A");
    printf("\n\t-Good move indeed MR.Player...\n");

    return inputColum;
}

// Player Greeting
void greeting() {
    //Inicialization of program
    printf("####################################################################\n                              CONNECT FOUR        \n####################################################################\n");
    printf("\n\t-Greetings MR.Player!\n\t-Press <Y> to start.\n\t-Press <N> to close.\n\t->");
    char userInput = ' ';
    boolean incorrect = false;
    do {
        if (incorrect) {
            system("cls");
            printf("####################################################################\n                              CONNECT FOUR        \n####################################################################\n");
            printf("\n\t-Sorry MR.Player, I can't understand you.\n\t-Can you repeat?\n\t->");
        }
        incorrect = true;
        userInput = getchar();


    } while (userInput != 'Y' && userInput != 'y' && userInput != 'N' && userInput != 'n');

    if (userInput == 'n' || userInput == 'N') {
        system("cls");
        printf("####################################################################\n                              CONNECT FOUR        \n####################################################################\n");
        printf("\n\t-Goodbye! Have nice day MR.Player!\n\n\n");
        exit(0);
    }

}


void master(int myRank, int commSize) {

    Node node;
    system("cls");
    system("COLOR 1A");
    // Print empty board
    int start = 1;
    int boardMatrix[ROWS][COLS];
    printboard(start, boardMatrix);
    start = 0;

    int curerntTurn = 0;
    int version = 1;
    // Main Loop
    while (runningFlag) {

        curerntTurn++;

        // Players turn
        int playerMove = playerInput();
        B.Move(playerMove, HUMAN);

        //Print Updated Board
        boarTranslator(B.field);
        printboard(start, printingMatrix);

        // Save current board
        node.actor = HUMAN;
        for (int i = 0; i < B.rows; i++) {
            for (int j = 0; j < B.cols; j++) {
                node.boardState[i][j] = printingMatrix[i][j];
            }
        }
        boards.insert({ curerntTurn,node });

        // Computer turn
        // Simulate 7 moves after the player move
        int iCol;
       for (iCol = 0; iCol < B.Columns(); iCol++) {
            // Restore Player Board
            for (int i = 0; i < B.rows; i++) {
                for (int j = 0; j < B.cols; j++) {
                    B.field[i][j] = boards[curerntTurn].boardState[i][j];
                }
            }
            //Chech if move is legal and perform it if so
            if (B.MoveLegal(iCol))
            {
                B.Move(iCol, CPU);
                //Print the new move
                boarTranslator(B.field);
                printboard(0, printingMatrix); // Curerent boards resieing in printMatrix
                printf("## Version = %d\n", version); // ## Debug
                // Save on node
                node.actor = CPU;
                for (int i = 0; i < B.rows; i++) {
                    for (int j = 0; j < B.cols; j++) {
                        node.boardState[i][j] = printingMatrix[i][j];
                    }
                }
                // Save on list
                boards[curerntTurn].subBoards.insert({ version,node });
                version++;
            }
        }

        /*
        // Simulate 7 moves for each of the 7 simulated CPU moves
        version = 0;
        for (iCol = 0; iCol < B.Columns(); iCol++)
        {
            version = version + 1;
            // Restore Board
            // Will chose boards from turn 1 + version of the board generated = 1.1
            //B.field = (int**)boards.at(curerntTurn + ((iCol++) / 10)).boardState;
            for (int i = 0; i < B.rows; i++) {
                for (int j = 0; j < B.cols; j++) {
                    printf("KEY = %f\n", (curerntTurn + (((version) / 10))));
                    B.field[i][j] = boards.at(curerntTurn + (((version) / 10))).boardState[i][j];// ## NO esta encontrando el elemento en la lista
                }
            }
            if (B.MoveLegal(iCol))
            {
                B.Move(iCol, HUMAN);
            }

            boarTranslator(B.field);
            printboard(0, printingMatrix);
            printf("Aqui se llega\n");
            printf("KEY = %d\n", iCol);
            printf("KEY = %d\n", version);


            node.turn = curerntTurn;
            node.version = iCol++;
            for (int i = 0; i < B.rows; i++) {
                for (int j = 0; j < B.cols; j++) {
                    node.boardState[i][j] = printingMatrix[i][j];
                }
            }
            // Save on list
           // Will save boards from turn 1 + version of the board generated + sub-Version= 1.1.1
            boards.insert({ curerntTurn + ((version) / 10 + ((version) / 100)), node });
        }*/
        printf("## Version = %d\n", version); // ## Debug
        runningFlag = false;
    }
}





    int main(int argc, char** argv)
    {
        //Inicialization of program
        greeting();
        Board();
        //MPI Inizialization 
        enum role { MASTER, SLAVES };
        int myRank, comm_size;
        MPI_Init(&argc, &argv);// ## POSIBLE CHANGE NEEDED FOR ALL PORCESS AVALIBLE
        MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
        MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

        // Check numer of porcess to call
        //if (comm_size != 2) // ## Testing
        if (false)
        {
            printf("This application must be run with X MPI processes.\n");
            MPI_Abort(MPI_COMM_WORLD, -1);
        }


        switch (myRank)
        {
        case MASTER:
        {
            master(myRank, comm_size);
            break;
        }
        default:
        {
            printf("Im in Default\n");
            break;
        }
        }

        MPI_Finalize();
        return 0;

    }
