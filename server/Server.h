#include <iostream>
#include <unistd.h>
#include <zmq.hpp>
#include "Board.h"

class Server {


	public:
		
		Server(std::string host);
		~Server();

		enum GameState { NO_GAME = 0, IN_PROGRESS };

		GameState _state;

		void init();
		std::string _host;

};
