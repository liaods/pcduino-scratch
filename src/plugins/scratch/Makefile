all: clean build

build:	
	gcc -fPIC -Wall -c *.c
	gcc -shared *.o -o ../../../Plugins/ScratchPlugin

clean:
	rm -f *.o ../../../Plugins/ScratchPlugin
