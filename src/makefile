
include ../config.make


IO_DIR    = io
CORE_DIR  = core
TYPES_DIR = types



build: 
	$(MAKE) -C $(IO_DIR) build
	$(MAKE) -C $(CORE_DIR) build
	$(MAKE) -C $(TYPES_DIR) build



clean:
	$(MAKE) -C $(IO_DIR) clean
	$(MAKE) -C $(CORE_DIR) clean
	$(MAKE) -C $(TYPES_DIR) clean
	$(RM) *.o *.out



