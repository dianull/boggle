#include <clocale>
#include <locale>
#include <locale.h>
#include <boost/iostreams/categories.hpp> 
#include <boost/iostreams/code_converter.hpp>
#include <boost/locale.hpp>
#include <ncursesw/curses.h>
#include <cdk/cdk.h>
#include "Client.h"

using namespace std;
using namespace boost::locale;

WINDOW* mainWindow;
int _scrolllistsize;
int _score;
CDKSCROLL* _cdkscrollLeft;
CDKSCROLL* _cdkscrollRight;
CDKSCREEN* _cdkscreen;
char** _itemlist; 



static const wchar_t* _wlettersSet[16] = {
	L"RWIIBN",
	L"KNOEDR",
	L"YZOEAZ",
	L"OTWCS\u0104",
	L"DAY\u0118AE",
	L"BZPE\u00D3E",
	L"AKIYAM",
	L"M\u015AG\u0141CT",
	L"IRPO\u0106K",
	L"EUYAIO",
	L"NIAPNC",
	L"J\u0143FLHC",
	L"GRSOAL",
	L"UMJT\u0179Z",
	L"\u017BWEILS",
	L"N\u0143DZWH"
};

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
	vector<int> letters = client._currentBoardLetters;

	
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
	drawBoard(boardWindow, frameX, frameY);

	//fill board with letters
	int id(0);

	for (int i(2); i < 12; i += 3) {
		for (int j(3); j < 22; j += 6) {
				int diceId = letters[id];
				int letterId = letters[id + 1];
				wchar_t tmp = _wlettersSet[diceId][letterId]; 
				mvwaddwstr(boardWindow, i, j, &tmp);
				id += 2;
		}
	}
	//----

	wrefresh(mainWindow);	
	wrefresh(boardWindow);	

	WINDOW* listWindow;
	drawScrollList(listWindow, frameY, frameX);

	WINDOW* inputWindow = newwin(8, 30, frameY + 27, frameX + 3);
 	mvwprintw(inputWindow, 1, 1, "%s", "What words do you see?");
	wrefresh(inputWindow);
		

	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	
	char input[255];
	char final_input[255];
	memset(input, 0, 255);
	memset(final_input, 0, 255);

	generator gen;
	std::locale loc = gen("");

	echo();
	while (true) {
		wmove(inputWindow, 3, 1);
	  wclrtoeol(inputWindow);
		wgetstr(inputWindow, input);
	  wclrtobot(inputWindow);
			
		wclrtoeol(inputWindow);
	

		strcat(final_input, input);
		string s = string(input);
	
		s = fold_case(normalize(s, norm_default, loc), loc);

 //  wattron(inputWindow, COLOR_PAIR(1));
 //  wattroff(inputWindow, COLOR_PAIR(1));

	string searchWord = string(input); 

	if (strlen(input) >= 3 /*&& isOnBoard() && is_from_dictionary(input)*/) {

		++_scrolllistsize;
//		calculate_score(input, &_score);


		insertCDKScrollItem(_cdkscrollLeft, final_input);
//		_inputedwords.push_back(s);
		mvwprintw(listWindow, 30, 10, "%d", _score);
		mvwprintw(inputWindow, 5, 1, "%s", searchWord.c_str());
		wrefresh(listWindow);
		setCDKScrollCurrentTop(_cdkscrollLeft, 0);
		drawCDKScroll(_cdkscrollLeft, true);
		
		} else {
			mvwprintw(inputWindow, 5, 1, "%s", "zle");
		}
				
	}

	endwin();
	return 0;

}

