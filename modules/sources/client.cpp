/**
 * class Client
 * Description: A class for encuaplsating and abstracting the client object
 * Author: Group 3
 * Date: 24th October 2016
 *  Project One - CSCE 4411
 */
#include "client.hpp"

int Client::_id = 0;

Client::Client(std::string ownerId, std::string host, int serverPort) : Thread(), _ownerId(ownerId) {

    if (!_sock.isValid()) {

        perror("Cannot create a client socket.");
        return;

    }

    if(!_sock.connect(_serverSocket, host, serverPort)) {

        perror("Cannot connect to the server.");
        return;

    }

    if (pthread_mutex_init(&_commandMutex, NULL)) {
            perror("Cannot initialize mutex.");
    }

    setMutex(_commandMutex);

    _id++;
}

Client::Client(std::string hostname, int serverPort) : Thread(){

    if (!_sock.isValid()) {

        perror("Cannot create a client socket.");

    }

    if(!_sock.connect(_serverSocket, hostname, serverPort)) {

        perror("Cannot connect to the server.");

    }

    _id++;
}

Client::~Client() {

    pthread_mutex_destroy(&_commandMutex);

}

void Client::setCommand(std::string command) {
    _lock();
    _command.erase();
    _command = command;
    _unlock();
}

bool Client::isAuthenticated() {

    return _isAuthenticated;

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

int Client::_receiveWithTimeout(std::string &msg, int timeout) {
    return _sock.recvWithTimeout(_serverSocket, msg, timeout);
}

void Client::_updateServerSocket(int port, std::string host) {
    _serverSocket.updateSocketInfo(port, host);
}

void Client::_exit(std::string msg) {
    MessageType type = MessageType::Exit;
    Message message(_ownerId, strdup(msg.c_str()), type);

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

void Client::_ping(std::string serializedMsg) {
    MessageType type = MessageType::Ping;
    Message message(_ownerId, serializedMsg, type);

    if (!_sendMessage(message)) {

        perror("Cannot Send Message.");

    } else {

        std::cout << "Now recieving..." << std::endl;

        if (_receive(serializedMsg) != -1) {

            Message message = Message::deserialize(serializedMsg);

            if (message.getMessageType() == MessageType::Info) {

                std::cout << "Updated Server Socket Info: " << message.getMessage() << std::endl;
                _updateServerSocket(atoi(message.getMessage().c_str()), _serverSocket.getHost());

            } else if (message.getMessageType() == MessageType::Pong) {

                std::cout << "Pong Message -> " << message.getMessage() << " RECIEVED" << std::endl;

            } else {

                std::cout << "Unkown Message Type has been recieved:" << serializedMsg <<" !" << std::endl;

            }
        }
    }
}

int Client::_lock() {
    return lock();
}

int Client::_unlock() {
    return release();
}

void Client::_clearCommand() {
    _lock();
    _command = "";
    _unlock();
}

void Client::_openFile() {
    std::string fileName;
    std::cout << "Enter File Name: ";
    std::cin >> fileName;
    std::cout << std::endl;
    File *file;
    file->setClientSocket(_sock);
    FileStatus status = file->ropen(_ownerId, fileName, FileMode::ReadOnly, _serverSocket);
    std::cout << "File Remote Open Status " << (int)status << std::endl;
    if (status == FileStatus::Opened) {
        std::cout << "File is open!" << std::endl;
        _files.insert(std::pair<FileDescriptor, File*>(file->getFd(), file));
    } else {
        std::cout << "Cannt Open Remote File!" << std::endl;
    }
}

bool Client::_createFile() {
    File remoteFile;
    std::string fileName;
    std::cout <<"Enter File Name: ";
    std::cin >> fileName;
    FileStatus status = remoteFile.rcreate(_ownerId, fileName, FileMode::ReadWrite, _serverSocket);
    std::cout << "Create Operation Status " << (int) status << std::endl;
    if (status == FileStatus::CreateOperationSuccess) {
        std::cout << "File " << fileName << " is created!" << std::endl;
        return true;
    } else {
        std::cout << "File creation failed!" << std::endl;
        return false;
    }
}

bool Client::_createFile(File *remoteFile) {

    std::string fileName;
    std::cout <<"Enter File Name: ";
    std::cin >> fileName;

    FileStatus status = remoteFile->rcreate(_ownerId, fileName, FileMode::ReadWrite, _serverSocket);

    std::cout << "Create Operation Status " << (int) status << std::endl;

    if (status == FileStatus::CreateOperationSuccess) {

        std::cout << "File " << fileName << " Remote Fd: "  << remoteFile->getFd() << " is created!" << std::endl;
        return true;

    } else {

        std::cout << "File creation failed!" << std::endl;
        return false;

    }
}

bool Client::_createFile(File *remoteFile, std::string fileName) {

    FileStatus status = remoteFile->rcreate(_ownerId, fileName, FileMode::ReadWrite, _serverSocket);

    std::cout << "Create Operation Status " << (int) status << std::endl;

    if (status == FileStatus::CreateOperationSuccess) {

        std::cout << "File " << fileName << " Remote Fd: " <<  remoteFile->getFd() << " is created!" << std::endl;
        return true;

    } else {

        std::cout << "File creation failed!" << std::endl;
        return false;

    }
}

void Client::_readFile() {

    std::string fileName;

    std::cout << "Enter File Name: ";
    std::cin >> fileName;

    File file;
    file.setName(fileName);

    FileStatus status = file.rread(_ownerId, _serverSocket);

    if (status == FileStatus::ReadOperationSuccess) {

        std::cout << "File is read!" << std::endl;

    } else {

        std::cout << "Read Operation Status: " << (int) status << std::endl;

    }
}

void Client::_sendFile() {

    std::string fileName, buffer;

    std::cout << "Enter File Name: ";

    std::cin >> fileName;

    File *remoteFile = new File;

    if(_createFile(remoteFile, "server/" + fileName)) {

        std::cout << "Mapping File to FD " << remoteFile->getFd() << std::endl;

        _files[remoteFile->getFd()] = remoteFile;
        File file;
        FileStatus status = file.open(fileName, FileMode::ReadOnly);

        std::cout << (int) status << std::endl;

        while (!file.isEOF()) {

            buffer = file.read();

            std::cout << "Buffer Size: " << buffer.length() << std::endl;

            status = remoteFile->rwrite(_ownerId, "server/" + fileName, buffer, _serverSocket);

            if (status != FileStatus::WriteOperationSuccess) {

                std::cout << "Couldn't send file!" << std::endl;
                break;

            }
        }

        if (status == FileStatus::WriteOperationSuccess) {

            std::cout << "File is sent!" << std::endl;

        }

    } else {

        std::cout << "Couldn't send file!" << std::endl;

    }
}

void Client::_writeFile() {

    std::string fileName, txt;

    std::cout << "Enter File Name: ";
    std::cin >> fileName;

    std::cout << "Enter some text to write: ";
    std::cin >> txt;

    File file;
    FileStatus status = file.rwrite(fileName, txt, _serverSocket);

    if (status == FileStatus::WriteOperationSuccess) {

        std::cout << "Write To File Success!" << std::endl;

    } else {

        std::cout << "Write To File Failure!" << std::endl;

    }
}

void Client::_handleFile(std::string msg) {

    std::cout << "What do you want to do with the file? (s) send file (r) remote read file (o) open remote file (w) write to remote file." << std::endl;
    std::cout << "Command: ";
    std::cin >> msg;

    if (msg == std::string(1, (char)Commands::ReadFile)) {

        _readFile();

    } else if (msg == std::string(1, (char)Commands::OpenFile)) {

        _openFile();

    } else if (msg == std::string(1, (char)Commands::CreateFile)){

        _createFile();

    } else if (msg == std::string(1, (char)Commands::WriteFile)) {

        _writeFile();

    } else if (msg == std::string(1, (char)Commands::SendFile)) {

        _sendFile();

    }
}
std::string Client::getOwnerId() const {

    return _ownerId;

}

void Client::setOwnerId(const std::string &ownerId) {

    _ownerId = ownerId;

}


void Client::_register() {
    _authStatus = ClientAuthenticationStatus::Authenticating;
    std::string username, password;

    std::cout << "Enter Username: ";
    std::cin >> username;

    std::cout << "Enter Password: ";
    std::cin >> password;

    Message registerationMsg;
    registerationMsg.setMessageType(MessageType::Register);
    registerationMsg.setMessage("username: " + username + " password: " + password);

    std::cout << "Registeration Message Formulated!" << std::endl;

    if (!_sendMessage(registerationMsg)) {

        std::cout << "Unauthorized!" << std::endl;

    } else {

        std::string auth;

        if(_receive(auth) != -1) {

            Message authMessage = Message::deserialize(auth);

            if(authMessage.getMessageType() == MessageType::Authenticated) {

                _authStatus = ClientAuthenticationStatus::Authenticated;
                _isAuthenticated = true;
                _ownerId = authMessage.getMessage();

                std::cout << "Authenticated!" << std::endl;

            } else {
                _authStatus = ClientAuthenticationStatus::Unauthenticated;
                _isAuthenticated = false;
                std::cout << "Unauthorized!" << std::endl;

            }
        } else {
            perror("Cannot recieve reply!");
            _authStatus = ClientAuthenticationStatus::Unauthenticated;
            _isAuthenticated = false;
            std::cout << "Unauthorized!" << std::endl;
        }
    }
}

void Client::_authenticate() {

    _authStatus = ClientAuthenticationStatus::Authenticating;

    std::string username, password;

    std::cout << "Enter Username: ";
    std::cin >> username;

    std::cout << "Enter Password: ";
    std::cin >> password;

    Message signInMsg;
    signInMsg.setMessageType(MessageType::Authenticate);
    signInMsg.setMessage("username: " + username + " password: " + password);

    if (!_sendMessage(signInMsg)) {

        std::cout << "Unauthorized!" << std::endl;

    } else {

        std::string auth;

        if(_receive(auth) != -1) {

            Message authMessage = Message::deserialize(auth);

            if(authMessage.getMessageType() == MessageType::Authenticated) {

                _authStatus = ClientAuthenticationStatus::Authenticated;
                _isAuthenticated = true;
                _ownerId = authMessage.getMessage();

                std::cout << "Authenticated!" << '\n';
                std::cout << "Your ID: " << _ownerId;

            } else {

                _authStatus = ClientAuthenticationStatus::Unauthenticated;

            }
        }
    }
}

void Client::_execute() {

    while(true) {

//        std::cout << "Enter Message: " ;

        if (_command == std::string(1, (char)Commands::Exit)) {

            std::cout << "---------------------------EXIT----------------------------" << std::endl;
            _exit(_command);
            std::cout << "---------------------------EXIT----------------------------" << std::endl;
            break;

        } else if (_command == std::string(1, (char)Commands::Ping)) {

            std::cout << "---------------------------PING----------------------------" << std::endl;
            _ping(_command);
            std::cout << "---------------------------PING----------------------------" << std::endl;

        } else if (_command == std::string(1, (char)Commands::File)) {

            std::cout << "---------------------------FILE----------------------------" << std::endl;
            _handleFile(_command);
            std::cout << "---------------------------FILE----------------------------" << std::endl;

        } else if (_command == std::string(1, (char)Commands::Register)) {

            std::cout << "---------------------------REGISTER----------------------------" << std::endl;
            _register();
            std::cout << "---------------------------REGISTER----------------------------" << std::endl;

        } else if (_command == std::string(1, (char)Commands::SignIn)) {

            std::cout << "---------------------------SIGN-IN----------------------------" << std::endl;
            _authenticate();
            std::cout << "---------------------------SIGN-IN----------------------------" << std::endl;

        }
        _clearCommand();
    }
    std::cout << "Bye!" << std::endl;
}

void Client::run () {
    _execute();
}

ClientAuthenticationStatus Client::getAuthStatus() {

    return _authStatus;

}
