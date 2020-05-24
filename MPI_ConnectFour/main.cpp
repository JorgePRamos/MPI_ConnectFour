
#include<iostream>
#include<ctime>
using namespace std;
#include"board.h"
const int DEPTH = 6;
const int ROWS = 6;
const int COLS = 7;






//CONNECT FOUR PROJECT
#include<stdio.h>
#include<string.h>
#include<string.h>
#include<math.h>
#include<windows.h>
#include<conio.h>
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


//FUNCTION TO PRINT THE BOARD
void printboard(int start, int state[ROWS][COLS]) {

    int a, c;
    //Game just started Start = 1
    if (start) {
        //Init with default symbol "*"                                             
        for (a = 0; a < ROWS; a++) {
            for (c = 0; c < COLS; c++)
                board[a][c] = '*';
        }
        
        system("cls");                          
        system("COLOR 1A");             
        //Board to Screen
        for (row = 0; row < ROWS; row++) {        
            for (col = 0; col < COLS; col++) {

                printf("| %c ", board[row][col]);
            }
            printf("|\n");
            for (a = 0; a < COLS; a++) printf("____");
            printf("\n");
        }
        for (a = 0; a < COLS; a++)printf("%*d", 4, a + 1);
        printf("\n");
    }
    else {

        system("cls");                          
        system("COLOR 1A");    
        //Board to Screen
        for (row = 0; row < ROWS; row++) {        
            for (col = 0; col < COLS; col++) {

                printf("| %c ", board[row][col]);
            }
            printf("|\n");
            for (a = 0; a < COLS; a++) printf("____");
            printf("\n");
        }
        for (a = 0; a < COLS; a++)printf("%*d", 4, a + 1);
        printf("\n");


    }

}

int main(int argc, char** argv)
{
    printf("####################################################################\n                              CONNECT FOUR        \n####################################################################\n");
    printf("\n\t-Greetings MR.Player!\n\t-Press <Y> to start.\n\t-Press <N> to close.\n\t->");
    char userInput = ' ';
    boolean incorrect = false;
    do{
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


        return 0;
    }

}




