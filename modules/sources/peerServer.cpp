#include "peerServer.h"


peerServer::peerServer()
{
	//the initialization of the server with the port and socket
}

bool peerServer::registerUser(_client client)
{
	if(clientsReg.count(client.ip)>0)	//check if the user already exists
	{
		std::cout<<"user already exists.\n";
		return false;
	}
	std:: string temp=client.ip;
	clientsReg[temp]=client;	//if this doesn't work, copy each element separately 
	std::cout<<"User successfully added.\n";
	return true;
}

bool peerServer::removeUser(std:: string ip)
{
	if(clientsReg.count(ip)>0)	//check if the user already exists
	{
		std::map<std:: string,_client>::iterator it;
		it=clientsReg.find(ip);
		clientsReg.erase(it, clientsReg.end());
		std::cout<<"User successfully removed.\n";
		return true;
	}
	std::cout<<"User already doesn't exist.\n";
	return false;
}

bool peerServer::checkAuthentication(std::string ip, std::string userName, std::string password)
{
	if(clientsReg.count(ip)>0)	//check if the user already exists
	{
		_client temp=clientsReg[ip];
		if(temp.userName==userName && temp.password==password)	//compare username and password to the ones already existing
			return true;
		else 
			return false;
	}	
	else
		return false;
}

void peerServer::initializeDb()
{
    if (sqlite3_open("Database.db", &_db))
        std::cout << "Can't open database.\n";
}

void peerServer:: addToDb(std::string ownerIp, std::string imageName, std::string decryptionKey, std::vector<std::string> viewersIp, std::vector <int>numberOfViews)
{
    char* error = 0;
  int connection;
  sql = "INSERT INTO USER (IP)" \
        "VALUES (" + ownerIp + ");";
  connection = sqlite3_exec (_db, sql, NULL, 0, &error);
 
  if (connection != SQLITE_OK)
      std::cout <<"Error inserting values into user table \n" ;
  
  sql = "INSERT INTO IMAGE (IMAGE_NAME, DEC_KEY, OWNER)" \
        "VALUES (" + imageName + ", " + decryptionKey + ", " + ownerIp + ");";
  
  connection = sqlite3_exec (_db, sql, NULL, 0, &error);
  //dsperr (&error);
  if (connection != SQLITE_OK)
      std::cout <<"Error inserting values into image table \n" ;
  
  
  for (int i = 0 ; i < viewersIp.size(); i++)
  {

      sql = "INSERT INTO IMAGE_USER (IMAGEID, USERIP, NUMBEROFVIEWS)" \
        "VALUES (" + sqlite3_last_insert_rowid(_db) + ", " +  viewersIp[i] + ", " + numberOfViews[i] + ");";
      connection = sqlite3_exec (_db, sql, NULL, 0, &error);
 
  if (connection != SQLITE_OK)
      std::cout <<"Error inserting values into image_user table \n" ;
  
  }
}

peerServer::~peerServer()
{
	//destroying the peering server

}
