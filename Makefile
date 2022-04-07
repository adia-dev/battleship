SRC_DIR = src
BUILD_DIR = build/Debug
CC = gcc
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
SERVER_FILES = $(SRC_DIR)/multi_server.c $(SRC_DIR)/client.c $(SRC_DIR)/board.c $(SRC_DIR)/ship.c $(SRC_DIR)/game_manager.c
CLIENT_FILES = $(SRC_DIR)/client.c
OBJ_NAME = app
INCLUDE_PATHS = -Iinclude
LIB_PATH = -Llib
C_FLAGS =  -O0 -g
LINKER_FLAGS = 

all:
	$(CC) $(C_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIB_PATH) $(SRC_FILES) -o $(SRC_DIR)/app

server:
	$(CC) $(C_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIB_PATH) $(SERVER_FILES) -o $(BUILD_DIR)/server

client:
	$(CC) $(C_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIB_PATH) $(CLIENT_FILES) -o $(BUILD_DIR)/client

run:
	./build/Debug/app

run_admin:
	./src/app -u admin -p admin

run_client:
	./src/app -u user_2 -p Respons11
