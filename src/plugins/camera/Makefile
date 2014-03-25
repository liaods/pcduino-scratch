all: clean build

build:	
	gcc -fPIC -Wall -c *.c
	gcc -shared -lv4l2 *.o -o ../../../Plugins/CameraPlugin

clean:
	rm -f *.o ../../../Plugins/CameraPlugin
