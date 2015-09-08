#include <stdio.h>
#include <string.h>
#include "Client.h"
#include <zmq.hpp>
#include <errno.h>

Client::Client(std::string host) : _host(host) {


}

Client::~Client() {

}

void Client::connect() {
	

}

void Client::sayHello() {


	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REQ);


	socket.connect ("tcp://localhost:5555");
	

	size_t requestLen = 1 + 2 + 8;

	zmq::message_t request(requestLen);

	unsigned char req[requestLen];
	req[0] = 1;
	unsigned short dataLen = 7;
	req[1] = (unsigned char) (dataLen >> 8);
	req[2] = (unsigned char) (dataLen);
	memcpy(&req[3], "dianull", dataLen);
	req[10] = '\0';

	memcpy(request.data(), req, requestLen);

	socket.send(request);

	
	zmq::message_t reply;
	socket.recv(&reply);
	std::string rpl = std::string(static_cast<char*>(reply.data()), reply.size());

	printf("server reply %s of size %d\n", rpl.c_str(), reply.size());
	


}

