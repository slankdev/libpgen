

SRC_DIR = src



all: build




build: 
	$(MAKE) -C $(SRC_DIR) build


clean:
	$(MAKE) -C $(SRC_DIR) clean
