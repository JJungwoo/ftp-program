CC = gcc
CFLAGS = -pthread
TARGET = ftp_server 

FTP_SRC = ftp_server.c ftp_func.c

INC	= -I./

BUILD = build
OUTPUT = $(BUILD)/ftp_server.o $(BUILD)/common.o $(BUILD)/ftp_func.o

COMMON_DIR = ../common
COMMON_SRC = $(COMMON_DIR)/common.c

INC_DIRS = $(COMMON)

TARGET_OBJS = $(TARGET_SRCS:%.c=$(OBJS_DIR)/%.o)


$(TARGET) : ftp_server.o common.o ftp_func.o
	$(CC) $(CFLAGS) $(INC) $(INC_DIRS) -o $(TARGET) build/ftp_server.o build/common.o build/ftp_func.o

ftp_server.o :ftp_server.c 
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o build/ftp_server.o ftp_server.c 

ftp_func.o :ftp_func.c
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o build/ftp_func.o ftp_func.c

common.o : $(COMMON_SRC)
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o build/common.o $(COMMON_SRC)

clean :
	rm -rf *.o $(TARGET) build/*.o

