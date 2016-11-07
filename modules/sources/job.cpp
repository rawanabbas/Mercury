/**
 * class Job
 * A class for encuaplsating and abstracting the job which is a worker for the parent thread.
 * It inherts from the thread class
 * Author: Group 3
 * Date: 24th October 2016
 *  Project One - CSCE 4411
 */
#include "job.hpp"
int Job::_id = 0;
Job::Job(UDPSocket sock): _clientSocket(sock), Thread() {
    _id++;
}

Job::~Job() {

}

int Job::getJobId() const {
    return _id;
}

void Job::run() {
    _isRunning = true;
     std::cout << "I am from thread -> " << getThreadId() << std::endl;
     std::string msg;
    if (!_clientSocket.isValid()) {
        perror("Invalid Client Socket.");
    }
    if (!_serverSocket.create()) {
        perror("Couldn't create a new server socket!");
    }
    if (!_serverSocket.sendTo(_clientSocket, std::to_string(_serverSocket.getPortNumber()))) {
        perror("Cannot Send the New Socket!");
    }
    std::cout << "Sending: " << std::to_string(_serverSocket.getPortNumber()) << std::endl;
    std::locale l;
    while(_isRunning) {
        _serverSocket.recvFrom(_clientSocket, msg);
        std::cout << "Message recieved: " << msg << std::endl;
        if (msg == END_COMMAND) {
            std::cout << "Exiting From " << _clientSocket.getHost() << ": " << std::to_string(_clientSocket.getPortNumber()) << std::endl;
            _serverSocket.sendTo(_clientSocket, "q");
            _isRunning = false;
        } else {
            _serverSocket.sendTo(_clientSocket, msg + " Confirmed!");
        }
    }
}

bool Job::operator == (const Job& job) {
    return job._id == _id;
}
