#pragma once
// Deklaracija razreda Board

#include <assert.h>
typedef int dat;
const dat EMPTY = 0;
const dat CPU = 1;
const dat HUMAN = 2;

class Board
{
private:
	dat** field;
	int* height;
	int rows, cols;
	dat LastMover;
	int lastcol;
	void Take();	// zauzmi i popuni prazninama
	void Free();
public:
	Board() : rows(6), cols(7), LastMover(EMPTY), lastcol(-1)
	{
		Take();
	}
	Board(const int row, const int col) : rows(row), cols(col), LastMover(EMPTY), lastcol(-1)
	{
		Take();
	}
	~Board()
	{
		Free();
	}
	int Columns()	// broj stupaca
	{
		return cols;
	}
	Board(const Board& src);
	dat* operator[](const int row);
	bool MoveLegal(const int col);	// moze li potez u stupcu col
	bool Move(const int col, const dat player);	// napravi potez
	bool UndoMove(const int col);	// vrati potez iz stupca
	bool GameEnd(const int lastcol);	// je li zavrsno stanje
	bool Load(const char* fname);
	void Save(const char* fname);
};

inline dat* Board::operator[](const int row)
{
	assert(row >= 0 && row < rows);
	return field[row];
}
