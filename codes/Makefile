CXX     = g++
CXXFLAGS = -std=c++17
LDLIBS  = -lstdc++fs `pkg-config opencv --cflags --libs`
WFLAGS = -Wall -Wextra -O3

edge_detection: edge_detection.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(WFLAGS) $(LDLIBS) 

binarization: binarization.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(WFLAGS) $(LDLIBS) 

binarization_2: binarization_2.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(WFLAGS) $(LDLIBS) 

.PHONY: clean

clean:
	rm -rf binarization binarization_2
