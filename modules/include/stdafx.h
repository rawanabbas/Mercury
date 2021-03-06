/**
 * A file that contains all the needed library in one place to avoid various design issues.
 * Author: Group 3
 * Date: 24th October 2016
 */

#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <vector>
#include <algorithm>
#include <pthread.h>
#include <ctime>
#include <time.h>
#include <fstream>
#include <sstream>
#include <map>
#include <sqlite3.h>

#include <map>
#include <netdb.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>


#include <cryptopp/sha.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>

#define END_COMMAND  "q"
#define MAX_CONNECTIONS  5
#define MAX_RECV  1024
#define SUCCESS 200
#define ERROR 400
#define MAX_READ 300
#define MAX_RETRY 3

typedef void (*Callback)(void*);
typedef std::map<std::string, std::string>  HeadersMap;

