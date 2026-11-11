CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2

all: list_manager_c.exe list_manager_cpp.exe test_runner.exe

list_manager_c.exe: dynamic_list.c main.c dynamic_list.h
	$(CC) $(CFLAGS) dynamic_list.c main.c -o list_manager_c.exe

list_manager_cpp.exe: list_manager.cpp
	$(CXX) $(CXXFLAGS) list_manager.cpp -o list_manager_cpp.exe

test_runner.exe: dynamic_list.c test_runner.c dynamic_list.h
	$(CC) $(CFLAGS) dynamic_list.c test_runner.c -o test_runner.exe

test: test_runner.exe
	./test_runner.exe

clean:
	del /f /q *.exe *.o 2>nul || rm -f *.exe *.o

