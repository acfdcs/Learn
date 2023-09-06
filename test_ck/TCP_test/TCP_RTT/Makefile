CFLAGS := -Wall -g -O0
CC := gcc

SRCS    :=  $(wildcard *.c)
OBJS    :=  $(SRCS:%.c=%.o)

TARGET := server client
.PHONY : clean all

all: $(TARGET)

socket_test_server: server.o

socket_test: client.o

clean:
	-rm -f $(TARGET)
	-rm -f $(OBJS)

