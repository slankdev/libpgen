
include ../config.mk


IO_DIR    = io
CORE_DIR  = core
TYPES_DIR = types
ARCH_DIR  = arch



build: 
	$(MAKE) -C $(IO_DIR) build
	$(MAKE) -C $(CORE_DIR) build
	$(MAKE) -C $(TYPES_DIR) build
	$(MAKE) -C $(ARCH_DIR) build



clean:
	$(MAKE) -C $(IO_DIR) clean
	$(MAKE) -C $(CORE_DIR) clean
	$(MAKE) -C $(TYPES_DIR) clean
	$(MAKE) -C $(ARCH_DIR) clean



