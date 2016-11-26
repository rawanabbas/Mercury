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
    if (!_sock.isValid()) {
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

bool Client::_sendMessage(Message msg) {
    // TODO
    return _sock.sendMessageTo(_serverSocket, msg);
}

bool Client::_send(std::string msg) {
    return _sock.sendTo(_serverSocket, msg);
}

bool Client::_receive(std::string &msg) {
    return _sock.recvFrom(_serverSocket, msg);
}

bool Client::_receiveWithTimeout(std::string &msg) {
    return _sock.recvWithTimeout(_serverSocket, msg);
}

void Client::_updateServerSocket(int port, std::string host) {
    _serverSocket.updateSocketInfo(port, host);
}

void Client::_exit(std::string msg)
{
    MessageType type = MessageType::Exit;
    Message message(strdup(msg.c_str()), type);
    if (!_sendMessage(message)) {
        perror("Cannot Send Message.");
    } else {
        std::cout << "Exit Message: -> " << msg << " SENT" << std::endl;
        std::cout << "Now recieving confirmation..." << std::endl;
        if (_receive(msg) != -1) {
            Message message = Message::deserialize(msg);
            if (message.getMessageType() == MessageType::Exit) {
                std::cout << "Server Port has exited, will now shutdown!" << std::endl;
            } else {
                std::cout << "Unkown server response, will now exit anyways" << std::endl;
                return;
            }
        }
    }
}

void Client::_ping(std::string msg)
{
    MessageType type = MessageType::Ping;
    Message message(strdup(msg.c_str()), type);
    if (!_sendMessage(message)) {
        perror("Cannot Send Message.");
    } else {
        std::cout << "Message: -> " << msg << " SENT" << std::endl;
        std::cout << "Now recieving..." << std::endl;
        if (_receive(msg) != -1) {
            Message message = Message::deserialize(msg);
            if (message.getMessageType() == MessageType::Info) {
                std::cout << "Updated Server Socket Info: " << message.getMessage() << std::endl;
                _updateServerSocket(atoi(message.getMessage()), _serverSocket.getHost());
            } else if (message.getMessageType() == MessageType::Pong) {
                std::cout << "Pong Message -> " << msg << " RECIEVED" << std::endl;
            } else {
                std::cout << "Unkown Message Type has been recieved:" << msg <<" !" << std::endl;
            }
        }
    }
}

void Client::_openFile()
{
    std::string fileName;
    std::cout << "Enter File Name: ";
    std::cin >> fileName;
    std::cout << std::endl;
    File file;
    file.setClientSocket(_sock);
    FileStatus openStatus = file.ropen(fileName, FileMode::ReadOnly, _serverSocket);
    std::cout << "File Remote Open Status " << (int)openStatus << std::endl;
    if (openStatus == FileStatus::Opened) {
        std::cout << "File is open!" << std::endl;
//        std::string str = file.read();
//        FileStatus status = file.rread(_serverSocket);
//        std::cout << str << std::endl;
//        std::cout << (int)status << std::endl;
//        std::cout << "------------------====================------------------" << std::endl;
    } else {
        std::cout << "Cannt Open Remote File!" << std::endl;
    }
}

bool Client::_createFile() {
    File remoteFile;
    std::string fileName;
    std::cout <<"Enter File Name: ";
    std::cin >> fileName;
    FileStatus status = remoteFile.rcreate(fileName, FileMode::ReadWrite, _serverSocket);
    std::cout << "Create Operation Status " << (int) status << std::endl;
    if (status == FileStatus::CreateOperationSuccess) {
        std::cout << "File " << fileName << " is created!" << std::endl;
        return true;
    } else {
        std::cout << "File creation failed!" << std::endl;
        return false;
    }
}

bool Client::_createFile(File remoteFile) {
    std::string fileName;
    std::cout <<"Enter File Name: ";
    std::cin >> fileName;
    FileStatus status = remoteFile.rcreate(fileName, FileMode::ReadWrite, _serverSocket);
    std::cout << "Create Operation Status " << (int) status << std::endl;
    if (status == FileStatus::CreateOperationSuccess) {
        std::cout << "File " << fileName << " is created!" << std::endl;
        return true;
    } else {
        std::cout << "File creation failed!" << std::endl;
        return false;
    }
}

bool Client::_createFile(File remoteFile, std::string fileName) {
    FileStatus status = remoteFile.rcreate(fileName, FileMode::ReadWrite, _serverSocket);
    std::cout << "Create Operation Status " << (int) status << std::endl;
    if (status == FileStatus::CreateOperationSuccess) {
        std::cout << "File " << fileName << " is created!" << std::endl;
        return true;
    } else {
        std::cout << "File creation failed!" << std::endl;
        return false;
    }
}

void Client::_execute() {
    while(true) {
        std::cout << "Enter Message: " ;
        std::string msg;
        std::cin >> msg;
        if (msg == std::string(1, (char)Commands::Exit)) {
            std::cout << "---------------------------EXIT----------------------------" << std::endl;
            _exit(msg);
            std::cout << "---------------------------EXIT----------------------------" << std::endl;
            break;
        } else if (msg == std::string(1, (char)Commands::Ping)) {
            std::cout << "---------------------------PING----------------------------" << std::endl;
            _ping(msg);
            std::cout << "---------------------------PING----------------------------" << std::endl;
        } else if (msg == std::string(1, (char)Commands::File)) {
            std::cout << "---------------------------FILE----------------------------" << std::endl;
            std::cout << "What do you want to do with the file? (s) send file (r) remote read file (o) open remote file (w) write to remote file." << std::endl;
            std::cout << "Command: ";
            std::cin >> msg;
            if (msg == std::string(1, (char)Commands::ReadFile)) {
                std::string fileName;
                std::cout << "Enter File Name: ";
                std::cin >> fileName;
                File file;
                file.setName(fileName);
                FileStatus status = file.rread(_serverSocket);
                if (status == FileStatus::ReadOperationSuccess) {
                    std::cout << "File is read!" << std::endl;
                } else {
                    std::cout << "Read Operation Status: " << (int) status << std::endl;
                }
            } else if (msg == std::string(1, (char)Commands::OpenFile)) {
                _openFile();
            } else if (msg == std::string(1, (char)Commands::CreateFile)){
                _createFile();

            } else if (msg == std::string(1, (char)Commands::WriteFile)) {
                std::string fileName, txt;
                std::cout << "Enter File Name: ";
                std::cin >> fileName;
                std::cout << "Enter some text to write: ";
                std::cin >> txt;
//                getline(std::cin, txt, '\n');
                std::cout << "You entered " << txt << std::endl;
                File file;
                FileStatus status = file.rwrite(fileName, txt, _serverSocket);
                if (status == FileStatus::WriteOperationSuccess) {
                    std::cout << "Write To File Success!" << std::endl;
                } else {
                    std::cout << "Write To File Failure!" << std::endl;
                }
            } else if (msg == std::string(1, (char)Commands::SendFile)) {
                std::string fileName, buffer;
                std::cout << "Enter File Name: ";
                std::cin >> fileName;
                File remoteFile;
                if(_createFile(remoteFile, "server/" + fileName)) {
                    File file;
                    file.open(fileName, FileMode::ReadOnly);
                    buffer = file.read();
                    FileStatus status = remoteFile.rwrite("server/" + fileName, buffer, _serverSocket);
                    if (status == FileStatus::WriteOperationSuccess) {
                        std::cout << "File is sent!" << std::endl;
                    } else {
                        std::cout << "Couldn't send file!" << std::endl;
                    }
                } else {
                    std::cout << "Couldn't send file!" << std::endl;
                }
            }
            std::cout << "---------------------------FILE----------------------------" << std::endl;
        }
    }
    std::cout << "Bye!" << std::endl;
}

void Client::run () {
    _execute();
}
