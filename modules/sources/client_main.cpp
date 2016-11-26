/**
 * Main File for testing the Client
 * Author: Group 3
 * Date: 24th October 2016
 *  Project One - CSCE 4411
 */

#include "stdfax.h"
#include "client.hpp"
#include "udp_socket.hpp"

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port>" << std::endl;
        exit(1);
    }
    std::cout << "Hello! Let's Do This! " << std::endl;
    Client client("127.0.0.1", 3001);
    client.start();
    client.join();

    return 0;
}
