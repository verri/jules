SRC = ../jules/array/slice.hpp ../jules/core/type.hpp ../jules/array/ref_array.hpp
JULES_PATH = ..

all: standardese_modules.html

standardese_modules.html: $(SRC)
	standardese -I $(JULES_PATH) --output.format html $(SRC)
	firefox ref_array.html
