#Name: Suraj Garg
#Roll No: 2018202003

CC = g++
CFLAGS = -g -Wall -std=c++1z
DEPS = connection.h utilities.h
OBJ = connection.o utilities.o tracker1.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

tracker1: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

