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
    std::cout << "Client is Now Sending ..." << std::endl;
    Client client("10.40.34.2", 3001);
    client.start();
    client.join();

    return 0;
}
