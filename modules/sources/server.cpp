/**
 * class Server
 * Description: A class for encuaplsating and abstracting the server object, it inherits from thread
 * Author: Group 3
 * Date: 24th October 2016
 *  Project One - CSCE 4411
 */
#include "server.hpp"

Server::Server(std::string ownerId, int port) : Thread(), _ownerId(ownerId) {

    std::cout << "Port Number: " << port << std::endl;

    if (!_sock.create(port)) {

        throw "An error has occured while creating the server socket.";

    }

    _isRunning = true;
}

Server::~Server() {

}

void Server::_spawnJob(UDPSocket clientSocket) {

    Job *job = new Job(_ownerId, clientSocket);

    job -> setParent((void *)this);
    job -> setDoneCallback(_callbackWrapper, (void *)this);
    job -> start();

    _jobs.push_back(job);
}

void Server::listen() {

    std::cout << "Server is now listening on " <<  _sock.getPortNumber() << std::endl;

    while (_isRunning) {

        if (_jobs.size() < MAX_CONNECTIONS) {

            UDPSocket clientSocket;
            std::string serializedMsg;



            if (_sock.recvFrom(clientSocket, serializedMsg) != -1) {

                std::cout << "Client Socket: " << clientSocket.getPortNumber() << std::endl;
                std::cout << "Messaged Received From  " << clientSocket.getHost() << ":" << clientSocket.getPortNumber() << "-> " << serializedMsg << std::endl;

                Message msg = Message::deserialize(serializedMsg);

                if (msg.getMessageType() != MessageType::Exit) {

                    _spawnJob(clientSocket);
                }
            }

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
    std::cout << "Terminating the Job!" << std::endl;
    _isRunning = false;
    std::vector<Job*>::iterator it = std::find(_jobs.begin(), _jobs.end(), job);
    if (it != _jobs.end()) {

        (*it) -> join();
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
