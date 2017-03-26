


def:
	@make -C test

testclean:
	@make -C test clean


install:
	cp -rf pgen2 $(INSTALL_PATH)

uninstall:
	rm -rf $(INSTALL_PATH)/pgen2



# .PHONY: depend
# depend:
# 	@echo "Create Dependfile"
# 	$(CXX) -MM -MG $(SRC) $(CXXFLAGS) -I$(INCLUDE_DIR) > depend.mk
#
# -include depend.mk
