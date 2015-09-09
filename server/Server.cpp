#include "Server.h"

using namespace std;

static const int HELLO = 0;
static const int BOARD_REQ = 1;
static const int WORD_REQ = 2;
static const int SCORE_REQ = 3;


Server::Server(std::string host) : _host(host) {

	init();
	_state = NO_GAME;
	
}

Server::~Server() {

}


void Server::init() {
	

	zmq::context_t context(1);
	zmq::socket_t socket (context, ZMQ_REP);
	socket.bind(_host.c_str());
	cout << "Server bound at " << _host << endl;

	while (true) {
			zmq::message_t request;

			socket.recv (&request);
			string rpl = std::string(static_cast<char*>((char*)request.data()), request.size());
			
			uint8_t id = rpl[0];
			short dsize = rpl[1];
			dsize <<= 8;
			dsize |= rpl[2];

			printf("Incoming request of data size %d\n", dsize);

			switch (id) {
				case HELLO: {
				//	string data = rpl.substr(3);
				//	printf("Received HELLO from %s\n", data.c_str());

					zmq::message_t reply(2);
					memcpy((void*)reply.data(), "OK", 2);
					socket.send(reply);
				}
				break;
				case BOARD_REQ:
					printf("Received BOARD_REQ\n");
					Board board;
					board.fillBoard();
					Board::indexes_t letters = board.getCurrentBoard();
				//	printf("Sending %s\n", letters.c_str());
					int len = 16 * 2 * (sizeof(int));
					zmq::message_t reply2(len);

					memcpy((void*)reply2.data(), letters.data(), len);
					socket.send(reply2);
				break;
				

			}

	}	

}

