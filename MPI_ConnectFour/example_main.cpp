
#include<iostream>
#include<ctime>
using namespace std;
#include"board.h"	

const int DEPTH = 4;	//Default depth

double Evaluate(Board Current, dat LastMover, int iLastCol, int iDepth);
/*
int main(int argc, char** argv)
{
    Board B;
    double dResult, dBest;
    int iBestCol, iDepth = DEPTH;
    if (argc < 2)//No file
    {
        cout << "Usage: <program> <current state file> [<deep>]" << endl;
        return 0;
    }
    B.Load("C:\\Users\\Jorge\\Downloads\\ploca.txt");//File Load
    if (argc > 2)
        iDepth = atoi(argv[2]);//Geting number of threads
    srand((unsigned)time(NULL));
    // check if the game is over (eg if the player has won)
    for (int iCol = 0; iCol < B.Columns(); iCol++)
        if (B.GameEnd(iCol))
        {
            cout << "Igra zavrsena!" << endl;
            return 0;
        }
    // assumption: the CPU is on the move
    do
    {
        cout << "Dubina: " << iDepth << endl;
        dBest = -1; iBestCol = -1;
        for (int iCol = 0; iCol < B.Columns(); iCol++)
        {
            if (B.MoveLegal(iCol))
            {
                if (iBestCol == -1)
                    iBestCol = iCol;
                B.Move(iCol, CPU);
                dResult = Evaluate(B, CPU, iCol, iDepth - 1);
                B.UndoMove(iCol);
                if (dResult > dBest || (dResult == dBest && rand() % 2 == 0))
                {
                    dBest = dResult;
                    iBestCol = iCol;
                }
                cout << "Stupac " << iCol << ", vrijednost: " << dResult << endl;
            }
        }
        iDepth /= 2;
        // why loop? if all moves lead to defeat, we count once again for twice the depth
        // because the player may not be aware of the possibility of winning
    } while (dBest == -1 && iDepth > 0);
    cout << "Najbolji: " << iBestCol << ", vrijednost: " << dBest << endl;
    B.Move(iBestCol, CPU);
    // did we win

    for (int iCol = 0; iCol < B.Columns(); iCol++)
        if (B.GameEnd(iCol))
        {
            cout << "Igra zavrsena! (pobjeda racunala)" << endl;
            return 0;
        }
    return 0;
}



// recursive function: examines all possible moves and returns an estimate of the obtained state of the board
// Current: current state of the board
// LastMover: HUMAN or CPU
// iLastCol: previous move column
// iDepth: depth is reduced to 0

double Evaluate(Board Current, dat LastMover, int iLastCol, int iDepth)
{
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





/*


MPI_Init(&argc, &argv);

    int comm_size;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    if(comm_size != 2)
    {
        printf("This application must be run with 2 MPI processes.\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

*/



/*



int main(int argc, char** argv) {
    enum role { MONITOR, PHILOSOPHER};
    int myRank, comm_size;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("********** Error Creating Sig_Handler **********\n");

    switch (myRank)
    {
    case MONITOR:
    {
        monitor(myRank, comm_size);
        break;
    }
    default:
    {
        philosopher(myRank);
        break;
    }
    }

    MPI_Finalize();
    return 0;

}
*/