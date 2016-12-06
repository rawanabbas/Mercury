/**
* class UDPSocket
* Description: A class for encuaplsating and abstracting the socket object
* Author: Group 3
* Date: 24th October 2016
*  Project One - CSCE 4411
*/
#include "udp_socket.hpp"

UDPSocket::UDPSocket(int port) {
    if ((_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("An error has occured while creating a socket.");
    } else {
        _address.sin_family = AF_INET;
        _address.sin_port = htons(port);
        _address.sin_addr.s_addr = INADDR_ANY;
    }
}

bool UDPSocket::create(int port) {
    if (isValid()) {
        _address.sin_port = htons(port);
        if (::bind(_sock, (sockaddr *) &_address, sizeof(_address)) == -1) {
            perror("An error has occured while binding the port.");
            return false;
        }
        _addressLength = sizeof(_address);
        getsockname(_sock, (sockaddr *) &_address, &_addressLength);
        return true;
    } else {
        return false;
    }
}


bool UDPSocket::sendTo(UDPSocket &sock, std::string msg) {

    socklen_t length = sizeof(sockaddr_in);

    if (::sendto(_sock, msg.c_str(), msg.length() + 1, 0, (sockaddr *) &sock._address, length) == -1) {

        perror("Cannot send to the server....");
        std::cerr << "Error: " << errno << std::endl;
        return false;

    } else {

        return true;

    }
}

bool UDPSocket::sendMessageTo(UDPSocket &sock, Message message) {
    return sendTo(sock, message.serialize());
}

int UDPSocket::recvFrom(UDPSocket &sock, std::string &msg) {

    socklen_t s = sizeof(sock._address);

    if ((_bytes = ::recvfrom(_sock, _buffer, MAX_RECV, 0, (sockaddr *) &(sock._address),  &s)) <= 0) {

        perror("Cannot recieve.");

    }

    sock._host = std::string(inet_ntoa(sock._address.sin_addr));


    if (_bytes > MAX_RECV) {

        perror("Buffer Overflow!");
        return -1;

    } else {

        _buffer[_bytes] = '\0';
        msg = std::string(_buffer);
        return _bytes;

    }

}

int UDPSocket::recvWithTimeout(UDPSocket& sock, std::string &msg, int timeout) {

    socklen_t s = sizeof(sock._address);
    memset(_buffer, 0, MAX_RECV);

    struct timeval timeOut;
    timeOut.tv_sec = timeout;
    timeOut.tv_usec = 0;

    FD_ZERO(&_fd);
    FD_SET(_sock, &_fd);

    if (select (_sock + 1, &_fd, NULL, NULL, &timeOut) < 0) {

        perror("Cannot Set Timeout!");

    }

    if (FD_ISSET(_sock, &_fd)) {

        if ((_bytes = ::recvfrom(_sock, _buffer, MAX_RECV, 0, (sockaddr *) &(sock._address),  &s)) <= 0) {

            perror("Cannot recieve.");

        }

        sock._host = std::string(inet_ntoa(sock._address.sin_addr));


        if (_bytes > MAX_RECV) {

            perror("Buffer Overflow!");
            return -1;

        } else {

            _buffer[_bytes] = '\0';
            msg = std::string(_buffer);
            return _bytes;

        }
    } else {

        std::cout << "Timed-out!" << std::endl;
        return -1;

    }
}

bool UDPSocket::connect(UDPSocket &sock, std::string host, int port) {
    sock._address.sin_family = AF_INET;
    sock._address.sin_addr.s_addr = inet_addr(host.c_str());
    sock._address.sin_port = htons(port);
    bzero(&(sock._address.sin_zero), 8);
    return true;
}


int UDPSocket::getPortNumber() {
    return ntohs(_address.sin_port);
}

std::string UDPSocket::getHost() {
    char host [200];
    inet_ntop(AF_INET, &(_address.sin_addr), host, INET_ADDRSTRLEN);

    return _host;
}

bool UDPSocket::isValid() {
    return _sock != -1;
}

void UDPSocket::updateSocketInfo(int port, std::string host) {
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = inet_addr(host.c_str());
    _address.sin_port = htons(port);
    int status = inet_pton(AF_INET, host.c_str(), &_address.sin_addr);
    if (status == 0) {
        std::cout << "Invalid host conversion: " << host.c_str() << std::endl;
    }
    bzero(&(_address.sin_zero), 8);
}

UDPSocket::~UDPSocket() {

}
