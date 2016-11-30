#include "stdafx.h"
#include "server.hpp"
#include "client.hpp"
#include "peer.hpp"
#include "heartbeat.hpp"

int main(int argc, char const *argv[]) {

    std::string ownerId;

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port>" << std::endl;
        exit(1);
    }

    std::string msg;

    std::cout << "Let's Authenticate!" << std::endl;
    std::cout << "Register Or Signin: ";
    std::cin >> msg;

    Client authClient(argv[1], atoi(argv[2]));

    authClient.start();
    authClient.setCommand(msg);

    while(authClient.getAuthStatus() == ClientAuthenticationStatus::Authenticating);


    if(authClient.isAuthenticated() && authClient.getAuthStatus() == ClientAuthenticationStatus::Authenticated) {


        ownerId = authClient.getOwnerId();

        Heartbeat heartbeat(ownerId, argv[1], atoi(argv[2]));
        Server server(ownerId, 3001);


        server.start();
        heartbeat.start();

        std::cout << "Now, Query Peering Server?";
        std::cin >> msg;
        std::cout << "Message: " << msg;

        if (msg == "y") {

            heartbeat.queryPeers();
            std::map<std::string, Peer*> peers = heartbeat.getPeers();
            std::map<std::string, Peer*>::iterator it = peers.begin();
            for (it; it != peers.end(); it++) {
                std::cout << "User: " << it -> first << " is Connected!" << std::endl;
            }

        }

        server.join();
        authClient.join();
        heartbeat.join();

    } else {

        std::cout << "Not Authenticated!" << std::endl;
        std::cout << "NOW EXITING!" << std::endl;
        std::cout << "Bye!" << std::endl;

    }




    return 0;
}
