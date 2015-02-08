#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/categories.hpp> 
#include <boost/iostreams/code_converter.hpp>
#include <ncursesw/curses.h>"
#include <boost/locale.hpp>
#include <menu.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sstream>
#include <cdk.h>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include <iterator>
#include <algorithm>
#include <set>
#include <boost/algorithm/string.hpp>
#include <locale.h>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <ostream>
#include <stack>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4

using namespace std;
using namespace boost::locale;

/*
static const short _dicesWalls[16][6] = {
	{'R', 'W', 'I', 'I', 'B', 'N'},
	{'K','N', 'O', 'E', 'D', 'R'},
	{'Y','Z', 'O', 'E', 'A', 'Z'},
	{'O','T', 'W', 'C', 'S', '¿'},
	{'D', 'A', 'Y', '¿', 'A', 'E'},
	{'B', 'Z', 'P', 'E', '¿', 'E'},
	{'A', 'K', 'I', 'Y', 'A', 'M'},
	{'M', 'Ó', 'G', '¿', 'C', 'T'},
	{'I', 'R', 'P', 'O', '¿', 'K'},
	{'E', 'U', 'Y', 'A', 'I', 'O'},
	{'N', 'I', 'A', 'P', 'N', 'C'},
	{'J', '¿', 'F', 'L', 'H', 'S'},
	{'G', 'R', 'S', 'O', 'A', 'L'},
	{'U', 'M', 'J', 'T', '¿', 'Z'},
	{'¿', 'W', 'E', 'I', 'L', 'S'},
	{'N', '¿', 'D', 'Z', 'W', 'H'}};
*/

static const wchar_t* _dicesWalls[16] = {
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

///	int width(6);
char *choices[] = {
	  "Choice 1",
	   "Choice 2",
	   "Choice 3",
	 };

struct Dice {
	const wchar_t* _walls;
	Dice(const wchar_t* letters) : _walls(letters)
	{}
};

struct Letter {
	Letter() {}
	Letter(const Letter& copy) {
		letter = copy.letter;
		i = copy.i;
		j = copy.j;
		visited = copy.visited;
	}
	wstring letter;
	int i;
	int j;
	bool visited;
};


string searchWord;
typedef vector<Dice*> _dices;
wchar_t _board[4][4];
Letter _boardLowerCase[4][4];
vector<string> _dictionary;
vector<string> _inputedwords;
WINDOW *create_newwin(int height_, int width_, int starty_, int startx_, _dices* list_);
char** _itemlist; 
int _scrolllistsize;
int _score;
CDKSCROLL* _cdkscrollLeft;
CDKSCROLL* _cdkscrollRight;
CDKSCREEN* _cdkscreen;
WINDOW* mainWindow;

const int di[] = {-1, 0, 1, -1, 1, -1, 0, 1};
const int dj[] = {-1, -1, -1, 0, 0, 1, 1, 1};

bool is_from_dictionary(char* input_) {/*{{{*/
	string s = string(input_);		
	boost::to_lower(s); //because earlier we do to_upper in is_on_board()
	return (std::find(_dictionary.begin(), _dictionary.end(), s) != _dictionary.end());
}
/*}}}*/

void read_dictionary() {
    string name;
 		std::string filename("dict.txt");
    boost::iostreams::stream<boost::iostreams::file_source> file(filename.c_str());
    std::string line;
    while (std::getline(file, line)) {
      _dictionary.push_back(line);
    }
}


stack<Letter> getNeighbours(int i, int j) { 
	stack<Letter> neighbours;
	for (int k(0); k < 8; ++k) {
		if (i + di[k] >= 0 && i + di[k] < 4 && j + dj[k] >= 0 && j + dj[k] < 4) {
			Letter l = _boardLowerCase[i + di[k]][j + dj[k]];
			neighbours.push(l/*_boardLowerCase[i + di[k]][j + dj[k]]*/);

		}	
	}
	return neighbours;
}

int tries(0);
stack<Letter> neighbours;

bool checkBoard(string word, Letter l) {
	
	if (!l.visited) {
		if (word[0] == l.letter[0]) {
			l.visited = true;
			tries++;
			std::cout << tries << "/" << searchWord.length();
			stack<Letter> neighbours = getNeighbours(l.i, l.j);
			
			while (!neighbours.empty()) {
				Letter l = neighbours.top();
				neighbours.pop();
						
				if (l.letter[0] == searchWord[tries]) {
					if (tries == searchWord.length() -1)
						return true;
					checkBoard(searchWord.substr(tries), l);
				}
			}
		}
	}
	return false;
}

bool is_on_board() {
	tries = 0;
	for (int i(0); i < 4; ++i) {
		for (int j(0); j < 4; ++j) {
			
			Letter l = _boardLowerCase[i][j];
	//		if (searchWord[0] == _boardLowerCase[i][j][0])
			if (!checkBoard(searchWord, l)) {
				continue;
			} else {
				tries = 0;
				return true;
			}
		}
	}
	return false;
}

void validate_letter_case(char* c_) {
	switch (*c_) {
		case '¡':
			*c_ = '±';
			break;
		case 'Ê':
			*c_ = 'ê';
			break;
		case 'Ó':
			*c_ = 'ó';
			break;
		case '£':
			*c_ = '³';
			break;
		case '¬':
			*c_ = '¼';
			break;
		case '¯':
			*c_ = '¿';
			break;
		case 'Ñ':
			*c_ = 'ñ';
			break;
		case '¦':
			*c_ = '¶';
			break;
		case 'Æ':
			*c_ = 'æ';
			break;
	}

}

_dices initDices() {

 	_dices dices;

	for (int i(0); i < 16; ++i) {

		const wchar_t* str = _dicesWalls[i];

		Dice* dice = new Dice(str);
		dices.push_back(dice);
	}

 	return dices;
}

void calculate_score(const char* input_, int* score_) {/*{{{*/
	int l(strlen(input_));
	switch(l) {
		case 3:
			++*score_; //!!!
			break;
		case 4:
			++*score_;
			break;
		case 5:
			*score_ += 2;
			break;
		case 6:
			*score_ += 3;
			break;
		case 7:
			*score_ += 5;
			break;
		default:
			*score_ += 11;
			break;
	}
}

bool is_duplicated(const string& input_) {/*{{{*/
		return (find(_inputedwords.begin(), _inputedwords.end(), input_) != _inputedwords.end());
}

void reset_game() {/*{{{*/
//	_itemlist = new char*[15];
	for (int i(0); i < 100; ++i) {
		_itemlist[i] = new char[81];
		for (int j(0); j < 30; ++j) {
			_itemlist[i][j] = '\0';
		}
	}
	_score = 0;
}
/*}}}*/

void endgame() {
	delete[] _board;

}

void prepare_gui(int startx_, int starty_, int liststartx_, int liststarty_ ) {
	mvprintw(0, 0, "%s", "*** Press Q to quit\n*** Press TAB to switch between scroll list and input\n*** Press R to get new board");
 	mvprintw(startx_ + 10, starty_, "%s", "What words do you see?");
}


void fillBoard(WINDOW* w, _dices list) {

	srand(time(NULL));
	

	generator gen;
	std::locale loc = gen("");

	for (int p(0); p < 4; ++p) {
		for (int q(0); q < 4; ++q) {
			int rand_wall = (rand() % 5);
			int rand_cube(0);
			int counter(list.size());

			if (counter > 0)
				rand_cube = (rand() % (counter));
			else
				rand_cube = 1;

	//		char c[2] = "1";
			wchar_t c = list.at(rand_cube)->_walls[rand_wall];
	//		_lettersOnBoard.push_back(*c);
			_board[p][q] = c;
				wstring s(&c);
				s = fold_case(normalize(s, norm_default, loc), loc);

			Letter l;
			l.i = p;
			l.j = q;
			l.letter = s;
			l.visited = false;
			_boardLowerCase[p][q] = l; //s;
			//mvwaddstr(local_win, 1.7, 2, c);
			if (counter > 0)
				list.erase(list.begin() + (rand_cube));
		}

	}

	//wattron(w, A_BOLD);


	int id(0);
	for (int i(2),  p(0); i < 12; i += 3) {
		for (int j(3), q(0); j < 22; j += 6) {
			 wchar_t tmp = _board[p][q++]; //_lettersOnBoard.at(id++);
	//		const wchar_t* wstr = L"\u0119";
				mvwaddwstr(w, i, j, &tmp);
		//		const wchar_t* x = s.c_str();
		//		_board[p][q] = *x;
		//		q++;
		}
		p++;
	}

//	wattroff(w, A_BOLD);

}

void drawBoard(WINDOW* w, int frameX, int frameY) {
	wbkgd(w, COLOR_PAIR(2));
	
	mvwhline(w, 3 , 0 , 0, frameX / 1.3);
	mvwhline(w, 6, 0 , 0, frameX / 1.3);
	mvwhline(w, 9, 0 , 0, frameX / 1.3);

	mvwvline(w, 0, 6, 0, frameY * 1.6);
	mvwvline(w, 0, 12,0, frameY * 1.6);
	mvwvline(w, 0, 18, 0, frameY * 1.6);

	wborder(w, 0, 0, 0, 0, 0, 0, 0, 0);
}

void drawScrollList(WINDOW*& listWindow, int frameY, int frameX) {
	_itemlist = new char*[80];
	for (int i(0); i < 80; ++i) {
		_itemlist[i] = new char[40];
		for (int j(0); j < 40; ++j) {
			_itemlist[i][j] = '\0';
		}
	}

	listWindow = newwin(32, 50, frameY + 2, frameX + 40);
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


int main(int argc, char *argv[]) {

	setlocale(LC_ALL, "");
	initscr();
	clear();
 	cbreak();
 	keypad(stdscr, TRUE);
	int row, col;
	getmaxyx(stdscr, row, col);
	int frameX = (col / 2) - (col / 4);
	int frameY = (row / 2) - (row/1.7/2);
	int frameHeight = row / 1.7;
	int frameWidth = col / 2;
//const wchar_t* wstr = L"<\u0119>";
//mvwaddwstr(stdscr, 0, 0, wstr);

	//init_color(COLOR_BLUE, 123, 196, 225);
 	mainWindow = newwin(frameHeight, frameWidth, frameY, frameX);
	wborder(mainWindow, 0, 0, 0, 0, 0, 0, 0, 0);

 	WINDOW* boardWindow = newwin(13, 25, frameY + 2, frameX + 3);
	drawBoard(boardWindow, frameX, frameY);

	_dices dicesList = initDices();
	fillBoard(boardWindow, dicesList);
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
	echo();

	generator gen;
	std::locale loc = gen("");


	while (true) {
		wmove(inputWindow, 3, 1);
		wclrtoeol(inputWindow);
		wgetstr(inputWindow, input);
		wclrtobot(inputWindow);

		string s = string(input);

		s = fold_case(normalize(s, norm_default, loc), loc);

		//	wattron(inputWindow, COLOR_PAIR(1));
		//	wattroff(inputWindow, COLOR_PAIR(1));
		searchWord = string(input); 
		if (strlen(input) >= 3 && is_on_board() /*&& is_from_dictionary(input)*/) {

			++_scrolllistsize;
			calculate_score(input, &_score);
			insertCDKScrollItem(_cdkscrollLeft, input);
			_inputedwords.push_back(s);
			mvwprintw(listWindow, 30, 10, "%d", _score);
			mvwprintw(inputWindow, 5, 1, "%s", s.c_str());
			wrefresh(listWindow);
			setCDKScrollCurrentTop(_cdkscrollLeft, 0);
			drawCDKScroll(_cdkscrollLeft, true);

		} else {
			mvwprintw(inputWindow, 5, 1, "%s", "zle");
		}
		

	}

//	srand(time(NULL));

	int key(0);
	/*
	while (true) {
		char input[255];
		char new_input[255];
		char final_input[255];
		memset(input, 0, 255);
		memset(new_input, 0, 255);
		memset(final_input, 0, 255);

		move(row + 12, col);
	//	getstr(input);
		//getch();
	  //refresh();
		key = getch();
		//clrtobot();//bot?
		if (key == '\t') {
			activateCDKScroll(_cdkscroll, 0);
		}
		else if (key == 'Q') {
			endgame();
			endwin();
			exit(0);
		} else if (key == 'R') {
			refresh();
		 } else {
			char ch = static_cast<char>(key);
			getstr(input);
	//		mvwaddstr(stdscr, 0, 0, input);

			new_input[0] = ch;

			stringstream ss;
			string s;
			ss << ch;
			ss >> s;

			const char* mych = s.c_str();
			strcat(final_input, mych);
			strcat(final_input, input);
		//	clrtobot();//bot?
			if (strlen(final_input) >= 3 && is_on_board(final_input) && is_from_dictionary(final_input)) {
					stringstream sss;
					string ss;
					sss << final_input;
					sss >> ss;

				if (is_duplicated(ss)) {
					attron(COLOR_PAIR(3));
					mvprintw(row + 14, col, "%s", final_input);
					attroff(COLOR_PAIR(3));
					setCDKScrollHighlight(_cdkscroll, COLOR_PAIR(3));

				} else {
					++_scrolllistsize;
					calculate_score(final_input, &_score);
					insertCDKScrollItem(_cdkscroll, final_input);
					_inputedwords.push_back(ss);
					attron(COLOR_PAIR(2));
					mvprintw(row + 12, col + 43, "%d", _score);
					mvprintw(row + 14, col, "%s", final_input);
					attroff(COLOR_PAIR(2));
					setCDKScrollCurrentTop(_cdkscroll, 0);
					drawCDKScroll(_cdkscroll, true);
				}
			} else {
				attron(COLOR_PAIR(1));
				mvprintw(row + 14, col, "%s", final_input);
				attroff(COLOR_PAIR(1));
			}
	//		clrtoeol();//?
	//		move(row + 12, col);
			clrtoeol();//bot?
	 } 
	} */

//	endgame();
 	endwin();
 	return 0;
}


