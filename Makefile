CC=g++
exe:=main

all:$ decoder.cpp
	$(CC) -std=c++11 -O3 -o $(exe) -I./ $^ `pkg-config opencv --cflags --libs`
