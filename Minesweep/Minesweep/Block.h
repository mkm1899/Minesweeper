#pragma once

#include "stdafx.h"
#include <sstream>
#include <string>
#include <iostream>
#include "Matrix.h"
#include <fstream>
#include <windowsx.h>
#include <windows.h>

using namespace std;

class Block
{
public:
	Block(RECT *rect);
	Block();
	~Block();
	void setRect(RECT *rect);
	bool isBomb();
	bool isFlagged();
	bool isClicked();
	void setBombStatus(bool x);
	void setFlaggedStatus(bool x);
	bool Clicked(); //return false if it was already clicked
	void bombsNear(unsigned char x);
	void bombsNear(int r, int c, Matrix<Block> *board); //where is this block in the matrix
	char getBombsNear();
	RECT getRECT();
	void paint(HDC hdc);
	void drawFlag(HDC hdc);
	void operator=(Block temp);
	void drawBomb(HDC hdc);
	void drawLoss(HDC hdc);
	void unClick();
	void save(ofstream *write);

private:

	wstring s2ws(const string &s);
	RECT rect;
	unsigned char status; //bit 5 isBomb, bit 6 isFlagged , bit 7 isClicked, //the number of bombs near are stored in bits 0-3
};