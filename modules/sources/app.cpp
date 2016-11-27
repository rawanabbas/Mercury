#include "stdfax.h"
#include "server.hpp"
#include "heartbeat.hpp"

int main(int argc, char const *argv[]) {

    Server server(3002);

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port>" << std::endl;
        exit(1);
    }

    Heartbeat heartbeat(argv[1], atoi(argv[2]));
    server.start();
    heartbeat.start();
    server.join();
    heartbeat.join();

    return 0;
}
