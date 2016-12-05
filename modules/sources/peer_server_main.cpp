#include "stdafx.h"
#include "peer_server.hpp"

int main(int argc, char const *argv[]) {

    std::cout << "Peer server listening on 3010" << std::endl;

    try {

        PeerServer peerServer(3010);
        peerServer.start();
        std::cout << "**" << std::endl;
        peerServer.join();
        std::cout << "--" << std::endl;

    } catch (const char * e) {
        std::cout << "Error: " << e << '\n';
    } catch (char const * e) {
        std::cout << "Error: " << e << '\n';
    }

    return 0;
}
