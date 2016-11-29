#ifndef FILE_HPP
#define FILE_HPP

#include "stdafx.h"
#include "udp_socket.hpp"
#include "message.hpp"

typedef int FileDescriptor;

enum class FileStatus {
    Locked,
    Opened,
    Closed,
    WriteOperationFailed,
    WriteOperationSuccess,
    ReadOperationFailed,
    ReadOperationSuccess,
    CreateOperationFailed,
    CreateOperationSuccess,
    OpenOperationFailed
};

enum class FileMode {
    WriteOnly = S_IWUSR,
    ReadOnly = S_IRUSR,
    ReadWrite = S_IWUSR | S_IRUSR,
    Undefined = -1
};

class File {
private:
    static int _id;
    static const std::string FileDescriptorToken;
    static const std::string FileNameToken;
    static const std::string FileModeToken;
    static const std::string DecodedLengthToken;
    static const std::string WriteDataToken;

    FileDescriptor _fd;

    bool _isLocal;
    bool _isOpen;
    bool _isEOF;

    UDPSocket _sock;
    std::string _dir;
    FileStatus _status;
    FileMode _mode;
    std::string _name;
    int _decodedLength;

public:

    File (bool local = false, std::string dir = "../../files/");

    FileStatus create(std::string name, FileMode mode = FileMode::Undefined);
    FileStatus rcreate(std::string ownerId, std::string name, FileMode mode, UDPSocket server);

    FileStatus open(std::string path, FileMode mode);
    FileStatus ropen(std::string ownerId, std::string path, FileMode mode, UDPSocket server);

    std::string read();
    FileStatus rread(std::string ownerId, UDPSocket server);

    FileStatus write(std::string str);
    FileStatus write(std::string str, unsigned int length);
    FileStatus write(std::string name, std::string str, unsigned int length);
    FileStatus rwrite(std::string ownerId, std::string name, std::string txt, UDPSocket server);
    FileStatus rwrite(std::string ownerId, std::string txt, UDPSocket server);

    FileStatus close();
    FileStatus rclose();

    void parseDetails(std::string details);
    void parseDetails(std::string details, std::string data);

    static void parse(std::string details, FileDescriptor &fd);
    static void parse(std::string details, FileDescriptor &fd, std::string &name);
    static void parse(std::string details, FileDescriptor &fd, std::string &name, FileMode &mode);
    static void parse(std::string details, FileDescriptor &fd, std::string &name, FileMode &mode, std::string &bytes);

    bool lock();
    bool unlock();

    void setClientSocket(UDPSocket sock);

    static int getId();
    static void setId(int id);

    bool isLocal() const;
    bool isOpen() const;
    bool isEOF() const;

    FileStatus getStatus() const;

    FileMode getMode() const;
    void setMode(FileMode mode);

    std::string getName() const;
    void setName(const std::string &name);

    int getFd() const;
    void setFd(int fd);

    virtual ~File ();

    int getDecodedLength() const;
    void setDecodedLength(int decodedLength);
};
#endif //FILE_HPP
