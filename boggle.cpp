#include <curses.h>
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

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4

using namespace std;

static const short _dicesWalls[16][6] = {
	{'R', 'W', 'I', 'I', 'B', 'N'},
	{'K','N', 'O', 'E', 'D', 'R'},
	{'Y','Z', 'O', 'E', 'A', 'Z'},
	{'O','T', 'W', 'C', 'S', '≈Å'},
	{'D', 'A', 'Y', '√ì', 'A', 'E'},
	{'B', 'Z', 'P', 'E', '≈ö', 'E'},
	{'A', 'K', 'I', 'Y', 'A', 'M'},
	{'M', '≈π', 'G', '≈ª', 'C', 'T'},
	{'I', 'R', 'P', 'O', 'ƒò', 'K'},
	{'E', 'U', 'Y', 'A', 'I', 'O'},
	{'N', 'I', 'A', 'P', 'N', 'C'},
	{'J', 'ƒÜ', 'F', 'L', 'H', 'S'},
	{'G', 'R', 'S', 'O', 'A', 'L'},
	{'U', 'M', 'J', 'T', 'ƒÑ', 'Z'},
	{'≈É', 'W', 'E', 'I', 'L', 'S'},
	{'N', '≈É', 'D', 'Z', 'W', 'H'}};

//	int width(6);
char *choices[] = {
	  "Choice 1",
	   "Choice 2",
	   "Choice 3",
	 };

struct Cube {
	const char* _walls;
	Cube(const char* letters) : _walls(letters)
	{}
};

typedef vector<Cube*> _cubes;
vector<char> _lettersOnBoard;
char** _board;
vector<string> _dictionary;
vector<string> _inputedwords;
WINDOW *create_newwin(int height_, int width_, int starty_, int startx_, _cubes* list_);
char** _itemlist; 
int _scrolllistsize;
int _score;
CDKSCROLL* _cdkscroll;
CDKSCREEN* _cdkscreen;

const int di[] = {-1, 0, 1, -1, 1, -1, 0, 1};
const int dj[] = {-1, -1, -1, 0, 0, 1, 1, 1};

bool is_from_dictionary(char* input_) {/*{{{*/
	string s = string(input_);		
	boost::to_lower(s); //because earlier we do to_upper in is_on_board()
	return (std::find(_dictionary.begin(), _dictionary.end(), s) != _dictionary.end());
}
/*}}}*/

void read_dictionary() {/*{{{*/
    string name;
 		std::string filename("dict.txt");
    boost::iostreams::stream<boost::iostreams::file_source> file(filename.c_str());
    std::string line;
    while (std::getline(file, line)) {
      _dictionary.push_back(line);
    }
}
/*}}}*/

bool into_the_board(string word_, int i_, int j_) {/*{{{*/
	char ch = _board[i_][j_];//s from board - upper
	char ch2 = word_[0];//s2 from input - lower
	toupper(ch2);
			stringstream ss;
			string s;
			ss << ch;
			ss >> s;
			stringstream sss;
			string s2;
			sss << ch2;
			sss >> s2;
	if (word_.length() == 0)
		return true;
	if (s2 == s) {
		for (int k(0); k < 8; ++k) {
			if (i_ + di[k] >= 0 && i_ + di[k] < 4 && j_ + dj[k] >= 0 && j_ + dj[k] < 4) {
				s = '!';
				bool is = into_the_board(word_.substr(1), i_ + di[k], j_ + dj[k]);
				s = s2;
				if (is)
					return true;
			}	
		}
	}  
	return false;
}
/*}}}*/

bool is_on_board(const char* input_) {/*{{{*/
	string s = string(input_);
	boost::to_upper(s);
	
	for (int i(0); i < 4; ++i) {
		for (int j(0); j < 4; ++j) {
			if (s[0] == _board[i][j]) {
				for (int k(0); k < 8; ++k) {
					if (i + di[k] >= 0 && i + di[k] < 4 && j + dj[k] >= 0 && j + dj[k] < 4) {
						_board[i][j] = '!';
						bool is = into_the_board(s.substr(1), i + di[k], j + dj[k]);
						_board[i][j] = s[0];
						if (is)
							return true;
					}	
				}
			}
		}
	}
	return false;
}
/*}}}*/

void validate_letter_case(char* c_) { //converting polish diacritics to lowercase/*{{{*/
	switch (*c_) {
		case '°':
			*c_ = '±';
			break;
		case ' ':
			*c_ = 'Í';
			break;
		case '”':
			*c_ = 'Û';
			break;
		case '£':
			*c_ = '≥';
			break;
		case '¨':
			*c_ = 'º';
			break;
		case 'Ø':
			*c_ = 'ø';
			break;
		case '—':
			*c_ = 'Ò';
			break;
		case '¶':
			*c_ = '∂';
			break;
		case '∆':
			*c_ = 'Ê';
			break;
	}

}/*}}}*/

_cubes init_cubes() {/*{{{*/ //creates cubes (dices) to rand from

 	_cubes cubes;

	for (int i(0); i < 16; ++i) {
		char* c = new char[7];
		for (int j(0); j < 6; ++j) {
			c[j] = _dicesWalls[i][j];
		}

		c[6] = 0;
		Cube* cube = new Cube(c);
		cubes.push_back(cube);
	}

 	return cubes;
}
/*}}}*/

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
/*}}}*/

bool is_duplicated(const string& input_) {/*{{{*/
		return (find(_inputedwords.begin(), _inputedwords.end(), input_) != _inputedwords.end());
}
/*}}}*/

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
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);

	_score = 0;
	_scrolllistsize = 0;
	_itemlist = new char*[80];

	for (int i(0); i < 80; ++i) {
		_itemlist[i] = new char[40];
		for (int j(0); j < 40; ++j) {
			_itemlist[i][j] = '\0';
		}
	}

	WINDOW* listwin = newwin(12, 20, startx_, starty_);
	_cdkscreen = initCDKScreen(listwin);
	
	char title[] = "Your matches:";
	char* t = title;
	//_cdkscroll = newCDKScroll(_cdkscreen, 10, 10, 10, 80, 30, t, _itemlist, 1, true, 0, true, false);

	//deleteCDKScrollItem(_cdkscroll, getCDKScrollCurrentTop(_cdkscroll));
	//drawCDKScroll(_cdkscroll, true);
 	//mvprintw(startx_ + 12, starty_ + 36, "%s", "Score:");
}

void init_board() {/*{{{*/
	
	_board = new char*[16];

	for (int i(0); i < 16; ++i) {
		_board[i] = new char[1];
		for (int j(0); j < 7; ++j) {
			_board[i][j] = 'x';
		}
	}
/*	
	int c(0);
	for (int p(0); p < 4; ++p) {
		for (int q(0); q < 4; ++q) {
			char tmp = _lettersOnBoard.at(c++);
			validate_letter_case(&tmp); //convert diacritics to lower case 
			_board[p][q] = tmp;
		}
	}*/
}
/*}}}*/


int main(int argc, char *argv[]) {/*{{{*/
//	setlocale(LC_ALL, "");
	initscr();
 	cbreak();
 	keypad(stdscr, TRUE);
	int row, col;
	getmaxyx(stdscr, row, col);
	int frameX = (col / 2) - (col / 4);
	int frameY = (row / 2) - (row/1.7/2);
	int frameHeight = row / 1.7;
	int frameWidth = col / 2;
	mvprintw(1, 2, "%d %d", frameY * 1.6 / 4, frameX / 1.3);
	refresh();

	start_color();
	// init_color(COLOR_BLUE, 200, 196, 225);
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_GREEN);
 	WINDOW* mainWindow = newwin(frameHeight, frameWidth, frameY, frameX);
	wborder(mainWindow, 0, 0, 0, 0, 0, 0, 0, 0);
	wbkgd(mainWindow, COLOR_PAIR(1));

 	WINDOW* boardWindow = newwin(21, 37, frameY + 2, frameX + 3);
	wbkgd(boardWindow, COLOR_PAIR(2));
	
	mvwhline(boardWindow, 5 , 0 , 0, frameX / 1.3);
	mvwhline(boardWindow, 10, 0 , 0, frameX / 1.3);
	mvwhline(boardWindow, 15, 0 , 0, frameX / 1.3);

	mvwvline(boardWindow, 0, 9, 0, frameY * 1.6);
	mvwvline(boardWindow, 0, 18,0, frameY * 1.6);
	mvwvline(boardWindow, 0, 27, 0, frameY * 1.6);

	wborder(boardWindow, 0, 0, 0, 0, 0, 0, 0, 0);

	wrefresh(mainWindow);	
	wrefresh(boardWindow);	

	//init_board();

//	_cubes cubes_list = init_cubes();
//	read_dictionary();
//	srand(time(NULL));
//	prepare_gui(row/2, col/2, 10, 10);
//	refresh();
//	init_board();
//	echo();
/*
	int key(0);
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
			cubes_list = init_cubes();
			init_board();
			reset_game();
			refresh();
		 } else {
			char ch = static_cast<char>(key);
			getstr(input);
			new_input[0] = ch;

			stringstream ss;
			string s;
			ss << ch;
			ss >> s;

			const char* mych = s.c_str();
			strcat(final_input,mych );
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
			clrtoeol();//?
			move(row + 12, col);
			clrtoeol();//bot?
	 }
	}*/

//	endgame();
 	endwin();
 	return 0;
}
/*}}}*/
/*
WINDOW *create_newwin(int height_, int width_, int starty_, int startx_, _cubes* list_) {
 	//box(local_win, 0 , 0);
	int rand_wall = (rand() % 5);
	int rand_cube(0);
	int counter(list_->size());

	if (counter > 0)
		rand_cube = (rand() % (counter));
	else
		rand_cube = 1;

	char c[2] = "1";
	*c = list_->at(rand_cube)->_walls[rand_wall];
	_lettersOnBoard.push_back(*c);
 	//mvwaddstr(local_win, 1.7, 2, c);
	if (counter > 0)
		list_->erase(list_->begin() + (rand_cube));
 	//wrefresh(local_win);
 	return local_win;
}*/


