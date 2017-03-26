

PREFIX   = ../../
CXXFLAGS = -std=c++11 -g -I$(PREFIX)


SRCS = \
	test_eth.cc
OBJS = $(SRCS:.cc=.out)

.cc.out:
	@echo "CXX $@"
	@$(CXX) $(CXXFLAGS) $< -o $@


def: $(OBJS)

clean:
	rm -rf *.out
