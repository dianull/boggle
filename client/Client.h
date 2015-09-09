#include <unistd.h>
#include <iostream>
#include <vector>
#include <map>

class Client {

	public:
		
		Client(std::string host);
		~Client();

		void connect();

		std::string _host;
<<<<<<< HEAD
		typedef std::vector<int> indexes_t; 
		indexes_t _currentBoardLetters;
=======
		std::string _currentBoardLetters;
>>>>>>> ce9a25c0ae764866bc85bf63f5e654e05863d3f3

		void sayHello();
		void getBoard();

	

};

