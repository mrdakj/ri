CXX     = g++
CXXFLAGS = -std=c++17
LDLIBS  = -lstdc++fs `pkg-config opencv --cflags --libs`
WFLAGS = -Wall -Wextra -O3

enhancement: fuzzy_enhancement.cpp image.o
	$(CXX) -o $@ $^ $(CXXFLAGS) $(WFLAGS) $(LDLIBS) 

edge_detection: edge_detection.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(WFLAGS) $(LDLIBS) 

binarization: binarization.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(WFLAGS) $(LDLIBS) 

image.o: image.cpp image.hpp
	$(CXX) -c $@ $^ $(CXXFLAGS) $(WFLAGS) $(LDLIBS) 

.PHONY: clean

clean:
	rm -rf binarization edge_detection enhancement
