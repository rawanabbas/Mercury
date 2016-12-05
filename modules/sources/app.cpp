#include "stdafx.h"
#include "server.hpp"
#include "client.hpp"
#include "peer.hpp"
#include "heartbeat.hpp"
#include "connection_manager.hpp"

int main(int argc, char const *argv[]) {


    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port>" << std::endl;
        exit(1);
    }


    std::cout << "Let's Authenticate!" << std::endl;

    std::string host = argv[1];
    int port = atoi(argv[2]);

    Client authClient(host, port);

    authClient.start();

    authClient.setCommand(std::string(1, (char)Commands::SignIn), [](void* client) -> void {


        if (((Client*) client) -> getStatus() != ClientStatus::Authenticated) {
            std::cout << "Unauthorized!" << std::endl;
        } else {

            std::string ownerId = ((Client*) client) -> getOwnerId();
            std::string username = ((Client*) client) -> getUsername();
            std::string host = ((Client*) client) -> getHost();
            int port = ((Client*) client) -> getServerPort();

            Heartbeat heartbeat(ownerId, username, host, port);
            ConnectionManager manager(ownerId, username, host, port);
            Server server(ownerId, username, 3001);

            server.start();
            heartbeat.start();
            manager.start();

            PeerMap peers = manager.getPeers();

            if (peers.size()) {

                PeerMap::iterator it;
                int index = 0;

                for (it = peers.begin(); it != peers.end(); ++it) {
                    std::cout << ++index << " : " << it -> first <<  " IP: " << it -> second -> getIP() << std::endl;
                }

                std::cout << "A random client has been selected for sending the file!"
                          << "\nClient Username: " << peers.begin() -> second -> getUsername() << std::endl;

                Client sClient(ownerId, username, peers[peers.begin() -> first] -> getIP(), 3002);


                sClient.start();

                std::cout << "A client has been created to send an image to!" << std::endl;

                sClient.setCommand(std::string(1, (char)Commands::EstablishConnection), [=](void *client) {

                    ((Client *) client)->setCommand(std::string(1, (char)Commands::File), [=](void *aClient) {

                        ((Client *) aClient)->setCommand(std::string(1, (char)Commands::SendFile), [=](void *client) {

                            std::cout << "Sending File" << std::endl;

                        });

                    });

                });

                sClient.join();

            } else {

                std::cerr << "An error has occured while fetching peers or no peers connected!" << std::endl;

            }

            heartbeat.join();
            manager.join();
            server.join();

        }
    });

    authClient.join();

    return 0;
}
