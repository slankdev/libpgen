
# CORE_DIR = core
# IO_DIR   = io
#
# def:
# 	$(MAKE) -C $(CORE_DIR)
# 	$(MAKE) -C $(IO_DIR)
#
# clean:
# 	$(MAKE) -C $(CORE_DIR) clean
# 	$(MAKE) -C $(IO_DIR)   clean
#



PREFIX   = ../
CXXFLAGS = -std=c++11 -g -I$(PREFIX)


SRCS = \
	test_core_eth.cc \
	test_io_netif.cc \
	test_io_pcap.cc  \
	test_io_pcapng.cc


OBJS = $(SRCS:.cc=.out)

.cc.out:
	@echo "CXX $@"
	@$(CXX) $(CXXFLAGS) $< -o $@


def: $(OBJS)

clean:
	rm -rf *.out


