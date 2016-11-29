#include "stdfax.h"
#include "peer_server.hpp"

int main(int argc, char const *argv[]) {


    try {

        PeerServer peerServer(3010);
        peerServer.start();
        peerServer.join();

    } catch (const char * e) {
        std::cout << "Error: " << e << '\n';
    } catch (char const * e) {
        std::cout << "Error: " << e << '\n';
    }

    return 0;
}
