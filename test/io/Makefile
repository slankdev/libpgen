

PREFIX   = ../../
CXXFLAGS = -std=c++11 -g -I$(PREFIX)

SRCS = \
	test_netif.cc \
	test_pcap.cc  \
	test_pcapng.cc
OBJS = $(SRCS:.cc=.out)

.cc.out:
	@echo "CXX $@"
	@$(CXX) $(CXXFLAGS) $< -o $@

def: $(OBJS)

clean:
	rm -rf $(OBJS)
