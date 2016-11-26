/**
 * class Message
 * This class is meant to for sending pings to clients as TCP/IP Keep Alive Protocol.
 * Author: Group 3
 * Date: 13th November 2016
 *  Project One - CSCE 4411
 */
#include "heartbeat.hpp"

Heartbeat::Heartbeat(std::string host, int port) : Client(host, port, 0) {
    std::cout << "Heartbeat()" << std::endl;
}

void Heartbeat::run() {
    Message message("1", MessageType::Ping);
    while(true) {
        std::string msg;
        if (!_sendMessage(message)) {
            perror("Cannot Send Message!");
        } else {
            std::cout << "Ping" << std::endl;
            if (!_receiveWithTimeout(msg)) {
                perror("Cannot Recieve Message!");
                std::cerr << "Server Disconnected!" << std::endl;
                break;
            } else {
                std::cout << "Pong!" << std::endl;
            }
        }
    }
    std::cout << "Heartbeat Stopped, Server " << _serverSocket.getHost() << ":" << _serverSocket.getPortNumber() << " is unreachable" << std::endl;
}

Heartbeat::~Heartbeat() {
    std::cout << "~Heartbeat" << std::endl;
}
