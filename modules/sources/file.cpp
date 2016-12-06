#include "file.hpp"

int File::_id = 0;

std::string File::getName() const {
    return _name;
}

void File::setName(const std::string &name) {
    _name = name;
}

bool File::isEOF() const {
    return _isEOF;
}


int File::getFd() const {
    return _fd;
}

void File::setFd(int fd) {
    _fd = fd;
}

bool File::lock() {
    //TODO
    std::cout << "LOCKING.................." << std::endl;
    return flock(_fd, LOCK_EX) == 0;
}

bool File::unlock() {
    //TODO
    std::cout << "UNLOCKING.................." << std::endl;
    return flock(_fd, LOCK_UN) == 0;
}

void File::parseDetails(std::string details, std::string data) {

    std::stringstream ss(details);
    std::string token;

    while (ss >> token) {

        if (token == Message::FileModeToken) {

            int mode;
            ss >> mode;
            _mode = (FileMode) mode;

        } else if (token == Message::FileNameToken) {

            ss >> _name;

        } else if (token == Message::DecodedLengthToken) {

            ss >> _decodedLength;

        } else if (token == Message::FileDescriptorToken) {

            ss >> _fd;

        }
    }
}

void File::parse(std::string details, FileDescriptor &fd) {
    std::stringstream ss(details);
    std::string token;
    while (ss >> token) {
        if (token == Message::FileDescriptorToken) {
            ss >> fd;
        }
    }
}

void File::parse(std::string details, FileDescriptor &fd, std::string &name) {
    std::stringstream ss(details);
    std::string token;
    while (ss >> token) {
        if (token == Message::FileDescriptorToken) {
            ss >> fd;
        } else if (token == Message::FileNameToken) {
            ss >> name;
        }
    }
}

void File::parse(std::string details, FileDescriptor &fd, std::string &name, FileMode &mode) {
    std::stringstream ss(details);
    std::string token;
    while (ss >> token) {
        if (token == Message::FileDescriptorToken) {
            ss >> fd;
        } else if (token == Message::FileNameToken) {
            ss >> name;
        } else if (token == Message::FileModeToken) {
            int m;
            ss >> m;
            mode = (FileMode) m;
        }
    }
}

void File::parse(std::string details, FileDescriptor &fd, std::string &name, FileMode &mode, std::string &bytes) {
    std::stringstream ss(details);
    std::string token;
    int decodedLength;
    std::cout << "=========================================================" << std::endl;
    while (ss >> token) {
        if (token == Message::FileDescriptorToken) {
            ss >> fd;
        } else if (token == Message::FileNameToken) {
            ss >> name;
        } else if (token == Message::FileModeToken) {
            int m;
            ss >> m;
            mode = (FileMode) m;
        } else if (token == Message::DecodedLengthToken) {
            ss >> decodedLength;
        } /*else if (token == WriteDataToken) {
            while(ss.peek() == ' ')
                ss.get();
            bytes.resize(decodedLength);
            ss.read(&(bytes[0]), decodedLength);
            std::cout << "Bytes Read: " << decodedLength << " " << bytes << std::endl;
            std::cout << bytes.size() << std::endl;
        }*/
    }
}

void File::parseDetails(std::string details) {
    std::cout << "File: parseDetails()" << std::endl;
    std::stringstream ss(details);
    std::string token;
    while (ss >> token) {
        std::cout << "Token: " << token << std::endl;
        if (token == Message::FileModeToken) {
            int mode;
            ss >> mode;
            _mode = (FileMode) mode;
        } else if (token == Message::FileNameToken) {
            ss >> _name;
        } else if (token == Message::DecodedLengthToken) {
            ss >> _decodedLength;
        } else if (token == Message::FileDescriptorToken) {
            ss >> _fd;
        }
    }
}


int File::getDecodedLength() const {
    return _decodedLength;
}

void File::setDecodedLength(int decodedLength) {
    _decodedLength = decodedLength;
}
File::File(bool local, std::string dir):_isLocal(local), _isEOF(false) {
    _id++;
}


std::string File::getDirectory() const {
    return _dir;
}

void File::setDirectory(const std::string &dir) {
    _dir = dir;
}

File::File () {
    _id++;
    _isLocal = true;
}

FileStatus File::create(std::string name, FileMode mode) {

    _mode = mode;

    std::string path = _dir + '/' + name;

    std::cout << path << std::endl;

    int fd = ::open(path.c_str(), O_RDWR|O_CREAT|O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);

    if (fd < 0) {

        perror("Cannot open output file\n");
        _status = FileStatus::CreateOperationFailed;
        return _status;

    } else {

        _fd = fd;
        _status = FileStatus::CreateOperationSuccess;
        return _status;

    }
}

FileStatus File::rcreate(std::string ownerId, std::string username, std::string name, FileMode mode, UDPSocket server) {
    _isLocal = false;
    _name = name;
//    std::string str = "N: " + _name + " FM: " + std::to_string((int)mode);


    Message message(ownerId, username, "Create!", MessageType::Request, RPC::CreateFile);
    message.addHeader(Message::FileNameToken, _name);
    message.addHeader(Message::FileModeToken, std::to_string((int)mode));

    if (!_sock.sendMessageTo(server, message)) {

        _status = FileStatus::CreateOperationFailed;
        return _status;

    } else {

        std::string serialized;

        if(_sock.recvFrom(server, serialized) == -1) {

            _status = FileStatus::CreateOperationFailed;
            return _status;

        } else {

            Message message = Message::deserialize(serialized);

            if (message.getMessageType() == MessageType::Reply && message.getReplyType() == ReplyType::Success) {
//                parseDetails(message.getMessage());

                setMode((FileMode)std::stoi(message.getHeader(Message::FileModeToken)));
                setName(message.getHeader(Message::FileNameToken));

//                setDecodedLength(std::stoi(message.getHeader(Message::DecodedLengthToken)));
                setFd(std::stoi(message.getHeader(Message::FileDescriptorToken)));

                _status = FileStatus::CreateOperationSuccess;
                return _status;

            } else {

                _status = FileStatus::CreateOperationFailed;
                return _status;

            }
        }

    }
}

FileStatus File::open(std::string name, FileMode mode) {
    _isLocal = true;
    _mode = mode;

    std::string path = _dir + "/" + name;

    std::cout << "path: " << path << std::endl;

    int fd = ::open(path.c_str(), O_RDWR | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd < 0) {
        perror("Cannot Open File.");
        _status = FileStatus::OpenOperationFailed;
        return _status;
    } else {
        _fd = fd;
        _status = FileStatus::Opened;
        return _status;
    }
}

FileStatus File::ropen(std::string ownerId, std::string username, std::string path, FileMode mode,  UDPSocket server) {

    std::cout << "Remote connection to " << _sock.getPortNumber() << std::endl;

    _isLocal = false;
    _name = path;

//    std::string str = _name + " FM: " + std::to_string((int)mode);

    Message message(ownerId, username, "Open!", MessageType::Request, RPC::OpenFile);

    message.addHeader(Message::FileNameToken, _name);
    message.addHeader(Message::FileModeToken, std::to_string((int)mode));

    if (!_sock.sendMessageTo(server, message)) {

        _status = FileStatus::OpenOperationFailed;
        return _status;

    } else {

        std::string serialized;

        if(_sock.recvFrom(server, serialized) == -1) {

            _status = FileStatus::OpenOperationFailed;
            return _status;

        } else {

            Message message = Message::deserialize(serialized);

            if (message.getMessageType() == MessageType::Reply && message.getReplyType() == ReplyType::Success) {

                parseDetails(message.getMessage());
                std::cout << "Opened!" << std::endl;
                _status = FileStatus::Opened;
                return _status;

            } else {

                std::cout << "============================Msg============================" << std::endl;
                std::cout << "Message Type Recieved: " << (int) message.getMessageType() << std::endl;
                std::cout << "Message Reply Type Recieved: " << (int) message.getReplyType() << std::endl;
                std::cout << "Message (Serialized) Recieved: " << serialized << std::endl;
                std::cout << "Message Recieved: " << message.getMessage() << std::endl;
                std::cout << "============================Msg============================" << std::endl;
                std::cout << "Failed!" << std::endl;

                _status = FileStatus::OpenOperationFailed;
                return _status;

            }
        }
    }
}

std::string File::read() {

    if(!lock())  std::cout << "Locking error " << strerror(errno) << std::endl;

    if (_status != FileStatus::Opened) {

        FileStatus status = open(_name, _mode);

        if(status != FileStatus::Opened) {
            perror("Cannot Open the File!");
            return "";
        }

    }

    int bytes, size = 0;
    char buffer[MAX_READ];

    bytes = ::read(_fd, &buffer, MAX_READ);

    if (bytes == -1) {

        perror("Error Reading From File!");

    } else if (bytes == 0) {

        _isEOF = true;

    } else {

        _isEOF = false;
        size = size + bytes;

    }

    std::cout << "Bytes Read: " << bytes << std::endl;
    std::cout << "Size Read: " << size << std::endl;

    if(!unlock())  std::cout << "UnLocking error " << strerror(errno) << std::endl;

    return std::string(buffer, size);
}

std::string File::rread(std::string ownerId, std::string username, UDPSocket server) {

    std::cout << "Remote Read.." << std::endl;

    Message message(ownerId, username, "Read", MessageType::Request, RPC::ReadFile);
    message.addHeader(Message::FileNameToken, _name);
    message.addHeader(Message::FileModeToken, std::to_string((int)FileMode::ReadOnly));
    message.addHeader(Message::FileDescriptorToken, std::to_string(_fd));

    if (!_sock.sendMessageTo(server, message)) {

        _status = FileStatus::ReadOperationFailed;
        return "";

    } else {

        std::cout << "Sending File succeed!" << std::endl;
        std::string serialized;

        if(_sock.recvFrom(server, serialized) == -1) {

            std::cout << "recvFrom Failed!" << std::endl;
            _status = FileStatus::ReadOperationFailed;
            return "";

        } else {

            std::cout << "recvFrom succeed!" << std::endl;
            Message message = Message::deserialize(serialized);
            if (message.getMessageType() == MessageType::Reply && message.getReplyType() == ReplyType::Success) {

                _status = FileStatus::ReadOperationSuccess;

                return message.getMessage();

            } else {

                _status = FileStatus::ReadOperationFailed;
                return "";

            }
        }
    }
}

FileStatus File::write(std::string str) {

    if(!lock())  std::cout << "Locking error " << strerror(errno) << std::endl;

    std::cout << "Directory: " << _dir << std::endl;

    _mode = FileMode::ReadWrite;

    if (_status != FileStatus::Opened) {

        FileStatus status = open(_name, _mode);

        if(status != FileStatus::Opened) {

            perror("Cannot Open the File!");
            return FileStatus::WriteOperationFailed;

        }
    }

    int bytes = ::pwrite(_fd, str.c_str(), str.length(), 0);

    if (bytes == -1) {

        perror("An error has occured while writing to the file.");
        return FileStatus::WriteOperationFailed;

    } else if (bytes == 0) {

        _isEOF = true;

    } else {

        _isEOF = false;

    }

    if(!unlock())  std::cout << "UnLocking error " << strerror(errno) << std::endl;

//    close();

    return FileStatus::WriteOperationSuccess;
}

FileStatus File::write(std::string str, unsigned int length) {

    if(!lock())  std::cout << "Locking error " << strerror(errno) << std::endl;

    _mode = FileMode::ReadWrite;

    std::cout << "Directory: " << _dir << std::endl;

    if (_status != FileStatus::Opened) {

        FileStatus status = open(_name, _mode);
        if(status != FileStatus::Opened) {

            perror("Cannot Open the File!");
            return FileStatus::WriteOperationFailed;

        }
    }

    int bytes = ::pwrite(_fd, str.c_str(), length, 0);

    if (bytes == -1) {

        perror("An error has occured while writing to the file.");
        return FileStatus::WriteOperationFailed;

    } else if (bytes == 0) {

        _isEOF = true;

    }

    if(!unlock())  std::cout << "UnLocking error " << strerror(errno) << std::endl;

    close();

//    _status = FileStatus::Opened;

    return FileStatus::WriteOperationSuccess;
}

FileStatus File::write(std::string name, std::string str, unsigned int length) {

    _name = name;
    return write(str, length);

}

FileStatus File::rwrite(std::string ownerId, std::string username, std::string name, std::string txt, UDPSocket server) {

    _name = name;
    return rwrite(ownerId, username, txt, server);

}

FileStatus File::rwrite(std::string ownerId, std::string username, std::string data, UDPSocket server) {
//    std::string str = "N: " + _name + " FM: " + std::to_string((int)FileMode::ReadOnly) + " L: " +
//                      std::to_string(txt.length()) + " W: " + txt + " FD: " + std::to_string(_fd);
//    std::cout << "rWrite: " << _fd << std::endl;

    Message message(ownerId, username, data, MessageType::Request, RPC::WriteToFile);

    message.addHeader(Message::FileNameToken, _name);
    message.addHeader(Message::FileModeToken, std::to_string((int) FileMode::ReadOnly));
    message.addHeader(Message::DecodedLengthToken, std::to_string(data.length()));
    message.addHeader(Message::FileDescriptorToken, std::to_string(_fd));

    if (!_sock.sendMessageTo(server, message)) {

        _status = FileStatus::WriteOperationFailed;
        return _status;

    } else {

        std::string serialized;

        if(_sock.recvFrom(server, serialized) == -1) {

            _status = FileStatus::WriteOperationFailed;
            return _status;

        } else {

            Message message = Message::deserialize(serialized);

            if (message.getMessageType() == MessageType::Reply && message.getReplyType() == ReplyType::Success) {
                _status = FileStatus::WriteOperationSuccess;
                return _status;

            } else {

                //TODO: RETURN ERROR;
                _status = FileStatus::WriteOperationFailed;
                return _status;

            }
        }
    }
}

FileStatus File::close() {

    int status = ::close(_fd);

    std::cerr << strerror(errno) << std::endl;

    _status = (status == 0) ? FileStatus::Closed : FileStatus::Opened;

    return _status;
}

void File::setClientSocket(UDPSocket sock) {
    _sock = sock;
}

int File::getId() {
    return _id;
}

bool File::isLocal() const {
    return _isLocal;
}

bool File::isOpen() const {
    return _isOpen;
}

FileStatus File::getStatus() const {
    return _status;
}

FileMode File::getMode() const {
    return _mode;
}

void File::setMode(FileMode mode) {
    _mode = mode;
}

File::~File() {

}
