# Makefile for this project
# C++ complier
CXX = g++
CXXFLAGS = -Wall -c
##############################################
# Path
BIN = bin
INCLUDE = include
SRC = src
TST = test
##############################################
# Link Mian function 
main: $(BIN)/main.o $(BIN)/MyPoint2D.o $(BIN)/Setting1.o
	$(CXX) -o main $(BIN)/main.o $(BIN)/MyPoint2D.o $(BIN)/Setting1.o

test: $(BIN)/test.o $(BIN)/MyPoint2D.o $(BIN)/Setting1.o
	$(CXX) -o test $(BIN)/test.o $(BIN)/MyPoint2D.o $(BIN)/Setting1.o
##############################################
# Compile
$(BIN)/main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(BIN)/main.o

$(BIN)/test.o: $(TST)/TestMain.cpp
	$(CXX) $(CXXFLAGS) $(TST)/TestMain.cpp -o $(BIN)/test.o

$(BIN)/MyPoint2D.o: $(SRC)/MyPoint2D.cpp
	$(CXX) $(CXXFLAGS) $(SRC)/MyPoint2D.cpp -o $(BIN)/MyPoint2D.o

$(BIN)/Setting1.o: $(SRC)/Setting1.cpp
	$(CXX) $(CXXFLAGS) $(SRC)/Setting1.cpp -o $(BIN)/Setting1.o

##############################################
# Clean Object file
clean:
	del $(BIN)\*.o
	del $(TST)\*.exe
	del *.exe