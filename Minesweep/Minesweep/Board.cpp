#include "stdafx.h"
#include "Board.h"

Board::Board(unsigned char row, unsigned char col, unsigned char bombs)
{
	restart = false;
	this->row = row;
	this->col = col;
	board = new Matrix<Block>(row, col);
	bombsleft = bombs;
	defaultBombs = bombs;
	firstClick = true;
	didLose = false;
	didWin = false;
	endTime = 0;
	isFirst = true;
	//startTime = time(0);
}

Board::~Board()
{
	delete board;
}

void Board::clicked(int x, int y, bool isLeftClicked, HWND hwnd)
{
	//make sure to check if it is left or right click

	//checks if restart button is clicked
	if (isLeftClicked) {
		if (y <= header.Restart.bottom && y >= header.Restart.top) {
			if (x >= header.Restart.left && x <= header.Restart.right) {
				restart = true;
				RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INTERNALPAINT | RDW_INVALIDATE);
				return;
			}
		}
	}

	if (checkWin() || didLose) {
		return;
	}

	RECT start = board->get(0, 0)->getRECT();
	if (x < start.left || y < start.top) {
		return;
	}

	int blockWidth = start.right - start.left;
	int blockHeight = start.bottom - start.top;
	//x col
	x -= start.left;
	x /= blockWidth;

	//y row
	y -= start.top;
	y /= blockHeight;

	if (x < 0 || y < 0 || x >= (board->getColSize()) || y >= (board->getRowSize())) {
		return;
	}

	if (isLeftClicked && firstClick) {
		firstClick = false;
		//setup everything
		//bool test = setBoard(y, x);
		//if (!test) {
		//	firstClick = true;
		//	return;
		//}
		//board->get(y, x)->Clicked();
		int count = 0;
		Block temp;

		//do {
		bool test = setBoard(y, x);
		if (!test) {
			firstClick = true;
			return;
		}
		count = 0;
		board->get(y, x)->Clicked();
		board->get(y, x)->bombsNear(y, x, board);

		//} while (count > 50);
		startTime = time(0);
		SetTimer(hwnd, timer, 1000, (TIMERPROC)NULL);
		RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INTERNALPAINT | RDW_INVALIDATE);
		//make sure to check that there are bombs near by and if he loses
	}
	else if (isLeftClicked && !board->get(y, x)->isFlagged()) {
		board->get(y, x)->Clicked();
		board->get(y, x)->bombsNear(y, x, board);
		checkWin();
		if (board->get(y, x)->isBomb()) {
			didLose = true;
			RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INTERNALPAINT | RDW_INVALIDATE);
			return;
		}
		RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INTERNALPAINT | RDW_INVALIDATE);
	}
	else if (!firstClick && board->get(y, x)->isClicked() == false && !isLeftClicked) {
		if (!board->get(y, x)->isClicked()) {
			if (board->get(y, x)->isFlagged()) {
				board->get(y, x)->setFlaggedStatus(false);
				bombsleft++;
				RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INTERNALPAINT | RDW_INVALIDATE);
			}
			else {
				board->get(y, x)->setFlaggedStatus(true);
				bombsleft--;
				RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INTERNALPAINT | RDW_INVALIDATE);
			}
		}
	}
}



void Board::setBoard(HWND hWnd)
{
	//static bool isFirst = true;
	Block temp;
	RECT Rtemp, dim;
	GetClientRect(hWnd, &dim);
	int length = dim.bottom;
	int width = dim.right;
	const float percentTop = .1; //the space at the top

	int sideSize;
	bool LExtraSpace;
	if (((length*(1 - percentTop)) / row) < (width / col)) {
		sideSize = static_cast<int>(((length*(1 - percentTop)) / row));
		LExtraSpace = false;
	}
	else {
		sideSize = static_cast<int>((width / col));
		LExtraSpace = true;
	}

	int t, b, l, r;
	int startSpot = 0;
	for (int j = 0; j < this->row; j++) {
		for (int i = 0; i < this->col; i++) {
			if (LExtraSpace == false) {
				startSpot = (width - (sideSize*col)) / 2;
			}
			t = ((static_cast<int>((length*percentTop))) + (j*sideSize));
			b = ((static_cast<int>((length*percentTop))) + ((j + 1)*sideSize));
			l = (startSpot + (i*(sideSize)));
			r = (startSpot + ((i + 1)*sideSize));
			Rtemp.top = t;
			Rtemp.bottom = b;
			Rtemp.left = l;
			Rtemp.right = r;
			if (isFirst) {
				temp = Block(&Rtemp);
				board->set(j, i, temp);
			}
			else {
				board->get(j, i)->setRect(&Rtemp);
				//temp.setFlaggedStatus(true);
				//board->set(j, i, temp);
			}
		}
	}
	setHeader(hWnd, startSpot, percentTop);
	isFirst = false;
}

__int64 Board::timeSpent()
{
	if (firstClick) {
		return 0;
	}
	else if (didlose()) {
		return endTime - startTime;
	}
	else if (didWin) {
		return endTime - startTime;
	}
	return time(0) - startTime;
}

void Board::paintAll(HDC hdc, HWND hwnd)
{
	HDC buffer = CreateCompatibleDC(hdc);
	RECT rect;
	GetClientRect(hwnd, &rect);
	HBITMAP bitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
	SelectObject(buffer, bitmap);
	HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(buffer, &rect, white);
	DeleteObject(white);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//board->get(i, j)->paint(hdc);
			if (didWin) {
				board->get(i, j)->setFlaggedStatus(true);
			}
			board->get(i, j)->paint(buffer);
		}
	}
	paintHeader(buffer);
	//paintInSections(bitmap, hdc, buffer, hwnd, 20);
	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, buffer, 0, 0, SRCCOPY);
	DeleteObject(bitmap);
	DeleteObject(buffer);
	//paintHeader(hdc);
}

Block Board::getBlock(int r, int c)
{
	return *(board->get(r, c));
}

bool Board::didlose()
{
	return didLose;
}

bool Board::setBoard(int row, int col)
{
	int r, c;
	srand(time(0));
	//vector<short> unWanted;
	//srand(time(0));
	unsigned short random;
	vector<unsigned short> unWanted;

	if (row < 0 || col < 0 || row > this->row || col >this->col) {
		return false;
	}

	unWanted.push_back((((row - 1)*this->col) + col - 1));
	unWanted.push_back((((row - 1)*this->col) + col));
	unWanted.push_back((((row - 1)*this->col) + col + 1));
	unWanted.push_back((((row)*this->col) + col - 1));
	unWanted.push_back((((row)*this->col) + col));
	unWanted.push_back((((row)*this->col) + col + 1));
	unWanted.push_back((((row + 1)*this->col) + col - 1));
	unWanted.push_back((((row + 1)*this->col) + col));
	unWanted.push_back((((row + 1)*this->col) + col + 1));

	for (int i = 0; i < bombsleft; i++) {
		insertionSort(unWanted);
		random = newRandomNumber(unWanted, 0, this->row*this->col);
		r = random / this->col;
		c = random % this->col;
		unWanted.push_back(random);
		board->get(r, c)->setBombStatus(true);
	}
	return true;
}

int Board::getBombsLeft()
{
	return bombsleft;
}

bool Board::didwin() {
	if (didWin) {
		return true;
	}
	return false;
}

UINT_PTR Board::getTimer()
{
	return timer;
}

//did not impliment
void Board::save(ofstream *write)
{
	if (write->is_open()) {
		write->write(reinterpret_cast<char *>(row), sizeof(row));
		write->write(reinterpret_cast<char *>(col), sizeof(col));
		write->write(reinterpret_cast<char *>(timeSpent()), sizeof(__int64));
		write->write(reinterpret_cast<char *>(defaultBombs), sizeof(defaultBombs));
		for (int i = 0; i < col*row; i++) {

		}
	}
}

bool Board::needRestart()
{
	return restart;
}

void Board::insertionSort(vector<unsigned short> &arr) {
	int j, temp;

	for (int i = 0; i < arr.size(); i++) {
		j = i;

		while (j > 0 && arr[j] < arr[j - 1]) {
			temp = arr[j];
			arr[j] = arr[j - 1];
			arr[j - 1] = temp;
			j--;
		}
	}
}
//void Board::leftClick(int r, int c)
//{
//	if (board->get(r,c)->isBomb()) {
//		//trigger loss
//	}
//	board->get(r, c)->Clicked();
//	calculateBombsNear(r, c);
//	int test;
//	test = board->get(r, c)->getBombsNear();
//	if (board->get(r, c)->getBombsNear() == 0) {
//		for (int j = -1; j <= 1; j++) {
//			for (int i = -1; i <= 1; i++) {
//				if (r + j >= 0 && c + i >= 0 && r + j <= board->getRowSize() && c + i <= board->getColSize()) {
//					if (!board->get(r + j, c + i)->Clicked()) {
//						if (calculateBombsNear(r + j, c + i) == 0) {
//							leftClick(r + j, c + i);
//						}
//					}
//				}
//			}
//		}
//	}
//}
//
//unsigned char Board::calculateBombsNear(int r, int c)
//{
//	unsigned char count = 0;
//	for (int j = -1; j <= 1; j++) {
//		for (int i = -1; i <= 1; i++) {
//			if (r + j < 0 || c + i < 0 || r + j > board->getRowSize() || c + i > board->getColSize()) {
//			}
//			else if (board->get(r+i, c+i)->isBomb()) {
//				count++;
//			}
//		}
//	}
//	board->get(r, c)->bombsNear(count);
//	return count;
//}

unsigned short Board::newRandomNumber(vector<unsigned short> &unWanted, int beginning, int end)
{
	//srand(time(0));
	unsigned short random = rand() % ((end - beginning) - unWanted.size());
	for (int i = 0; i < unWanted.size(); i++) {
		if (random >= unWanted[i]) {
			random++;
		}
	}
	random = random % end;
	return random;
}

void Board::setHeader(HWND hWnd, int const &startspot, const float percentTop)
{
	RECT screen;
	GetClientRect(hWnd, &screen);
	RECT temp;
	int const endspot = screen.right-startspot; //this is where the blocks end 
	int const third = ((endspot - startspot) * (1.0f / 3.0f)); //third of the space between startspot and endspot


	temp.left = startspot;
	temp.top = 0;
	temp.bottom = static_cast<int>(screen.bottom*percentTop);
	temp.right = startspot + static_cast<int>(screen.right * percentTop);
	if (temp.right > startspot + third) {
		temp.right = startspot + third;
	}
	header.Bombsleft = temp;
	
	temp.right = screen.right - startspot;
	temp.left = (screen.right - startspot) - static_cast<int>(screen.right*percentTop);
	temp.top = 0;
	temp.bottom = static_cast<int>(screen.bottom*percentTop);
	if (temp.left < endspot - third) {
		temp.left = endspot - third;
	}
	header.Time = temp;
	
	temp.left = screen.right*.45;
	temp.right = screen.right*.55;
	temp.top = 0;
	temp.bottom = screen.bottom*percentTop;
	if ((temp.right - temp.left) > third) {
		temp.right = screen.right*.5 + third / 2;
		temp.left = screen.right*.5 - third / 2;
	}
	header.Restart = temp;
}

void Board::paintHeader(HDC hdc)
{
	HBRUSH black = CreateSolidBrush(RGB(0, 0, 0));
	FrameRect(hdc, &header.Bombsleft, black);
	__int64 x = bombsleft;
	stringstream ss;
	ss << x;
	string temp = ss.str();
	wstring wtemp = s2ws(temp);
	LPCWSTR ltemp = wtemp.c_str();
	DrawText(hdc, ltemp, temp.size(), &header.Bombsleft, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	FrameRect(hdc, &header.Time, black);
	x = timeSpent();
	ss.str("");
	ss << x;
	temp = ss.str();
	wtemp = s2ws(temp);
	ltemp = wtemp.c_str();
	DrawText(hdc, ltemp, temp.size(), &header.Time, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	SetTextColor(hdc, RGB(255, 255, 255));
	wtemp = s2ws("Restart");
	ltemp = wtemp.c_str();
	HBRUSH green = CreateSolidBrush(RGB(98, 108, 55));
	SelectBrush(hdc, green);
	RoundRect(hdc, header.Restart.left, header.Restart.top, header.Restart.right, header.Restart.bottom, 5, 5);
	//FrameRect(hdc, &header.Restart, black);
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, ltemp, 7, &header.Restart, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	DeleteObject(black);
	DeleteObject(green);
}

void Board::Loss(HDC hdc, HWND hwnd)
{
	//setup
	HDC buffer = CreateCompatibleDC(hdc);
	RECT rect;
	GetClientRect(hwnd, &rect);
	HBITMAP bitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
	SelectObject(buffer, bitmap);

	//clears board
	HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(buffer, &rect, white);
	DeleteObject(white);


	if (endTime == 0) {
		endTime = time(0);
	}
	int count = 0;
	for (int j = 0; j < board->getRowSize(); j++) {
		for (int i = 0; i < board->getColSize(); i++) {
			board->get(j, i)->drawLoss(buffer);
		}
	}
	paintHeader(buffer);

	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, buffer, 0, 0, SRCCOPY);
	DeleteObject(bitmap);
	DeleteObject(buffer);
	KillTimer(hwnd, timer);
}

void Board::Win(HDC hdc, HWND hwnd)
{
	//paintAll(hdc, hwnd);
	if (endTime == 0) {
		endTime = time(0);
	}


	bombsleft = 0;

    
	
	/*stringstream ss;
	ss << timeSpent();
	string temp = "You won in " + ss.str() + " seconds";
	wstring wtemp = s2ws(temp);
	LPCWSTR ltemp = wtemp.c_str();
	RECT Rtemp;
	RECT size;
	GetClientRect(hwnd, &size);
	Rtemp.bottom = (size.bottom *.6);
	Rtemp.top = (size.bottom * .4);
	Rtemp.left = (size.left);
	Rtemp.right = size.right;*/

	paintAll(hdc, hwnd);

	//DrawText(hdc, ltemp, temp.size(), &Rtemp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	//HBRUSH black = CreateSolidBrush(RGB(0, 0, 0));
	//FrameRect(hdc, &Rtemp, black);
	//DeleteObject(black);

	KillTimer(hwnd, timer);

	//paintHeader(hdc);
}

bool Board::checkWin()
{
	for (int r = 0; r < board->getRowSize(); r++) {
		for (int c = 0; c < board->getColSize(); c++) {
			if (!board->get(r, c)->isClicked() && !board->get(r, c)->isBomb()) {
				return false;
			}
		}
	}
	didWin = true;
	return true;
}

//void Board::restart()
//{
//	isFirst = true;
//	firstClick = true;
//	Block temp;
//	for (int r = 0; r < board->getRowSize(); r++) {
//		for (int c = 0; c < board->getColSize(); c++) {
//			board->get(r, c)->operator=(temp);
//		}
//	}
//	didLose = false;
//	endTime = 0;
//	bombsleft = defaultBombs;
//	didWin = false;
//}

void Board::paintInSections(HBITMAP bitmap, HDC hdc, HDC buffer, HWND hwnd, int divisions)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	int width = rect.right - rect.left;
	int length = rect.bottom - rect.top;
	for (int i = 0; i < divisions; i++) {
		BitBlt(hdc, 0, (length / divisions)*i, width, length / divisions, buffer, 0, (length / divisions)*i, SRCCOPY);
	}
}

wstring Board::s2ws(const string & s)
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

