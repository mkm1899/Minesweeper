#include "stdafx.h"
#include "paint.h"

Paint::Paint(int difficulty)
{
	difficulty;
	switch (difficulty) {
		case 1:
			//beginner
			board = new Board(9, 9, 10);
			break;
		case 2:
			//intermidiate
			board = new Board(16, 16, 40);
			break;
		case 3:
			//expert
			board = new Board(16, 30, 99);
			break;
		default:
			board = new Board(16, 30, 99);
			break;
	}
	//board = new Board(16, 30, 99);
	//board = new Board(9, 9, 10);
	startTime = time(0);
}

Paint::~Paint()
{
	delete board;
}

void Paint::paint(HDC hdc, HWND hWnd)
{
	//board->setBoard(hWnd);
	if (board->needRestart()) {
		switchDifficulty();
		board->setBoard(hWnd);
		board->paintAll(hdc, hWnd);
	}
	else if (board->didlose()) {
		board->setBoard(hWnd);
		board->Loss(hdc, hWnd);
	}
	else if (board->didwin()) {
		board->setBoard(hWnd);
		board->Win(hdc, hWnd);

		stringstream ss;
		ss << board->timeSpent();
		string temp = "\tYou WON in " + ss.str() + " seconds\t";
		MessageBoxA(hWnd, temp.c_str(), "You won", MB_OK);
		switchDifficulty();
		RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INTERNALPAINT | RDW_INVALIDATE);
	}
	else {
		board->setBoard(hWnd);
		board->paintAll(hdc, hWnd);
	}
}

Board* Paint::getBoard()
{
	return board;
}

bool Paint::setDiffSwitch(int difficulty)
{
	if (difficulty > 3 || difficulty < 1) {
		return false;
	}
	diffSwitch = difficulty;
	return true;
}

void Paint::switchDifficulty()
{
	delete board;
	switch (diffSwitch) {
	case 1:
		//beginner
		board = new Board(9, 9, 10);
		break;
	case 2:
		//intermidiate
		board = new Board(16, 16, 40);
		break;
	case 3:
		//expert
		board = new Board(16, 30, 99);
		break;
	default:
		board = new Board(9, 9, 10);
		break;
	}
}


