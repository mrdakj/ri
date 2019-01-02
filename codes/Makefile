CXX     = g++
CXXFLAGS = -std=c++17
LDLIBS  = -lstdc++fs `pkg-config opencv --cflags --libs`
WFLAGS = -Wall -Wextra -O3

binarization: binarization.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(WFLAGS) $(LDLIBS) 

.PHONY: clean

clean:
	rm -rf binarization
