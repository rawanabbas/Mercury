#ifndef PEERSERVER
#define PEERSERVER
#include<map>

class peerServer
{
private:
	struct _client	//the client should be defined outside in another place
	{
		std::string ip;
		int port;
		std::string userName;
		std::string password;
	};
	std::map<std::string,_client> clientsReg;
	int _portNumber;
	std::string IP;
public:
	peerServer();	//initialize surver
	bool registerUser(_client client);	//registering the users
	bool removeUser(std::string);	//removing the users
	void pulse();	//for the heartbeat
	bool checkAuthentication(std::string ip, std::string userName, std::string passowrd);	//check if the user is authenticated or not
	~peerServer();	//destruct server
};

#endif // !PEERSERVER
