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
#include <queue>
#include <queue>

#define REQUEST 1
#define RESPONSE 2
#define TASK_FINISH 3
const int DEPTH = 4;
const int ROWS = 6;
const int COLS = 7;
Board B;
using namespace std;
boolean runningMainFlag = true;
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
    int key;
    double score;
};

    // Boards Storage Class
    struct Node
    {
        boolean revised = false;
        double score = 0;
        int actor = 0;
        int lastCol = 0;
        int lastHeight[COLS];
        int boardState[ROWS][COLS] = {
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0}
        };
        int key;
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

        if (Current.GameEnd(iLastCol))	
            if (LastMover == CPU)
                return 1;	
            else //if(LastMover == HUMAN)
                return -1;	
    
        if (iDepth == 0)
            return 0;	
        iDepth--;
        if (LastMover == CPU)	
            NewMover = HUMAN;
        else
            NewMover = CPU;
        dTotal = 0;
        iMoves = 0;	
        for (int iCol = 0; iCol < Current.Columns(); iCol++)
        {
            if (Current.MoveLegal(iCol))	
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
                    return 1;	
                if (dResult == -1 && NewMover == HUMAN)
                    return -1;	
              
                dTotal += dResult;
            }
        }
        if (bAllWin == true)	
            return 1;
        if (bAllLose == true)
            return -1;
        dTotal /= iMoves;	
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
        greeting();
        // Creation of Struct MPI Data type

       // Number of data items in Struct
        const int nitems = 6;
        // {actor, lastCol, lastHeight, boardStatem, key, score}
        int          blocklengths[nitems] = { 1, 1, COLS, (ROWS * COLS), 1 , 1};
        MPI_Datatype types[nitems] = { MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_FLOAT};
        MPI_Datatype mpi_nodePack;

        MPI_Aint     offsets[nitems];
        offsets[0] = offsetof(NodePack, actor);
        offsets[1] = offsetof(NodePack, lastCol);
        offsets[2] = offsetof(NodePack, lastHeight);
        offsets[3] = offsetof(NodePack, boardState);
        offsets[4] = offsetof(NodePack, key);
        offsets[4] = offsetof(NodePack, score);
        // Commit of MPI Data Structure
        MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_nodePack);
        MPI_Type_commit(&mpi_nodePack);

        Node node;
        NodePack bufferNode;
        int bufferInt;
        double bufferDouble;
        MPI_Status stat;
        MPI_Request request;
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
        while (runningMainFlag) {
            int numberBoards = 0;
            curerntTurn++;

            // Players turn
            int playerMove = playerInput();
            B.Move(playerMove, HUMAN);

            //Print Updated Board
            boarTranslator(B.field);
            printboard(start, printingMatrix);

            // Save current Turn board 
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
            node.key = curerntTurn;
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


                // Check if move is legal and perform it if so
                if (B.MoveLegal(iCol))
                {
                    B.Move(iCol, CPU);
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
                    node.key = version;
                    // Save on list
                    boards[curerntTurn].subBoards.insert({ version,node });
                    version++;
                }
            }


            // Simulate 7 moves for each of the 7 simulated CPU moves
            for (int x = 1; x <= boards[curerntTurn].subBoards.size(); x++) { 
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
                        // Print the new move
;
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
                        node.key = version;
                    }


                    // Save on sub list of ---> sub list
                    boards[curerntTurn].subBoards[x].subBoards.insert({ version,node });
                    version++;
                }
            }





            int senderRank = 0;
            // For each of the generated 7 boards from the first turn
            for (int x = 0; x < boards[curerntTurn].subBoards.size(); x++) {
                    
                // For each of the 7 generated sub-boards from each of the 7 suboards
                for (int j = 0; j < boards[curerntTurn].subBoards[x].subBoards.size(); j++) {
                    // Start distributio of jobs
                    // BufferInt == rank

                    // Packcage node for sending
                    NodePack nodePack;
                    nodePack.actor = boards[curerntTurn].subBoards[x].subBoards[j].actor;
                    for (int i = 0; i < B.rows; i++) {
                        for (int j = 0; j < B.cols; j++) {
                            nodePack.boardState[i][j] = boards[curerntTurn].subBoards[x].subBoards[j].boardState[i][j];
                        }
                    }
                    nodePack.key = boards[curerntTurn].subBoards[x].subBoards[j].key;
                    nodePack.lastCol = boards[curerntTurn].subBoards[x].subBoards[j].lastCol;

                    for (int u = 0; u < COLS; u++) {
                        nodePack.lastHeight[u] = boards[curerntTurn].subBoards[x].subBoards[j].lastHeight[u];
                    }


                    // Communication 
                    if (MPI_Irecv(&bufferInt, 1, MPI_INT, MPI_ANY_SOURCE, REQUEST, MPI_COMM_WORLD, &request) == MPI_SUCCESS) {
                        MPI_Wait(&request, &stat);
                        printf("[N_%d] Recieve request from [ %d ]\n", myRank, stat.MPI_SOURCE);

                        bufferNode = nodePack;
                        MPI_Send(&bufferNode, 1, mpi_nodePack, stat.MPI_SOURCE, RESPONSE, MPI_COMM_WORLD);
                        printf("[N_%d] Sending Board to [ %d ]\n", myRank, stat.MPI_SOURCE);
                    }

                    if (MPI_Irecv(&bufferNode, 1, mpi_nodePack, MPI_ANY_SOURCE, TASK_FINISH, MPI_COMM_WORLD, &request) == MPI_SUCCESS){
                        MPI_Wait(&request, &stat);
                        printf("[N_%d] Revieve final score for [ %d ]\n", myRank, bufferNode.key);
                        boards[curerntTurn].subBoards[x].subBoards[bufferNode.key].score = bufferNode.score;

                     }
                }
                // Get the max score for each of the 7 sub-boards
                double maxScore = boards[curerntTurn].subBoards[x].subBoards[1].score;
                for (int j = 0; j < boards[curerntTurn].subBoards[x].subBoards.size(); j++) {
                    if (boards[curerntTurn].subBoards[x].subBoards[j].score > maxScore) {
                        maxScore = boards[curerntTurn].subBoards[x].subBoards[j].score;
                    }
                }
            }
            // Get the max score for each of the 7 boards
            int maxScoreIndex = 1;
            double maxScore = boards[curerntTurn].subBoards[1].score;
            for (int j = 0; j < boards[curerntTurn].subBoards.size(); j++) {
                if (boards[curerntTurn].subBoards[j].score > maxScore) {
                    maxScore = boards[curerntTurn].subBoards[j].score;
                    maxScoreIndex = j;
                }
            }
            // Set the max score for the cpu move in the playing turn
            boards[curerntTurn].score = maxScore;

            // Update Boards list Max Score
            for (int i = 0; i < B.rows; i++) {
                for (int j = 0; j < B.cols; j++) {
                    boards[curerntTurn].boardState[i][j] = boards[curerntTurn].subBoards[maxScoreIndex].boardState[i][j];
                }
            }
            // Update playing board
            for (int i = 0; i < B.rows; i++) {
                for (int j = 0; j < B.cols; j++) {
                    B.field[i][j] = boards[curerntTurn].subBoards[maxScoreIndex].boardState[i][j];
                }
            }
            //B.LastMover = boards[curerntTurn].subBoards[maxScoreIndex].actor;
            B.LastMover = CPU;
            for (int u = 0; u < COLS; u++) {
                B.height[u] = boards[curerntTurn].subBoards[maxScoreIndex].lastHeight[u];
            }
            B.lastcol = boards[curerntTurn].subBoards[maxScoreIndex].lastCol;

            //Print Updated Board
            boarTranslator(B.field);
            printboard(0, printingMatrix);

            // Check if game is finish
            if (B.GameEnd(boards[curerntTurn].lastCol)) {
                printf("[N_%d] PARTIDA TERMINADA\n", myRank);
                runningSlaveFlag = false;
                runningMainFlag = false;
            }
        }
    }

    // Slave Function
    void slave(int myRank) {
        // Creation of Struct MPI Data type

        // Number of data items in Struct
        const int nitems = 6;
        // {actor, lastCol, lastHeight, boardStatem, key, score}
        int          blocklengths[nitems] = { 1, 1, COLS, (ROWS * COLS), 1 , 1 };
        MPI_Datatype types[nitems] = { MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_FLOAT };
        MPI_Datatype mpi_nodePack;

        MPI_Aint     offsets[nitems];
        offsets[0] = offsetof(NodePack, actor);
        offsets[1] = offsetof(NodePack, lastCol);
        offsets[2] = offsetof(NodePack, lastHeight);
        offsets[3] = offsetof(NodePack, boardState);
        offsets[4] = offsetof(NodePack, key);
        offsets[4] = offsetof(NodePack, score);
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
            bufferInt = myRank;
            MPI_Send(&bufferInt, 1, MPI_INT, 0, REQUEST, MPI_COMM_WORLD);
            printf("[N_%d] Requesting Board\n", myRank);
            // Wait for task
            MPI_Recv(&bufferNode, 1, mpi_nodePack, 0, RESPONSE, MPI_COMM_WORLD, &stat);
            printf("[N_%d] Task recieved\n", myRank);

            // Pass from Node Matrix to SlaveBoard Field
            for (int i = 0; i < B.rows; i++) {
                for (int j = 0; j < B.cols; j++) {
                    SlaveBoard.field[i][j] = bufferNode.boardState[i][j];
                }
            }
            // Calcualte Score
            bufferNode.score = Evaluate(SlaveBoard,bufferNode.actor,bufferNode.lastCol,DEPTH);

            // Send final calculation
            MPI_Send(&bufferNode, 1, mpi_nodePack, 0, TASK_FINISH, MPI_COMM_WORLD);
            printf("[N_%d] Sending final result\n", myRank);

        }
    }



    int main(int argc, char** argv)
    {
        //MPI Inizialization 
        enum role { MASTER, SLAVES };
        int myRank, comm_size;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
        MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

        // Check numer of porcess to call
       if (comm_size < 2){

            printf("\n#### This application must be run with at least 2 MPI processes. ####\n\n");
            MPI_Abort(MPI_COMM_WORLD, -1);
        }
        printf("HEllo");

        switch (myRank)
        {
        case MASTER:
        {
            master(myRank, comm_size);
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

    /*
    mpiexec -n 2 "MPI_ConnectFour.exe"
    */