#ifndef FILE_HPP
#define FILE_HPP

#include "stdfax.h"
#include "udp_socket.hpp"
#include "message.hpp"

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
    int _fd;
    bool _isLocal;
    bool _isOpen;
    bool _isEOF;
    std::string _dir;
    FileStatus _status;
    FileMode _mode;
    UDPSocket _sock;
    std::string _name;

public:
    File (bool local = false, std::string dir = "../../files/");
    FileStatus create(std::string name, FileMode mode = FileMode::Undefined);
    FileStatus rcreate(std::string name, FileMode mode, UDPSocket server);
    FileStatus open(std::string path, FileMode mode);
    FileStatus ropen(std::string path, FileMode mode, UDPSocket server);
    std::string read();
    FileStatus rread(UDPSocket server);
    FileStatus write(std::string str);
    FileStatus write(std::string str, unsigned int length);
    FileStatus write(std::string name, std::string str, unsigned int length);
    FileStatus rwrite(std::string name, std::string txt, UDPSocket server);
    FileStatus rwrite(std::string txt, UDPSocket server);
    FileStatus close();
    FileStatus rclose();

    bool _lock();
    bool _unlock();

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

};
#endif //FILE_HPP
