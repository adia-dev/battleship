SRC_DIR = src
BUILD_DIR = build/Debug
CC = gcc
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_NAME = app
INCLUDE_PATHS = -Iinclude
LIB_PATH = -Llib
C_FLAGS =  -Wall -O0 -g
LINKER_FLAGS = 

all:
	$(CC) $(C_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIB_PATH) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)
run:
	./build/Debug/app