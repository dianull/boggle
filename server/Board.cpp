#include "Board.h"

using namespace std;

Board::Board() {

}

Board::~Board() {

}

string Board::getCurrentBoard() {

	return _currentBoardLetters;
}


vector<Dice*> Board::createDices() {

	std::vector<Dice*> dices;

	for (int i(0); i < 16; ++i) {

		string s = _lettersSet[i];
		Dice* dice = new Dice(s);
		dices.push_back(dice);
	}

	return dices;

}

void Board::fillBoard() {

	std::vector<Dice*> dices = createDices();

	_currentBoardLetters = "";

	srand(time(NULL));
	
	boost::locale::generator gen;
//	std::locale loc = gen("");

	for (int p(0); p < 4; ++p) {
		for (int q(0); q < 4; ++q) {
			//printf("fill board\n");
			int rand_wall = (rand() % 5);
			int rand_cube(0);
			int counter(dices.size());

			if (counter > 0)
				rand_cube = (rand() % (counter));
			else
				rand_cube = 1;

			string str = dices.at(rand_cube)->_walls;

			boost::locale::boundary::ssegment_index map(boost::locale::boundary::character, str.begin(), str.end(), gen("en_US.UTF-8")); 
			boost::locale::boundary::ssegment_index::iterator it = map.begin();
			int i = 0;
			while (i++ < rand_wall)
				it++;

			Letter l;
			l.i = p;
			l.j = q;
			l.letter = *it;
			l.visited = false;
			_board[p][q] = l;
		//	fprintf(stderr, "%s", l.letter.c_str());
		//	fflush(stderr);
			_currentBoardLetters += l.letter;
			if (counter > 0)
				dices.erase(dices.begin() + (rand_cube));

		}

	}

}

