all:  build
#NOTE: compile on pcduino, install libjpeg-dev and libv4l-dev before compile:
# apt-get install libjpeg-dev libv4l-dev

build:	
	gcc -fPIC -Wall -c *.c -DLIBDIR=/
	gcc -shared *.o -o libCameraPlugin.so  -lrt -lm -ljpeg -D__USE_GNU=1 -D__GNUC__=1
	cp libCameraPlugin.so /usr/lib/scratch/plugins/so.CameraPlugin

clean:
	rm -f *.o *.d *.so ../../../Plugins/CameraPlugin
