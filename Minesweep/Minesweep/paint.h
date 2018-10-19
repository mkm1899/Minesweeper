#pragma once
//#include "Board.h"
#include "Board.h"
#include <time.h>
#include <Windows.h>

class Paint
{
public:
	Paint(int difficulty);
	~Paint();
	void paint(HDC hdc, HWND hWnd);
	Board* getBoard();
	bool setDiffSwitch(int difficulty);
	void switchDifficulty();
	void restart();

private:
	int diffSwitch; //stores which difficulty to which to when the restart button is pressed
	Board * board;
	__int64 startTime;
};
