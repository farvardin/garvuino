CXX ?= g++
CXXFLAGS ?= -O2 -g -Wall -std=c++11
LDFLAGS ?=

vgm2rsf_OBJS = vgm2rsf.o

all: vgm2rsf

clean:
	rm -f vgm2rsf $(vgm2rsf_OBJS)

vgm2rsf: $(vgm2rsf_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) -lz

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: all clean
