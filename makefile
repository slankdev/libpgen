


INSTALL_PATH := /usr/local/include


def:
	@echo test

install:
	cp -rf include/pgen2   $(INSTALL_PATH)

uninstall:
	rm -rf $(INSTALL_PATH)/pgen2



# .PHONY: depend
# depend:
# 	@echo "Create Dependfile"
# 	$(CXX) -MM -MG $(SRC) $(CXXFLAGS) -I$(INCLUDE_DIR) > depend.mk
#
# -include depend.mk
