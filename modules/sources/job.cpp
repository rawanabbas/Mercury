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

Job::Job(std::string ownerId, std::string username, UDPSocket sock, FilesMap *files, pthread_mutex_t filesMutex)
    : _clientSocket(sock), _ownerId(ownerId), _username(username), _fileRecipients(files), _filesMutex(filesMutex), Thread() {

    setMutex(_filesMutex);

    _id++;
}

Job::~Job() {

}


bool Job::_sendInfo () {
    if (!_serverSocket.sendMessageTo(_clientSocket, Message(_ownerId, _username, std::to_string(_serverSocket.getPortNumber()), MessageType::Info))) {
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
    File *file = new File;

    file->setFd(std::stoi(message.getHeader(Message::FileDescriptorToken)));
    file->setMode((FileMode) std::stoi(message.getHeader(Message::FileModeToken)));
    file->setName(message.getHeader(Message::FileNameToken));

    file->setDirectory(message.getHeader(Message::UsernameToken));

    ReplyType replyType = file->open(file->getName(), file->getMode()) == FileStatus::Opened ? ReplyType::Success : ReplyType::Failure;

    message.setReplyType(replyType);
    message.addHeader(Message::FileDescriptorToken, std::to_string((int)file->getFd()));

    _files.insert(std::pair<FileDescriptor, File*>(file->getFd(), file));
}

void Job::_createFile(Message &message) {

    std::cout << "Serialized Message: " << message.serialize() << std::endl;
    File *file = new File;

    file->setMode((FileMode) std::stoi(message.getHeader(Message::FileModeToken)));
    file->setName(message.getHeader(Message::FileNameToken));

    mkdir(message.getHeader(Message::UsernameToken).c_str(), S_IRWXO | S_IRWXU | S_IRWXG);

    file->setDirectory(message.getHeader(Message::UsernameToken));

    FileStatus status = file->create(file->getName(), file->getMode());

    std::cout << "File Create Operation status " << (int)(status) << std::endl;

    if (status == FileStatus::CreateOperationSuccess) {

        std::cout << "File Created!" << std::endl;

        message.setReplyType(ReplyType::Success);
        message.setMessage("FD: " + std::to_string((int)file->getFd()));
        message.addHeader(Message::FileDescriptorToken, std::to_string(file->getFd()));

        std::cout << "FD: " << file->getFd() << std::endl;

        _files[file->getFd()] = file;

    } else {

        std::cout << "File Not Created!" << std::endl;
        message.setReplyType(ReplyType::Failure);
        message.setMessage("File is not created!");

    }
}

void Job::_readFile(Message &message) {

    std::string buffer;
    FileDescriptor fd;

    fd = std::stoi(message.getHeader(Message::FileDescriptorToken));

    File *file = _files[fd];

    file->setDirectory(message.getHeader(Message::UsernameToken));
    buffer = file->read();

    message.setMessageType(MessageType::Reply);
    message.setMessage(buffer);
    message.setReplyType(ReplyType::Success);
}

void Job::_writeFile(Message &message) {

    std::cout << "------------------------WRITE FILE-------------------------" << std::endl;
    std::cout << "Writing File ..." << std::endl;
    std::cout << "Message Size: " << message.getMessageSize() << std::endl;

    FileDescriptor fd = std::stoi(message.getHeader(Message::FileDescriptorToken));
    FileMode mode = (FileMode) std::stoi(message.getHeader(Message::FileModeToken));
    std::string fileName = message.getHeader(Message::FileNameToken);
    std::string bytes = message.getMessage();

    //    File::parse(message.getMessage(), fd, fileName, mode, bytes);

    std::cout << "Write FD: " << fd << std::endl;

    File *file = _files[fd];

    if  (file != NULL) {

        file->setDirectory(message.getHeader(Message::UsernameToken));

        FileStatus status = file->write(message.getHeader(Message::FileNameToken), bytes, bytes.size());

        if (status == FileStatus::WriteOperationSuccess) {

            message.setMessage("Text is written to file!");
            message.setReplyType(ReplyType::Success);
            message.editHeader(Message::OwnerIdToken, _ownerId);

        } else {

            message.setMessage("Text is written to file failure!");
            message.setReplyType(ReplyType::Failure);
            message.editHeader(Message::OwnerIdToken, _ownerId);

        }
    } else {

        std::cout << "No Such File!" << std::endl;
        message.setMessage("Text is written to file failure!");
        message.setReplyType(ReplyType::Failure);

    }
}

bool Job::_createRemoteFile(File *remoteFile, std::string fileName) {

    FileStatus status = remoteFile->rcreate(_ownerId, _username, fileName, FileMode::ReadWrite, _clientSocket);

    std::cout << "Create Operation Status " << (int) status << std::endl;

    if (status == FileStatus::CreateOperationSuccess) {

        std::cout << "File " << fileName << " Remote Fd: " <<  remoteFile->getFd() << " is created!" << std::endl;
        return true;

    } else {

        std::cout << "File creation failed!" << std::endl;
        return false;

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

        } else if (message.getRpcId() == RPC::CloseFile) {

            std::cout << "------------------------CLOSE FILE-------------------------" << std::endl;

            File *file = _files[std::stoi(message.getHeader(Message::FileDescriptorToken))];

            FileStatus status = file->close();

            if (status == FileStatus::Closed) {

                std::cout << "File is closed!" << std::endl;
                message.setReplyType(ReplyType::Success);

            } else {

                std::cout << "Error Closing the File!" << std::endl;
                message.setReplyType(ReplyType::Failure);

            }

            std::cout << "------------------------CLOSE FILE-------------------------" << std::endl;

        } else {

            std::cout << "------------------------NO RPC-------------------------" << std::endl;
            std::cerr << "No RPC ID specified." << std::endl;
            std::cout << "------------------------NO RPC-------------------------" << std::endl;

            message.setReplyType(ReplyType::Failure);

        }
    } else if (message.getMessageType() == MessageType::QueryAll) {
        std::cout << "------------------------QUERY-------------------------" << std::endl;

        std::string recpients = message.getHeader(Message::UsernameToken);

        std::vector <File *> files = (*_fileRecipients)[recpients];

        std::string filenames = "";

        for (int i = 0; i < files.size(); ++i) {

            filenames += files[i]->getName() + "\n";

        }

        message.setMessage(filenames);
        message.setOwnerId(_ownerId);
        message.addHeader(Message::UsernameToken, _username);

        std::cout << "------------------------QUERY-------------------------" << std::endl;
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

            std::cout << serializedMsg << std::endl;

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
