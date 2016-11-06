#include "stdfax.h"
// #include "socket.hpp"
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
