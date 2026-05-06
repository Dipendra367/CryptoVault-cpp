CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
LIBS = -lcryptopp
SRC = $(wildcard src/*.cpp)
OUT = cryptovault

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)