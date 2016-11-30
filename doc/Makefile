SRC = ../jules/array/slice.hpp ../jules/core/type.hpp
JULES_PATH = $(shell pwd)/..

all: standardese_modules.html

standardese_modules.html: $(SRC)
	standardese -I $(JULES_PATH) --output.format html $(SRC)
	firefox $@
