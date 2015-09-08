#include <clocale>
#include <locale>
#include <locale.h>
#include <ncursesw/curses.h>
#include <cdk/cdk.h>
#include "Client.h"

WINDOW* mainWindow;
int _scrolllistsize;
int _score;
CDKSCROLL* _cdkscrollLeft;
CDKSCROLL* _cdkscrollRight;
CDKSCREEN* _cdkscreen;
char** _itemlist; 

void drawScrollList(WINDOW*& listWindow, int frameY, int frameX) {
	_itemlist = new char*[80];
	for (int i(0); i < 80; ++i) {
		_itemlist[i] = new char[40];
		for (int j(0); j < 40; ++j) {
			_itemlist[i][j] = '\0';
		}
	}

	listWindow = newwin(30, 50, frameY + 2, frameX + 40);
//	wborder(listWindow, 0, 0, 0, 0, 0, 0, 0, 0);
 	mvwprintw(listWindow, 30, 1, "%s", "Score:");
 	mvwprintw(listWindow, 30, 28, "%s", "Score:");
	wrefresh(listWindow);
	_cdkscreen = initCDKScreen(listWindow);
	_cdkscrollLeft = newCDKScroll(_cdkscreen, LEFT, 10, RIGHT, 30, 22, "Your matches", _itemlist, 1, true, 0, true, false);
	_cdkscrollRight = newCDKScroll(_cdkscreen, RIGHT, 10, RIGHT, 30, 22, "Opponent matches", _itemlist, 1, true, 0, true, false);
	drawCDKScroll(_cdkscrollLeft, true);
	drawCDKScroll(_cdkscrollRight, true);

	_score = 0;
	_scrolllistsize = 0;
	

}

void drawBoard(WINDOW*& w, int frameX, int frameY) {
//	wbkgd(w, COLOR_PAIR(2));
	
	mvwhline(w, 3 , 1, 0, frameX);
	mvwhline(w, 6, 1, 0, frameX);
	mvwhline(w, 9, 1, 0, frameX);

	mvwvline(w, 1, 6, 0, frameY * 1.8);
	mvwvline(w, 1, 12, 0, frameY * 1.8);
	mvwvline(w, 1, 18, 0, frameY * 1.8);

	wborder(w, 0, 0, 0, 0, 0, 0, 0, 0);
}

int main(int, char**) {

	Client client("tcp://localhost:5555");
	client.sayHello();

	std::string letters = client._currentBoardLetters;

	
	setlocale(LC_ALL, "");
	initscr();
	clear();
 	cbreak();
 	keypad(stdscr, TRUE);
	int row, col;
	getmaxyx(stdscr, row, col);
	int frameX = (col / 2) - (col / 3);
	int frameY = (row / 2) - (row / 1.4 / 2);
	int frameHeight = row / 1.4;
	int frameWidth = col / 1.5;

	//init_color(COLOR_BLUE, 123, 196, 225);
 	mainWindow = newwin(frameHeight, frameWidth, frameY, frameX);
	wborder(mainWindow, 0, 0, 0, 0, 0, 0, 0, 0);

 	WINDOW* boardWindow = newwin(13, 25, frameY + 2, frameX + 3);
//	wborder(boardWindow, 0, 0, 0, 0, 0, 0, 0, 0);
	drawBoard(boardWindow, frameX, frameY);

	for (int i(2),  p(0); i < 12; i += 3) {
		for (int j(3), q(0); j < 22; j += 6) {
			// wchar_t tmp = _board[p][q++]; //_lettersOnBoard.at(id++);
	//		const wchar_t* wstr = L"\u0119";
				const wchar_t* tmp = L"\u0104"; 
				mvwaddwstr(boardWindow, i, j, tmp);
		//		const wchar_t* x = s.c_str();
		//		_board[p][q] = *x;
		//		q++;
		}
		p++;
	}


	wrefresh(mainWindow);	
	wrefresh(boardWindow);	

	WINDOW* listWindow;
	drawScrollList(listWindow, frameY, frameX);

	WINDOW* inputWindow = newwin(8, 30, frameY + 27, frameX + 3);
	//wborder(inputWindow, 0, 0, 0, 0, 0, 0, 0, 0);
 	mvwprintw(inputWindow, 1, 1, "%s", "What words do you see?");
	wrefresh(inputWindow);
		
//	read_dictionary();

	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	
	char input[255];
	char final_input[255];
	memset(input, 0, 255);
	memset(final_input, 0, 255);

//	echo();




	endwin();
	return 0;

}

