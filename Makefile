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
main: $(BIN)/main.o $(BIN)/MyPoint2D.o\
 $(BIN)/Setting1.o $(BIN)/Test.o
	$(CXX) -o main $(BIN)/main.o $(BIN)/MyPoint2D.o\
	 $(BIN)/Setting1.o $(BIN)/Test.o
##############################################
# Compile
$(BIN)/main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(BIN)/main.o

$(BIN)/MyPoint2D.o: $(SRC)/MyPoint2D.cpp
	$(CXX) $(CXXFLAGS) $(SRC)/MyPoint2D.cpp -o $(BIN)/MyPoint2D.o

$(BIN)/Setting1.o: $(SRC)/Setting1.cpp
	$(CXX) $(CXXFLAGS) $(SRC)/Setting1.cpp -o $(BIN)/Setting1.o

$(BIN)/Test.o: $(SRC)/Test.cpp
	$(CXX) $(CXXFLAGS) $(SRC)/Test.cpp -o $(BIN)/Test.o
##############################################
# Clean Object file
clean:
	del $(BIN)\*.o
	del *.exe