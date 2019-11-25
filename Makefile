CXX      = g++
CXXFLAGS = -O3 -Wall -std=c++11
RM       = rm -f

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@
mkind1.out: mkind1.o judwin.o
	$(CXX) $(CXXFLAGS) $^ -o $@
sample.out: sample.o calsht.o
	$(CXX) $(CXXFLAGS) $^ -o $@
clean:
	$(RM) *.o