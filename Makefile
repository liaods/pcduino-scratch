all: clean build

build:
	$(MAKE) build -C src/plugins/scratch/
	$(MAKE) build -C src/plugins/unicode/
	$(MAKE) build -C src/plugins/camera/
	
clean:
	$(MAKE) clean -C src/plugins/scratch/
	$(MAKE) clean -C src/plugins/unicode/
	$(MAKE) clean -C src/plugins/camera/	
