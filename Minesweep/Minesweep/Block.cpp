#include "stdafx.h"
#include "Block.h"

Block::Block(RECT *rect) {
	this->rect.bottom = rect->bottom;
	this->rect.left = rect->left;
	this->rect.right = rect->right;
	this->rect.top = rect->top;
	status = 0;
}

Block::Block() {
	status = 0;
}

Block::~Block()
{
}

void Block::setRect(RECT *rect) {
	this->rect.bottom = rect->bottom;
	this->rect.left = rect->left;
	this->rect.right = rect->right;
	this->rect.top = rect->top;
}

bool Block::isBomb() {
	return (status >> 5) & 1;
}

bool Block::isFlagged()
{
	return (status >> 6) & 1;
}

bool Block::isClicked() {
	return (status >> 7) & 1;
}

void Block::setBombStatus(bool x) {
	if (x == false) {
		status &= ~(1 << 5);
	}
	else {
		status |= (1) << 5;
	}
}

void Block::setFlaggedStatus(bool x) {
	if (x == false) {
		//status = 0 << 6;
		status &= ~(1 << 6);
	}
	else {
		status |= (1) << 6;
	}
}

bool Block::Clicked()
{
	if (isClicked()) {
		return false;
	}
	else {
		status |= (1) << 7;
		return true;
	}
}

void Block::bombsNear(unsigned char x)
{
	for (int i = 0; i < 4; i++) {
		status ^= ((-((x >> i) & 1)) ^ status) & (1 << i);
	}
}

void Block::bombsNear(int r, int c, Matrix<Block> *board)
{
	unsigned char count = 0;
	for (int j = -1; j <= 1; j++) {
		for (int i = -1; i <= 1; i++) {
			if (r + j < 0 || c + i < 0 || r + j >= board->getRowSize() || c + i >= board->getColSize()) {
			}
			else if (board->get(r + j, c + i)->isBomb()) {
				count++;
			}
		}
	}
	this->bombsNear(count);
	if (count == 0) {
		if (r - 1 >= 0 && r - 1 < board->getRowSize()) {
			if (c - 1 >= 0 && c - 1 < board->getColSize()) {
				if (board->get(r - 1, c - 1)->Clicked()) {
					board->get(r - 1, c - 1)->bombsNear(r - 1, c - 1, board);
				}
			}
			if (c >= 0 && c < board->getColSize()) {
				if (board->get(r - 1, c)->Clicked()) {
					board->get(r - 1, c)->bombsNear(r - 1, c, board);
				}
			}
			if (c + 1 >= 0 && c + 1 < board->getColSize()) {
				if (board->get(r - 1, c + 1)->Clicked()) {
					board->get(r - 1, c + 1)->bombsNear(r - 1, c + 1, board);
				}
			}
		}
		if (r >= 0 && r < board->getRowSize()) {
			if (c - 1 >= 0 && c - 1 < board->getColSize()) {
				if (board->get(r, c - 1)->Clicked()) {
					board->get(r, c - 1)->bombsNear(r, c - 1, board);
				}
			}
			if (c + 1 >= 0 && c + 1 < board->getColSize()) {
				if (board->get(r, c + 1)->Clicked()) {
					board->get(r, c + 1)->bombsNear(r, c + 1, board);
				}
			}
		}
		if (r + 1 >= 0 && r + 1 < board->getRowSize()) {
			if (c - 1 >= 0 && c - 1 < board->getColSize()) {
				if (board->get(r + 1, c - 1)->Clicked()) {
					board->get(r + 1, c - 1)->bombsNear(r + 1, c - 1, board);
				}
			}
			if (c >= 0 && c < board->getColSize()) {
				if (board->get(r + 1, c)->Clicked()) {
					board->get(r + 1, c)->bombsNear(r + 1, c, board);
				}
			}
			if (c + 1 >= 0 && c + 1 < board->getColSize()) {
				if (board->get(r + 1, c + 1)->Clicked()) {
					board->get(r + 1, c + 1)->bombsNear(r + 1, c + 1, board);
				}
			}
		}
	}
}


char Block::getBombsNear()
{
	char x = 0;
	for (int i = 0; i < 4; i++) {
		x |= ((status >> i) & 1) << i;
	}
	return x;
}

RECT Block::getRECT()
{
	return rect;
}

wstring Block::s2ws(const string &s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

void Block::paint(HDC hdc)
{
	char x = getBombsNear();
	if (isClicked()) {
		if (isBomb()) {
			drawBomb(hdc);
			//string temp = "B";
			//wstring wtemp;
			//LPCWSTR ltemp;
			//wtemp = s2ws(temp);
			//ltemp = wtemp.c_str();
			//DrawText(hdc, ltemp, temp.size(), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
		else if (x != 0) {
			int y = x;
			stringstream ss;
			ss << y;
			string temp = ss.str();
			wstring wtemp;
			LPCWSTR ltemp;
			wtemp = s2ws(temp);
			ltemp = wtemp.c_str();
			DrawText(hdc, ltemp, temp.size(), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
		HBRUSH black = CreateSolidBrush(RGB(0, 0, 0));
		FrameRect(hdc, &rect, black);
		DeleteObject(black);
	}
	else {
		HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
		HBRUSH darkGray = CreateSolidBrush(RGB(123, 123, 123));
		HBRUSH gray = CreateSolidBrush(RGB(189, 189, 189));
		HBRUSH black = CreateSolidBrush(RGB(0, 0, 0));
		

		//the width of the box is rect.right - rect.left
		//the ratio of the width of the lines is 3/20
		const int widthOfLine = static_cast<int>((rect.right - rect.left) * (3.0f / 20.0f));

		
		//white lines pts
		POINT pts[6];
		pts[0].x = rect.left; pts[0].y = rect.bottom;
		pts[1].x = rect.left; pts[1].y = rect.top;
		pts[2].x = rect.right; pts[2].y = rect.top;
		pts[3].x = rect.right - widthOfLine; pts[3].y = rect.top + widthOfLine;
		pts[4].x = rect.left + widthOfLine; pts[4].y = rect.top + widthOfLine;
		pts[5].x = rect.left + widthOfLine; pts[5].y = rect.bottom - widthOfLine;
		
		FillRect(hdc, &rect, gray);
		FrameRect(hdc, &rect, black);
		
		//draw white lines
		SelectBrush(hdc, white);
		Polygon(hdc, pts, 6);

		//darkgray lines pts
		pts[0].x = rect.left; pts[0].y = rect.bottom;
		pts[1].x = rect.left + widthOfLine; pts[1].y = rect.bottom - widthOfLine;
		pts[2].x = rect.right - widthOfLine; pts[2].y = rect.bottom - widthOfLine;
		pts[3].x = rect.right - widthOfLine; pts[3].y = rect.top + widthOfLine;
		pts[4].x = rect.right; pts[4].y = rect.top;
		pts[5].x = rect.right; pts[5].y = rect.bottom;

		//draw gray lines
		SelectBrush(hdc, darkGray);
		Polygon(hdc, pts, 6);

		if (isFlagged()) {
			drawFlag(hdc);
		}
		DeleteObject(gray);
		DeleteObject(black);
		DeleteObject(white);
		DeleteObject(darkGray);
	}
}

void Block::drawFlag(HDC hdc)
{
	int width = rect.right - rect.left;
	int length = rect.bottom - rect.top;
	POINT points[3];
	//right
	points[0].x = (rect.left + (width *.8));
	points[0].y = (rect.top + (length * .3));
	//top left
	points[1].x = (rect.left + width * .5);
	points[1].y = (rect.top + length * .2);
	//bottom left
	points[2].x = (rect.left + width * .5);
	points[2].y = (rect.top + length * .4);
	SetDCBrushColor(hdc, RGB(255, 0, 0));
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	Polygon(hdc, points, 3);

	//line to base
	RECT temp;
	temp.top = rect.top + length * .2;
	temp.left = rect.left + width * .47;
	temp.right = rect.left + width * .5;
	temp.bottom = rect.top + length * .8;
	HBRUSH black = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(hdc, &temp, black);

	//base
	temp.top = rect.top + length * .8;
	temp.left = rect.left + width * .3;
	temp.right = rect.left + width * .7;
	temp.bottom = rect.top + length * .85;
	FillRect(hdc, &temp, black);
	DeleteObject(black);
}

void Block::operator=(Block temp)
{
	setRect(&temp.getRECT());
	setBombStatus(temp.isBomb());
	setFlaggedStatus(temp.isFlagged());
	if (temp.isClicked()) {
		Clicked();
	}
	else {
		unClick();
	}
	bombsNear(temp.getBombsNear());

}

void Block::drawBomb(HDC hdc)
{
	int width = rect.right - rect.left;
	int length = rect.bottom - rect.top;


	if (isFlagged()) {
		return;
	}


	if (!isClicked()) {
		SetDCBrushColor(hdc, RGB(0, 0, 0));
		SelectObject(hdc, GetStockObject(DC_BRUSH));
	}
	//the else statement only happens if the block was the bomb that was clicked on that created the loss
	else {
		SetDCBrushColor(hdc, RGB(255, 0, 0));
		SelectObject(hdc, GetStockObject(DC_BRUSH));
	}
	//circle
	const float BOUND = 1.0 / 4.0;
	int right, left, bottom, top;
	top = static_cast<int>(length*(BOUND)) + rect.top;
	bottom = static_cast<int>(rect.bottom - length * (BOUND));
	left = static_cast<int>(rect.left + width * (BOUND));
	right = static_cast<int>(rect.right - width * (BOUND));
	Ellipse(hdc, left, top, right, bottom);

	//spikes
	//RECT Rtemp;
	//Rtemp.bottom = top;
	//Rtemp.top = top + length*(1 / 5);
	//FillRect()

}

void Block::drawLoss(HDC hdc)
{
	if (isFlagged() && !isBomb()) {
		drawBomb(hdc);
		paint(hdc);
		drawFlag(hdc);

		POINT points[4];
		int width = rect.right - rect.left;
		int length = rect.bottom - rect.top;
		//top left 
		points[0].x = (rect.left); //(width * .0));
		points[0].y = (rect.top + (length * .1));
		//top left
		points[1].x = (rect.left + width * .1);
		points[1].y = (rect.top);
		//bottom right
		points[3].x = (rect.right - (width*.1));
		points[3].y = (rect.bottom);
		//bottom right
		points[2].x = (rect.right);
		points[2].y = (rect.bottom - length * .1);
		SetDCBrushColor(hdc, RGB(255, 0, 0));
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		Polygon(hdc, points, 4);

		//top right
		points[0].x = (rect.right); //(width * .0));
		points[0].y = (rect.top + (length * .1));
		//top right
		points[1].x = (rect.right - (width*.1));
		points[1].y = (rect.top);
		//bottom left
		points[2].x = (rect.left);
		points[2].y = (rect.bottom - length * .1);
		//bottom left
		points[3].x = (rect.left + width * .1);
		points[3].y = (rect.bottom);
		SetDCBrushColor(hdc, RGB(255, 0, 0));
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		Polygon(hdc, points, 4);
		
	}
	else if (isBomb() && isClicked()) {
		HBRUSH red = CreateSolidBrush(RGB(255, 0, 0));
		FillRect(hdc, &rect, red);
		drawBomb(hdc);
		DeleteObject(red);
	}
	else if (isBomb() && isFlagged()) {
		paint(hdc);
		drawFlag(hdc);
	}
	else if (isBomb()) {
		HBRUSH red = CreateSolidBrush(RGB(0, 0, 0));
		FrameRect(hdc, &rect, red);
		drawBomb(hdc);
		DeleteObject(red);
	}
	else {
		paint(hdc);
	}
}

void Block::unClick()
{
	status &= ~(1 << 7);
}

void Block::save(ofstream *write)
{
	if (write->is_open()) {
		write->write(reinterpret_cast<char *>(status), sizeof(status));
		write->write(reinterpret_cast<char *>(rect.bottom), sizeof(rect.bottom));
		write->write(reinterpret_cast<char *>(rect.left), sizeof(rect.left));
		write->write(reinterpret_cast<char *>(rect.right), sizeof(rect.right));
		write->write(reinterpret_cast<char *>(rect.top), sizeof(rect.top));
	}
}

//www-user.tu-chemnitz.de/~heha/petzold/ch05e.htm
//https://msdn.microsoft.com/en-us/library/windows/desktop/dd145096(v=vs.85).aspx