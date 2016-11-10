#################################################################################################################################
# Makefile                                                                                                                      #
# Author: Group 3                                                                                                               #
# Date: 6th November 2016                                                                                                       #
# CSCE 4411 - Project One Mercury                                                                                               #
#################################################################################################################################
.PHONY: all server client clean
SRC_DIR = ./sources
OBJ_DIR = ./objects
INCLUDE_DIR = ./include
INCLUDE = -I$(INCLUDE_DIR)
BINARY = ./binary
BUILD = ./build

CC = g++
DEBUG = g
LIBS = -lpthread
CFLAGS = -std=c++11 -Wall -Wno-reorder $(LIBS)

SERVER_OBJS = \
	$(OBJ_DIR)/udp_socket.o \
	$(OBJ_DIR)/thread.o \
	$(OBJ_DIR)/job.o \
	$(OBJ_DIR)/server.o \
	$(OBJ_DIR)/server_main.o

CLIENT_OBJS = \
	$(OBJ_DIR)/client.o \
	$(OBJ_DIR)/udp_socket.o \
	$(OBJ_DIR)/thread.o \
	$(OBJ_DIR)/job.o \
	$(OBJ_DIR)/client_main.o

SERVER_BINARY = server.o

CLIENT_BINARY = client.o

SERVER_MAIN = $(SRC_DIR)/server_main.cpp

CLIENT_MAIN = $(SRC_DIR)/client_main.cpp

all: server client

%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)/$(notdir $@)  $(CFLAGS)

pserver:
	$(foreach file,$(SERVER_OBJS),\
	make $(notdir $(file));)

pclient:
	$(foreach file,$(CLIENT_OBJS),\
	make $(notdir $(file));)

server: pserver $(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) $(INCLUDE) -o $(BUILD)/$(SERVER_BINARY) $(CFLAGS)

client: pclient $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) $(INCLUDE) -o $(BUILD)/$(CLIENT_BINARY) $(CFLAGS)

clean:
	rm -rf $(BUILD)/*.o $(BINARY)/*.o $(OBJ_DIR)/*.o
