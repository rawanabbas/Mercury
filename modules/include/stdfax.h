#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <vector>
#include <algorithm>
#include <pthread.h>
#include <ctime>
#include <fstream>

#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define END_COMMAND  "q"
#define MAX_CONNECTIONS  5
#define MAX_RECV  1024
#define SUCCESS 200
#define ERROR 400
