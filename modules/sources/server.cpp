#include "server.hpp"

Server::Server(int port) : Thread() {
    std::cout << "Port Number: " << port << std::endl;
    if (!_sock.create(port)) {
        throw "An error has occured while creating the server socket.";
    }
}

Server::~Server() {

}

void Server::listen() {
	int activity, max;
	struct timeval timeOut;
	timeOut.tv_sec = 0;
	timeOut.tv_usec = 0;
    std::cout << "Server is now listening on " <<  _sock.getPortNumber() << std::endl;
    while (true) {
		
	
            if (_jobs.size() < MAX_CONNECTIONS) {
                UDPSocket clientSocket;
                std::string msg;
		
		
                if (_sock.recvFrom(clientSocket, msg) == -1) {
                    perror("Cannot recieve from the client.");
                }

                std::cout << "Client Socket: " << clientSocket.getPortNumber() << std::endl;
                std::cout << "Messaged Received From  " << clientSocket.getHost() << ":" << clientSocket.getPortNumber() << "-> " << msg << std::endl;
                Job *job = new Job(clientSocket);
                job -> setParent((void *)this);
                job -> setDoneCallback(_callbackWrapper, (void *)this);
                job -> start();
                _jobs.push_back(job);
            }
    }
}

void Server::accept(UDPSocket& client) {
    if (_clients.size() == MAX_CONNECTIONS) {
        throw "Maximum number of connections, connection rejected";
    } else {
        _clients.push_back(&client);
    }
}

void Server::run() {
    listen();
}

int Server::getServerPort() {
    return _sock.getPortNumber();
}

void Server::_terminateJob(Job *job) {
    std::vector<Job*>::iterator it = std::find(_jobs.begin(), _jobs.end(), job);
    if (it != _jobs.end()) {
        (*it) -> exit();
        _jobs.erase(it);
    }
}

void Server::writeBuffer(char * msg) {
    // _buffer = msg;
}

char * Server::readBuffer(int start, int end)  {
    return _buffer;
}

void * Server::_callbackWrapper(Thread *thread, void *parent) {
    std::cout << "Callback Wrapper!" << std::endl;
    Job *job = (Job *) thread;
    Server *server = (Server *) parent;
    server -> _terminateJob(job);
    return (void *) thread;
}
