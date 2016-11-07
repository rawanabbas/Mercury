/**
 * Main File for testing the Server
 * Author: Group 3
 * Date: 24th October 2016
 *  Project One - CSCE 4411
 */

#include "stdfax.h"
#include "server.hpp"

int main(int argc, char const *argv[]) {
    try {
        Server server(3001);
        server.start();
        server.join();
    } catch (const char * e ) {
        std::cerr << "Error: " << e << std::endl;
    }
    return 0;
}
