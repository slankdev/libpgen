
include ../../config.mk
INCLUDE_DIR = ../../include


PROTCOL_DIR = protocol


build: address.o header.o packet.o build-protocol


build-protocol:
	$(MAKE) -C $(PROTCOL_DIR) build


clean:
	$(RM) *.o
	$(MAKE) -C $(PROTCOL_DIR) clean
