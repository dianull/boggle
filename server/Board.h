#include <iostream>
#include <stdio.h>
#include <boost/locale.hpp>
#include <boost/algorithm/string.hpp>
#include <utility>

static const std::string _lettersSet[16] = {
	"rwiibn",
	"knoedr",
	"yzoeaz",
	"otwcsą",
	"dayęae",
	"bzpeóe",
	"akiyam",
	"mśgłct",
	"irpoćk",
	"euyaio",
	"niapnc",
	"jńflhc",
	"grsoal",
	"umjtźz",
	"żweils",
	"nńdzwh"
};


struct Dice {
	std::string _walls;
	Dice(std::string letters) : _walls(letters) {}

};

struct Letter {
  Letter() {}
  Letter(const Letter& copy) {
    letter = copy.letter;
    i = copy.i;
    j = copy.j;
    visited = copy.visited;
  }
	std::string letter;
  int i;
  int j;
  bool visited;
};


class Board {
	
	public:
		Board();
		~Board();

		typedef std::pair<int, int> indexes_t;
		Letter _board[4][4];

		std::vector<Dice*> createDices();
		void fillBoard();
		std::string getCurrentBoard();
		std::string _currentBoardLetters;
		
};

