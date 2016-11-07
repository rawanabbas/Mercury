/**
 * class Client
 * Description: A class for encuaplsating and abstracting the client object
 * Author: Group 3
 * Date: 24th October 2016
 *  Project One - CSCE 4411
 */
#include "client.hpp"

int Client::_id = 0;

Client::Client(std::string host, int serverPort, int port) : Thread() {
    if (!_sock.create(port)) {
        perror("Cannot create a client socket.");
    }
    if(!_sock.connect(_serverSocket, host, serverPort)) {
        perror("Cannot connect to the server.");
    }
    _id++;
}

Client::~Client() {
    std::cout << "Destructing the client!" << std::endl;
}

bool Client::sendMsg(std::string msg) {
    return _sock.sendTo(_serverSocket, msg);
}

bool Client::recvMsg(std::string &msg) {
    return _sock.recvFrom(_serverSocket, msg);
}


void Client::_updateServerSocket(int port, std::string host) {
    _serverSocket.updateSocketInfo(port, host);
}

void Client::_execute() {
    std::string input;
    std::string filePath;
    // std::cout << "Enter Input File name: " ;
    std::cout << "Enter Message: " ;
    // std::cin >> filePath;
    std::string msg;
    std::cin >> msg;
    // _inputFile.open(filePath.c_str());
    // if (_inputFile.fail())
    // {
    	// std::cout << "Could not open file.\n";
	// return;
    // }
    // std::getline (_inputFile, input, '#');
    // if (!sendMsg(input)) {
    //     perror("Cannot Send Message");
    // }
    if (!sendMsg(msg)) {
        perror("Cannot Send Message");
    }
    std::cout << "Message -> " << msg << " SENT" << std::endl;
    if (recvMsg(msg) != -1) {
        std::cout << "Message -> " << msg << " RECIEVED" << std::endl;
        _updateServerSocket(atoi(msg.c_str()), "127.0.0.1");
        std::cout << "\nEnter Message: " ;
        while(std::cin >> msg) {
            if (!sendMsg(msg)) {
                perror("Cannot Send Message");
            }
            std::cout << "Message -> " << msg << " SENT" << std::endl;
            if (recvMsg(msg) != -1) {
                std::cout << "Message -> " << msg << " RECIEVED" << std::endl;
                if (msg == "q") {
                    std::cout << "TERMINATING.." << std::endl;
                    break;
                }
            }
            std::cout << "\nEnter Message: " ;
        }
        std::cout << "After loop." << std::endl;
    }
}

void Client::run () {
    _execute();
}
