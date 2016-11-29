#################################################################################################################################
# Makefile                                                                                                                      #
# Author: Group 3                                                                                                               #
# Date: 6th November 2016                                                                                                       #
# CSCE 4411 - Project One Mercury                                                                                               #
#################################################################################################################################
.PHONY: all pre server client app peering clean
SRC_DIR = ./sources
INCLUDE_DIR = ./include
INCLUDE = -I$(INCLUDE_DIR)
BINARY_DIR = ./binary
BUILD_DIR = ./build
OBJ_DIR = ./objects

CC = g++
DEBUG = g
LIBS = -lpthread -lcrypto++ -lsqlite3
CFLAGS = -std=c++11 -Wall -Wno-reorder $(LIBS)

SERVER_OBJS = \
	$(OBJ_DIR)/udp_socket.o \
	$(OBJ_DIR)/thread.o \
	$(OBJ_DIR)/job.o \
	$(OBJ_DIR)/message.o \
	$(OBJ_DIR)/file.o \
	$(OBJ_DIR)/server.o \
	$(OBJ_DIR)/decoder.o
	# $(OBJ_DIR)/server_main.o

CLIENT_OBJS = \
	$(OBJ_DIR)/client.o \
	$(OBJ_DIR)/udp_socket.o \
	$(OBJ_DIR)/thread.o \
	$(OBJ_DIR)/message.o \
	$(OBJ_DIR)/job.o \
	$(OBJ_DIR)/file.o \
	$(OBJ_DIR)/decoder.o
	# $(OBJ_DIR)/client_main.o

APP_OBJS = \
	$(OBJ_DIR)/udp_socket.o \
	$(OBJ_DIR)/job.o \
	$(OBJ_DIR)/thread.o \
	$(OBJ_DIR)/server.o \
	$(OBJ_DIR)/client.o \
	$(OBJ_DIR)/message.o \
	$(OBJ_DIR)/file.o \
	$(OBJ_DIR)/decoder.o \
	$(OBJ_DIR)/peer.o \
	$(OBJ_DIR)/heartbeat.o \
	$(OBJ_DIR)/app.o

PEER_OBJS = \
	$(OBJ_DIR)/udp_socket.o \
	$(OBJ_DIR)/thread.o \
	$(OBJ_DIR)/tracker.o \
	$(OBJ_DIR)/database.o \
	$(OBJ_DIR)/user.o \
	$(OBJ_DIR)/message.o \
	$(OBJ_DIR)/peer_server.o \
	$(OBJ_DIR)/peer.o \
	$(OBJ_DIR)/decoder.o \
	$(OBJ_DIR)/peer_server_main.o


PRECOMPILED_HEADER_BINARY = stdfax.h.gch
PRECOMPILED_HEADER = stdfax.h

SERVER_BINARY = server.o

CLIENT_BINARY = client.o

APP_BINARY = app.o

PEER_BINARY = peer.o

SERVER_MAIN = $(SRC_DIR)/server_main.cpp

CLIENT_MAIN = $(SRC_DIR)/client_main.cpp

MKDIR = mkdir -p

MKDIR_OBJS = $(MKDIR) $(OBJ_DIR)

MKDIR_BINARY = $(MKDIR) $(BINARY_DIR)

MKDIR_BUILD = $(MKDIR) $(BUILD_DIR)



all: pre server client app peering

pre:
	$(MKDIR_BUILD); $(MKDIR_BINARY); $(MKDIR_BINARY)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)/$(notdir $@)  $(CFLAGS)

%.o: pre $(SRC_DIR)/%.cpp
		echo $< $@
		$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)/$(notdir $@)  $(CFLAGS)

%.h.gch: pre $(INCLUDE_DIR)/%.h
	$(CC) -c $< -o $(BINARY_DIR)/$(notdir $@) $(CFLAGS)

server: pre $(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) $(INCLUDE) -o $(BUILD_DIR)/$(SERVER_BINARY) $(CFLAGS)

client: pre $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) $(INCLUDE) -o $(BUILD_DIR)/$(CLIENT_BINARY) $(CFLAGS)

app: pre $(APP_OBJS)
	$(CC) $(APP_OBJS) $(INCLUDE) -o $(BUILD_DIR)/$(APP_BINARY) $(CFLAGS)

peering: pre $(PEER_OBJS)
	$(CC) $(PEER_OBJS) $(INCLUDE) -o $(BUILD_DIR)/$(PEER_BINARY) $(CFLAGS)

clean:
	rm -rf $(BUILD_DIR)/*.o $(BINARY_DIR)/*.o $(OBJ_DIR)/*.o
