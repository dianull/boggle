#include <unistd.h>
#include <iostream>

class Client {

	public:
		
		Client(std::string host);
		~Client();

		void connect();

		std::string _host;
		std::string _currentBoardLetters;

		void sayHello();
		void getBoard();

	

};

