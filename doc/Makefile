SRC = ../jules
JULES_PATH = ..

all: standardese_modules.html

standardese_modules.html: $(SRC)
	standardese -I $(JULES_PATH) --output.format html $(SRC)
	firefox standardese_modules.html
