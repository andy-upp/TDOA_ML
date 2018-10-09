# Makefile for this project
# C++ complier
CXX = g++
CXXFLAGS = -Wall -g

main: bin/main.o bin/MyPoint2D.o
	$(CXX) -o main bin/main.o bin/MyPoint2D.o

bin/main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

bin/MyPoint2D.o: include/MyPoint2D.cpp
	$(CXX) $(CXXFLAGS) include/MyPoint2D.cpp
