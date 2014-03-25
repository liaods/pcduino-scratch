all: clean build

build:	
	gcc -fPIC -Wall -c `pkg-config --cflags pangocairo` *.c 
	gcc -shared `pkg-config --libs pangocairo` -lc *.o -o ../../../Plugins/UnicodePlugin

clean:
	rm -f *.o ../../../Plugins/UnicodePlugin
