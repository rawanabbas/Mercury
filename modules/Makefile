#################################################################################################################################
# Makefile                                                                                                                      #
# Author: Group 3                                                                                                               #
# Date: 6th November 2016                                                                                                       #
# CSCE 4411 - Project One Mercury                                                                                               #
#################################################################################################################################
.PHONY: all pre server client app peering clean
SRC_DIR = ./sources
OBJ_DIR = ./objects
INCLUDE_DIR = ./include
INCLUDE = -I$(INCLUDE_DIR)
BINARY = ./binary
BUILD = ./build

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

all: pre server client app peering

%.o: $(SRC_DIR)/%.cpp
	echo $< $@
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)/$(notdir $@)  $(CFLAGS)

%.h.gch: $(INCLUDE_DIR)/%.h
	$(CC) -c $< -o $(BINARY)/$(notdir $@) $(CFLAGS)

pre: $(PRECOMPILED_HEADER_BINARY)

pserver:
	$(foreach file,$(SERVER_OBJS),\
	make $(notdir $(file));)

pclient:
	$(foreach file,$(CLIENT_OBJS),\
	make $(notdir $(file));)

papp:
	$(foreach file,$(APP_OBJS),\
	make $(notdir $(file));)

ppeering: 
		$(foreach file,$(PEER_OBJS),\
		make $(notdir $(file));)

server: pre pserver
	$(CC) $(SERVER_OBJS) $(INCLUDE) -o $(BUILD)/$(SERVER_BINARY) $(CFLAGS)

client: pre pclient
	$(CC) $(CLIENT_OBJS) $(INCLUDE) -o $(BUILD)/$(CLIENT_BINARY) $(CFLAGS)

app: pre papp
	$(CC) $(APP_OBJS) $(INCLUDE) -o $(BUILD)/$(APP_BINARY) $(CFLAGS)

peering: pre ppeering
	$(CC) $(PEER_OBJS) $(INCLUDE) -o $(BUILD)/$(PEER_BINARY) $(CFLAGS)

clean:
	rm -rf $(BUILD)/*.o $(BINARY)/*.o $(OBJ_DIR)/*.o
