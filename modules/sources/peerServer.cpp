#include "peerServer.h"
using namespace std;


peerServer::peerServer()
{
	//the initialization of the server with the port and socket
}

bool peerServer::registerUser(_client client)
{
	if(clientsReg.count(ip)>0)	//check if the user already exists
	{
		cout<<"user already exists.\n";
		return false;
	}
	string temp=client.ip;
	clientReg[temp]=client;	//if this doesn't work, copy each element seperately 
	cout<<"User successfully added.\n";
	return true;
}

bool peerServer::removeUser(string ip)
{
	if(clientsReg.count(ip)>0)	//check if the user already exists
	{
		map<string,_client>::iterator it;
		it=clientsReg.find(ip);
		clientsReg.erase(it, clientsReg.end());
		cout<<"User successfully removed.\n";
		return true;
	}
	cout<<"User already doesn't exist.\n";
	return false;
}

bool peerServer::checkAuthentication(string ip, string userName, string passowrd)
{
	if(clientsReg.count(ip)>0)	//check if the user already exists
	{
		_client temp=clientsReg[ip];
		if(temp.userName==userName && temp.passowrd==password)	//compare username and password to the ones already existing
			return true
		else 
			return false;
	}	
	else
		return false;
}


peerServer::~peerServer()
{
	//deconstructing the peering server

}
