/**
* class Job
* A class for encuaplsating and abstracting the job which is a worker for the parent thread.
* It inherts from the thread class
* Author: Group 3
* Date: 24th October 2016
*  Project One - CSCE 4411
*/
#include "job.hpp"
std::string toString (int i) {
    std::ostringstream stream;
    stream << i;
    return stream.str();
}

int Job::_id = 0;

Job::Job(UDPSocket sock): _clientSocket(sock), Thread() {
    _id++;
}

Job::~Job() {

}


bool Job::_sendInfo () {
    if (!_serverSocket.sendMessageTo(_clientSocket, Message(toString(_serverSocket.getPortNumber()).c_str (), MessageType::Info))) {
        perror("Cannot Send the New Socket!");
        return false;
    } else {
        std::cout << "Sending: " << std::to_string(_serverSocket.getPortNumber()) << std::endl;
        return true;
    }
}


int Job::getJobId() const {
    return _id;
}

void Job::_openFile(Message message) {
    std::cout << "Opening File ..." << std::endl;
    char details[MAX_RECV];
    //TO-DO: CHANGE TO STRING.
    strcpy(details, message.getMessage().c_str());
    char fileName[100];
    int mode;
    sscanf(details, "%s FM: %d", fileName, &mode);
    std::cout << "File Name recieved: " << fileName << std::endl;
    std::cout << "File Mode recieved: " << mode << std::endl;
    File file;
    ReplyType replyType = file.open(fileName, (FileMode)mode) == FileStatus::Opened ? ReplyType::Success : ReplyType::Failure;
    std::string fd = std::to_string(file.getFd());
    message.setReplyType(replyType);
    message.setMessage(fd.c_str());
}

void Job::_createFile(Message &message) {
    char details[MAX_RECV];
    memset(details, 0, MAX_RECV);
    //TO-DO: CHANGE TO STRING.
    strcpy(details, message.getMessage().c_str());
    std::cout << "Creating File ..." << std::endl;
    char fileName[100];
    int mode;
    sscanf(details, "N: %s FM: %d", fileName, &mode);
    std::cout << "File Name recieved: " << fileName << std::endl;
    std::cout << "File Mode recieved: " << mode << std::endl;
    File file;
    FileStatus status = file.create(fileName, (FileMode)mode);
    std::cout << "File Create Operation status " << (int)status << std::endl;
    if (status == FileStatus::CreateOperationSuccess) {
        message.setReplyType(ReplyType::Success);
        message.setMessage("File created!");
    } else {
        message.setReplyType(ReplyType::Failure);
        message.setMessage("File is not created!");
    }
}

void Job::_readFile(Message &message) {
    char details[MAX_RECV];
    memset(details, 0, MAX_RECV);
    //TO-DO: CHANGE TO STRING.
    strcpy(details, message.getMessage().c_str());
    std::cout << "Reading File ..." << std::endl;
    char fileName[100];
    int mode;
    sscanf(details, "%s FM: %d", fileName, &mode);
    std::cout << "File Name recieved: " << fileName << std::endl;
    std::cout << "File Mode recieved: " << mode << std::endl;
    std::string buffer;
    File file;
    file.setName(fileName);
    file.setMode((FileMode)mode);
    buffer = file.read();
    message.setMessageType(MessageType::Reply);
    message.setMessage(buffer);
    message.setReplyType(ReplyType::Success);
}

void Job::_writeFile(Message &message) {
    std::cout << "------------------------WRITE FILE-------------------------" << std::endl;
    std::cout << "Writing File ..." << std::endl;
    std::cout << "Message Size: " << message.getMessageSize() << std::endl;
    char details[MAX_RECV];
    //TO-DO: CHANGE TO STRING.
    memcpy(details, message.getMessage().c_str(), message.getMessageSize());
    char fileName[100];
    int mode, decodedLength, bytesRead;
    sscanf(details, "N: %s FM: %d L: %d W: %n", fileName, &mode, &decodedLength, &bytesRead);
    File file;
    FileStatus status = file.write(fileName, std::string(details + bytesRead, message.getMessageSize() - (bytesRead)), decodedLength);
    if (status == FileStatus::WriteOperationSuccess) {
        message.setMessage("Text is written to file!");
        message.setReplyType(ReplyType::Success);
    } else {
        message.setMessage("Text is written to file failure!");
        message.setReplyType(ReplyType::Failure);
    }
}

JobState Job::_handleMessage(Message message) {
    std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=MSG-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
    MessageType type;
    if (message.getMessageType() == MessageType::Exit) {
        std::cout << "Exiting From " << _clientSocket.getHost() << ": " << std::to_string(_clientSocket.getPortNumber()) << std::endl;
        type = MessageType::Exit;
    } else if (message.getMessageType() == MessageType::Ping) {
        type = MessageType::Pong;
    } else if (message.getMessageType() == MessageType::Request) {
        type = MessageType::Reply;
        if (message.getRpcId() == RPC::CreateFile) {
            std::cout << "------------------------CREATE FILE-------------------------" << std::endl;
            _createFile(message);
            std::cout << "------------------------CREATE FILE-------------------------" << std::endl;
        } else if (message.getRpcId() == RPC::OpenFile) {
            std::cout << "------------------------OPEN FILE--------------------------" << std::endl;
            _openFile(message);
            std::cout << "------------------------OPEN FILE--------------------------" << std::endl;
        } else if (message.getRpcId() == RPC::ReadFile) {
            std::cout << "------------------------READ FILE--------------------------" << std::endl;
            _readFile(message);
            std::cout << "------------------------READ FILE--------------------------" << std::endl;
        } else if (message.getRpcId() == RPC::WriteToFile) {
            _writeFile(message);
            std::cout << "------------------------WRITE FILE-------------------------" << std::endl;
        } else {
            std::cout << "------------------------NO RPC-------------------------" << std::endl;
            std::cerr << "No RPC ID specified." << std::endl;
            std::cout << "------------------------NO RPC-------------------------" << std::endl;
            message.setReplyType(ReplyType::Failure);
        }
    }
    message.setMessageType(type);
    message.setRpcId(RPC::Undefined);
    if (!_serverSocket.sendMessageTo(_clientSocket, message)) {
        perror("Cannot send the reply.");
    }
    std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=MSG-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
    return (type == MessageType::Exit) ? JobState::Exit : JobState::Running;
}

void Job::_listen() {
    std::cout << "Job listening on " << _serverSocket.getPortNumber() << std::endl;
    std::string serializedMsg;
    while(true) {
        std::cout << "Job listening loop..." << std::endl;
        if (_serverSocket.recvFrom(_clientSocket, serializedMsg) != -1) {
            Message message = Message::deserialize(serializedMsg);
            if (_handleMessage(message) == JobState::Exit) {
                break;
            } else {
                std::cout << "Continue." << std::endl;
            }
        } else {
            perror("Cannot receive new requests!");
        }
    }
    std::cout << "Exiting from thread -> " << getThreadId() << std::endl;
}

void Job::run() {
    _isRunning = true;
    std::cout << "Handling Client " << _clientSocket.getHost() << ": " << _clientSocket.getPortNumber() << " from thread -> " << getThreadId() << std::endl;
    if (!_clientSocket.isValid()) {
        perror("Invalid Client Socket.");
        return;
    } else if (!_serverSocket.create()) {
        perror("Couldn't create a new server socket!");
        return;
    } else if (!_sendInfo()) {
        perror("Cannot send the info!");
        return;
    } else {
        _listen();
    }
}

bool Job::operator == (const Job& job) {
    return job._id == _id;
}
