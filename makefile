

all:
	$(MAKE) -C lib/

clean:
	$(MAKE) -C lib/ clean

install:
	$(MAKE) -C lib/ install

uninstall:
	$(MAKE) -C lib/ uninstall
