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

#define REQUEST 1
#define RESPONSE 2
#define TASK_FINISH 3
const int DEPTH = 4;
const int ROWS = 6;
const int COLS = 7;
Board B;
using namespace std;
boolean runningMaterFlag = true;
boolean runningSlaveFlag = false;
char board[100][100];
int  row, col;


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

// Data communication Class
struct NodePack
{
    int actor = 0;
    int lastCol = 0;
    int lastHeight[COLS];
    int boardState[ROWS][COLS] = {
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0}
    };
};

    // Boards Storage Class
    struct Node
    {
        boolean revised = false;
        int score = 0;
        int actor = 0;
        int lastCol = 0;
        int lastHeight[COLS];
        int boardState[ROWS][COLS] = {
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0}
        };
        // Sub boards list for each of the moves
        std::map<int, Node> subBoards;

    };

    std::map<int, Node> boards; // Global collection of boards

    // recursive function : examines all possible moves and returns an estimate of the obtained state of the board
        // Current: current state of the board
        // LastMover: HUMAN or CPU
        // iLastCol: previous move column
        // iDepth: depth is reduced to 
    double Evaluate(Board Current, dat LastMover, int iLastCol, int iDepth){
        double dResult, dTotal;
        dat NewMover;
        bool bAllLose = true, bAllWin = true;
        int iMoves;

        if (Current.GameEnd(iLastCol))	// igra gotova?
            if (LastMover == CPU)
                return 1;	// pobjeda
            else //if(LastMover == HUMAN)
                return -1;	// poraz
        // nije gotovo, idemo u sljedecu razinu
        if (iDepth == 0)
            return 0;	// a mozda i ne... :)
        iDepth--;
        if (LastMover == CPU)	// tko je na potezu
            NewMover = HUMAN;
        else
            NewMover = CPU;
        dTotal = 0;
        iMoves = 0;	// broj mogucih poteza u ovoj razini
        for (int iCol = 0; iCol < Current.Columns(); iCol++)
        {
            if (Current.MoveLegal(iCol))	// jel moze u stupac iCol
            {
                iMoves++;
                Current.Move(iCol, NewMover);

                dResult = Evaluate(Current, NewMover, iCol, iDepth);
                Current.UndoMove(iCol);
                if (dResult > -1)
                    bAllLose = false;
                if (dResult != 1)
                    bAllWin = false;
                if (dResult == 1 && NewMover == CPU)
                    return 1;	// ako svojim potezom mogu doci do pobjede (pravilo 1)
                if (dResult == -1 && NewMover == HUMAN)
                    return -1;	// ako protivnik moze potezom doci do pobjede (pravilo 2)
                //if(dResult > 0)	// izgleda da je bolje ako se u racunanju uzimaju i porazi ...
                dTotal += dResult;
            }
        }
        if (bAllWin == true)	// ispitivanje za pravilo 3.
            return 1;
        if (bAllLose == true)
            return -1;
        dTotal /= iMoves;	// divide the rating by the number of possible moves from the given state
        return dTotal;
    }

    //Board printing and Movement printing
    void printboard(int start, int state[ROWS][COLS]) {

        //system("cls");
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
            for (a = -1; a < COLS - 1; a++)printf("%*d", 4, a + 1);
            printf("\n");
        }
        else {

            //system("cls");             
            system("COLOR 1A");
            //Board to Screen-With player moves
            for (row = 0; row < ROWS; row++) {
                for (col = 0; col < COLS; col++) {

                    if (state[row][col] == 1) {
                        printf("| O ");
                    }
                    else if (state[row][col] == 2) {
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
            for (a = -1; a < COLS - 1; a++)printf("%*d", 4, a + 1);
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

    // Master Function
    void master(int myRank, int commSize) {

        // Creation of Struct MPI Data type

       // Number of data items in Struct
        const int nitems = 4;
        // {actor, lastCol, lastHeight, boardState}
        int          blocklengths[nitems] = { 1, 1, COLS, (ROWS * COLS) };
        MPI_Datatype types[nitems] = { MPI_INT, MPI_INT, MPI_INT, MPI_INT };
        MPI_Datatype mpi_nodePack;

        MPI_Aint     offsets[nitems];
        offsets[0] = offsetof(NodePack, actor);
        offsets[1] = offsetof(NodePack, lastCol);
        offsets[2] = offsetof(NodePack, lastHeight);
        offsets[3] = offsetof(NodePack, boardState);
        // Commit of MPI Data Structure
        MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_nodePack);
        MPI_Type_commit(&mpi_nodePack);
        Node node;
        NodePack bufferNode;
        int bufferInt;
        double bufferDouble;
        MPI_Status stat;
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
        while (runningMaterFlag) {
            int numberBoards = 0; // ##
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
            node.lastCol = B.lastcol;
            for (int u = 0; u < COLS; u++) {
                node.lastHeight[u] = B.height[u];
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
                B.lastcol = boards[curerntTurn].lastCol;
                for (int u = 0; u < COLS; u++) {
                    B.height[u] = boards[curerntTurn].lastHeight[u];
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
                    node.lastCol = B.lastcol;
                    for (int u = 0; u < COLS; u++) {
                        node.lastHeight[u] = B.height[u];
                    }
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


            // Simulate 7 moves for each of the 7 simulated CPU moves
             // ## Scalable¿?

            for (int x = 1; x <= boards[curerntTurn].subBoards.size(); x++) { // ## < ¿?  <= 
                version = 1;
                for (iCol = 0; iCol < B.Columns(); iCol++) {
                    numberBoards++;
                    // Restore Board
                    for (int i = 0; i < B.rows; i++) {
                        for (int j = 0; j < B.cols; j++) {
                            B.field[i][j] = boards[curerntTurn].subBoards[x].boardState[i][j];
                        }
                    }
                    B.lastcol = boards[curerntTurn].subBoards[x].lastCol;
                    for (int u = 0; u < COLS; u++) {
                        B.height[u] = boards[curerntTurn].subBoards[x].lastHeight[u];
                    }

                    if (B.MoveLegal(iCol))
                    {
                        B.Move(iCol, HUMAN);
                        //Print the new move
                        boarTranslator(B.field);
                        printboard(0, printingMatrix); // Curerent boards resieing in printMatrix
                        printf("## Version = %d\n", version); // ## Debug
                        for (int i = 0; i < B.rows; i++) { // ## Debug
                            for (int j = 0; j < B.cols; j++) {
                                printf("%d", printingMatrix[i][j]);
                            }
                            printf("\n");
                        }// ## Debug
                        printf("##################################\n\n"); // ## Debug
                        // Save on node
                        node.actor = CPU;
                        node.lastCol = B.lastcol;
                        for (int u = 0; u < COLS; u++) {
                            node.lastHeight[u] = B.height[u];
                        }
                        for (int i = 0; i < B.rows; i++) {
                            for (int j = 0; j < B.cols; j++) {
                                node.boardState[i][j] = printingMatrix[i][j];
                            }
                        }
                    }


                    // Save on sub list of ---> sub list
                    boards[curerntTurn].subBoards[x].subBoards.insert({ version,node });
                    version++;
                }
            }




            printf("## Version = %d\n", version); // ## Debug
            printf("## Number Boards = %d\n", numberBoards); // ## Debug


            /*
            Necesitamos que esto se repita hasta que no existan más trabajos 49 tablas == 0
            REcibir request y mandar a esclavo en cocncreto
            Recibir puntatuaciones y almacenarlas
            
            */
            //Start distributio of jobs
            MPI_Recv(&bufferInt, 1, MPI_INT, 0, RESPONSE, MPI_COMM_WORLD, &stat);
            MPI_Send(&bufferNode, 1, mpi_nodePack, 0, REQUEST, MPI_COMM_WORLD);

            // Wait for task
            MPI_Recv(&bufferDouble, 1, MPI_DOUBLE, 0, RESPONSE, MPI_COMM_WORLD, &stat);

            runningMaterFlag = false; // ##
        }
    }

    // Slave Function
    void slave(int myRank) {
        // Creation of Struct MPI Data type

        // Number of data items in Struct
        const int nitems = 4;
        // {actor, lastCol, lastHeight, boardState}
        int          blocklengths[nitems] = { 1, 1, COLS, (ROWS * COLS) };
        MPI_Datatype types[nitems] = { MPI_INT, MPI_INT, MPI_INT, MPI_INT };
        MPI_Datatype mpi_nodePack;

        MPI_Aint     offsets[nitems];
        offsets[0] = offsetof(NodePack, actor);
        offsets[1] = offsetof(NodePack, lastCol);
        offsets[2] = offsetof(NodePack, lastHeight);
        offsets[3] = offsetof(NodePack, boardState);
        // Commit of MPI Data Structure
        MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_nodePack);
        MPI_Type_commit(&mpi_nodePack);

        NodePack bufferNode;
        int bufferInt;
        double bufferDouble;
        MPI_Status stat;

        // Slave loop
        while (runningSlaveFlag) {
            Board SlaveBoard;
            
            // Request task
            MPI_Send(&bufferInt, 1, MPI_INT, 0, REQUEST, MPI_COMM_WORLD);

            // Wait for task
            MPI_Recv(&bufferNode, 1, mpi_nodePack, 0, RESPONSE, MPI_COMM_WORLD, &stat);
            printf("[N_%d] \n", myRank);
            //Pass from Node Matrix to SlaveBoard Field
            for (int i = 0; i < B.rows; i++) {
                for (int j = 0; j < B.cols; j++) {
                    SlaveBoard.field[i][j] = bufferNode.boardState[i][j];
                }
            }
            bufferDouble = Evaluate(SlaveBoard,bufferNode.actor,bufferNode.lastCol,DEPTH);
            // Send final calculation
            MPI_Send(&bufferDouble, 1, MPI_DOUBLE, 0, TASK_FINISH, MPI_COMM_WORLD);
            printf("[N_%d] ", myRank);

        }
    }
    /*
            MPI_Recv(&buffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat); // Recive next message
            philosopherRank = stat.MPI_SOURCE; //Read source of message-Rank of philosopher
            MPI_Send(&buffer, 1, MPI_INT, philosopherRank, RESPONSE, MPI_COMM_WORLD); // Send Fork response to the right philosopher

    */

    /*
    mpiexec -n 2 "MPI_ConnectFour.exe"

    */


    int main(int argc, char** argv)
    {
        //MPI Inizialization 
        enum role { MASTER, SLAVES };
        int myRank, comm_size;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
        MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
        

        //Inicialization of program
        //greeting(); // ##
        


        // Check numer of porcess to call
        //if (comm_size < 2) // ## Testing
        if (false)
        {
            printf("This application must be run with at least 2 MPI processes.\n");
            MPI_Abort(MPI_COMM_WORLD, -1);
        }


        switch (myRank)
        {
        case MASTER:
        {
            //master(myRank, comm_size); // ##
            break;
        }
        default:
        {
            slave(myRank);
            printf("Im in Default\n");
            break;
        }
        }

        MPI_Finalize();
        return 0;

    }
