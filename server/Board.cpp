#include "Board.h"
#include <map>

using namespace std;

Board::Board() {

}

Board::~Board() {

}

Board::indexes_t Board::getCurrentBoard() {

	return _currentBoardLettersIds;
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

	_currentBoardLettersIds.clear();

	boost::locale::generator gen;
	map<string, int> dicesIds;
	for (int i(0); i < 16; ++i) {
		string s = _lettersSet[i];

		boost::locale::boundary::ssegment_index map(boost::locale::boundary::character, s.begin(), s.end(), gen("en_US.UTF-8")); 
		boost::locale::boundary::ssegment_index::iterator it = map.begin();
		string c = *it;
		dicesIds[c] = i;
	}

	srand(time(NULL));
	
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
			int diceId = dicesIds[*it];
			while (i++ < rand_wall)
				it++;

			Letter l;
			l.i = p;
			l.j = q;
			l.letter = *it;
			l.visited = false;
			_board[p][q] = l;
			
			int letterId = rand_wall;

			fprintf(stderr, "%s %d %d\n", l.letter.c_str(), diceId, letterId);
			fflush(stderr);
			_currentBoardLettersIds.push_back(diceId);
			_currentBoardLettersIds.push_back(letterId);
			
			if (counter > 0)
				dices.erase(dices.begin() + (rand_cube));

		}

	}

}

