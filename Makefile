# Makefile for this project
# C++ complier
CXX = g++
CXXFLAGS = -Wall -c
##############################################
# Path
BIN = bin
INCLUDE = include
SRC = src
##############################################
# Link
main: $(BIN)/main.o $(BIN)/MyPoint2D.o
	$(CXX) -o main bin/main.o bin/MyPoint2D.o
##############################################
# Compile
$(BIN)/main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o bin/main.o

$(BIN)/MyPoint2D.o: $(SRC)/MyPoint2D.cpp
	$(CXX) $(CXXFLAGS) $(SRC)/MyPoint2D.cpp -o bin/MyPoint2D.o
##############################################
# Clean Object file
clean:
	del $(BIN)\*.o
	del *.exe