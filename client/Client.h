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
		typedef std::vector<int> indexes_t; 
		indexes_t _currentBoardLetters;

		void sayHello();
		void getBoard();

	

};

