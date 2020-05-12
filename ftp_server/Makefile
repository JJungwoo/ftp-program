CC = gcc
CFLAGS = -pthread
TARGET = ftp_server 

FTP_SRC = ftp_server.c

BUILD = build
OUTPUT = $(BUILD)/ftp_server.o $(BUILD)/common.o

COMMON_DIR = ../../common
COMMON_SRC = $(COMMON_DIR)/common.c

INC_DIRS = $(COMMON)

TARGET_OBJS = $(TARGET_SRCS:%.c=$(OBJS_DIR)/%.o)


$(TARGET) : ftp_server.o common.o
	$(CC) $(CFLAGS) $(INC_DIRS) -o $(TARGET) build/ftp_server.o build/common.o

ftp_server.o : $(FTP_SRC)
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o build/ftp_server.o $(FTP_SRC)

common.o : $(COMMON_SRC)
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o build/common.o $(COMMON_SRC)

clean :
	rm -rf *.o $(TARGET) build/*.o

