/**
 * Main File for testing the Server
 * Author: Group 3
 * Date: 24th October 2016
 *  Project One - CSCE 4411
 */

#include "stdafx.h"
#include "server.hpp"

int main(int argc, char const *argv[]) {
    try {
        Server server("3E23E8160039594A33894F6564E1B1348BBD7A0088D42C4ACB73EEAED59C009D46E99B9FF66D3293E25757AE60CA3E7D", "b", 3001);
        server.start();
        server.join();
    } catch (const char * e ) {
        std::cerr << "Error: " << e << std::endl;
    }
    return 0;
}
