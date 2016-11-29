#include "stdafx.h"
#include "server.hpp"
#include "client.hpp"
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

        server.join();
        authClient.join();
        heartbeat.join();

    } else {
        std::cout << "Not Authenticated!" << std::endl;
    }




    return 0;
}
