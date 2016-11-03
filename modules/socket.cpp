#include "socket.hpp"

Socket::Socket():_sock(-1) {
    memset(&_address, 0, sizeof(_address));
}

Socket::~Socket() {

}

bool Socket::isValid () const {
    return _sock != -1;
}

bool Socket::create() {
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if (!isValid()) {
        return false;
    } else {
        int on = 1;
        return (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof(on)) != -1);
    }

}

bool Socket::bind(const int port) {
    if (!isValid()) {
        return false;
    } else {
        _address.sin_family = AF_INET;
        _address.sin_addr.s_addr = INADDR_ANY;
        _address.sin_port = htons(port);
        int fd = ::bind(_sock, (sockaddr * ) &_address, sizeof(_address));
        socklen_t length = sizeof(_address);
        getsockname(_sock, (sockaddr *)& _address, &length) ;
        return fd != -1;
    }
}


bool Socket::listen() const {
    if (!isValid()) {
        return false;
    } else {
        int val = ::listen(_sock, MAX_CONNECTIONS);
        return val != -1;
    }
}

bool Socket::accept(Socket &socket) const {
    socklen_t length = sizeof(_address);
    socket._sock = ::accept(_sock, ( sockaddr * ) &_address, &length);
    return !(socket._sock <= 0);
}

bool Socket::send(const std::string msg) const {
    int status = ::send(_sock, msg.c_str(), msg.size(), MSG_NOSIGNAL);
    return status != -1;
}

bool Socket::sendTo(Socket sock, std::string msg) {
    std::cout << "SendTo" << std::endl;
    sockaddr_in destination;
    destination.sin_port = htons(port);
    inet_aton(host.c_str(), &destination.sin_addr);
    std::cout << "Address Set" << std::endl;
    int status = ::sendto(sock, msg.c_str(), strlen(msg.c_str()) + 1, 0,(sockaddr *) &destination, sizeof(destination));
    std::cout << "Status: " << status << std::endl;
    return status != -1;
}
// bool Socket::sendTo(std::string host, int port, std::string msg) {
//     std::cout << "SendTo" << std::endl;
//     sockaddr_in destination;
//     destination.sin_port = htons(port);
//     inet_aton(host.c_str(), &destination.sin_addr);
//     std::cout << "Address Set" << std::endl;
//     int status = ::sendto(_sock, msg.c_str(), strlen(msg.c_str()) + 1, 0,(sockaddr *) &destination, sizeof(destination));
//     std::cout << "Status: " << status << std::endl;
//     return status != -1;
// }

int Socket::recv(std::string &msg) const {
    char buffer[MAX_RECV];
    msg = "";
    memset(buffer, 0, MAX_RECV + 1);
    int status = ::recv(_sock, buffer, MAX_RECV, 0);
    if (status == -1) {
        std::cerr << "status = -1 errno = " << errno <<" in Socket::recv" << std::endl;
        return 0;
    } else if (status == 0) {
        return 0;
    } else {
        msg = buffer;
        return status;
    }
}

bool Socket::connect(const std::string host, const int port) {
    if (!isValid()) {
        return false;
    } else {
        _address.sin_family = AF_INET;
        _address.sin_port = htons(port);

        int status = inet_pton( AF_INET, host.c_str(), &_address.sin_addr );
        if (errno == EAFNOSUPPORT) {
            return false;
        } else {
            status = ::connect(_sock, ( sockaddr * ) &_address, sizeof(_address));
            return !status;
        }
    }
}

void Socket::setNonBlocking(const bool block) {
    int opts = fcntl(_sock, F_GETFL);
    if (opts < 0) {
        return;
    } else {
        if (block) {
            opts = (opts | O_NONBLOCK);
        } else {
            opts = (opts & ~O_NONBLOCK);
        }
        fcntl(_sock, F_SETFL, opts);
    }
}

int Socket::getPortNumber() {
    return ntohs(_address.sin_port);
}

std::string Socket::getHost() {
    char host [200];
    inet_ntop(AF_INET, &(_address.sin_addr), host, INET_ADDRSTRLEN);
    return std::string(host);
}
