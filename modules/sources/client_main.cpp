#include "stdfax.h"
#include "client.hpp"
#include "udp_socket.hpp"

int main(int argc, char const *argv[]) {
    std::cout << "Client is Now Sending ..." << std::endl;
    Client client("127.0.0.1", 3001);
    client.start();
    client.join();

    return 0;
}
