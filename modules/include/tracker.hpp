#ifndef TRACKER_HPP
#define TRACKER_HPP

#include "thread.hpp"
#include "udp_socket.hpp"
#include "message.hpp"
#include "user.hpp"

typedef std::map<std::string, Peer*> PeerMap;

class Tracker : public Thread {

    static int _id;

    User *_user;

    PeerMap *_peers;
    std::vector<Peer> *_allPeers;

    UDPSocket _clientSocket;
    UDPSocket _serverSocket;
    Message _msg;

    bool _sendInfo();
    void _pulse();
public:

    Tracker(UDPSocket clientSocket, User *user, std::map<std::string, Peer*> *peers, std::vector<Peer> *allPeers);

    void run();
    std::string getUserID();
    inline bool operator == (const Tracker& tracker);

    virtual ~Tracker();

};

#endif //TRACKER_HPP
