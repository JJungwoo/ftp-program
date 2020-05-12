CC = gcc
CFLAGS = -pthread
TARGET = ftp_client 

SRC = ftp_client.c

BUILD = build/
OUTPUT = $(BUILD)/ftp_client.o $(BUILD)/common.o

COMMON_DIR = ../../common
COMMON_SRC = $(COMMON_DIR)/common.c

INC_DIRS = $(COMMON)

TARGET_OBJS = $(TARGET_SRCS:%.c=$(OBJS_DIR)/%.o)


$(TARGET) : ftp_client.o common.o
	$(CC) $(CFLAGS) $(INC_DIRS) -o $(TARGET) build/ftp_client.o build/common.o

ftp_client.o : ftp_client.c
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o  build/ftp_client.o $(SRC)

common.o : $(COMMON_SRC)
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o build/common.o $(COMMON_SRC)

clean :
	rm -rf *.o $(TARGET) build/*.o

