/**
 * A file that contains all the needed libraries in one place to avoid various design issues.
 * Author: Group 3
 * Date: 24th October 2016
 */

#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <vector>
#include <algorithm>
#include <pthread.h>
#include <ctime>
#include <fstream>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define END_COMMAND  "q"
#define MAX_CONNECTIONS  5
#define MAX_RECV  1024
#define SUCCESS 200
#define ERROR 400
