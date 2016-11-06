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
    std::cout << "Enter Message: " ;
    std::string msg;
    std::cin >> input;
    if (!sendMsg(input)) {
        perror("Cannot Send Message");
    }
    std::cout << "Message -> " << input << " SENT" << std::endl;
    if (!recvMsg(msg)) {
        perror("Cannot Recieve Message!");
    }
    std::cout << "Message -> " << msg << " RECIEVED" << std::endl;
    _updateServerSocket(atoi(msg.c_str()), "127.0.0.1");
    std::cout << "\nEnter Message: " ;
    while(std::cin >> input) {
        std::string msg;
        if (!sendMsg(input)) {
            perror("Cannot Send Message");
        }
        std::cout << "Message -> " << input << " SENT" << std::endl;
        if (!recvMsg(msg)) {
            perror("Cannot Recieve Message!");
        }
        std::cout << "Message -> " << msg << " RECIEVED" << std::endl;
        if (msg == "Terminated!") {
            break;
        }
        std::cout << "\nEnter Message: " ;
    }
}

void Client::run () {
    _execute();
}
