#pragma once

#include "Block.h"
#include "Matrix.h"
#include "Header.h"
#include <ctime>
#include <sstream>
#include <vector>
#include <fstream>

class Board
{
public:
	Board(unsigned char row, unsigned char col, unsigned char bombs);
	~Board();
	void clicked(int x, int y, bool isLeftClicked, HWND hwnd);
	void setBoard(HWND hWnd);
	__int64 timeSpent(); //time spent in seconds
	void paintAll(HDC hdc, HWND hwnd);
	Block getBlock(int r, int c);
	bool didlose();
	void Loss(HDC hdc, HWND hwnd);
	void Win(HDC hdc, HWND hWnd);
	bool setBoard(int row, int col); //the matrix spot of the first clicked
									 // bool is to make sure clicked in the right place
	wstring s2ws(const string &s);
	int getBombsLeft();
	bool didwin();
	UINT_PTR getTimer();
	void save(ofstream *write);
	bool needRestart();


private:
	//beginning is what you want it to start at, end i where u want it to end at
	unsigned short newRandomNumber(vector<unsigned short> &unWanted, int beginning, int end);
	void setHeader(HWND hWnd, int const &startspot, const float percentTop);
	void paintHeader(HDC hdc);
	Matrix<Block> *board;
	unsigned char row, col;
	__int64 startTime;
	__int64 endTime;
	Header header;
	unsigned char bombsleft;
	bool firstClick;
	bool didLose, didWin;
	bool checkWin();
	//void restart();
	bool isFirst;
	unsigned char defaultBombs;
	void paintInSections(HBITMAP bitmap, HDC hdc, HDC buffer, HWND hwnd, int divisions);
	
	void insertionSort(vector<unsigned short> &arr);
	UINT_PTR timer;
	bool restart;
};

