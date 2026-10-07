ROOTDIR = $(shell git rev-parse --show-toplevel)
BUILDDIR = $(ROOTDIR)/build
LIBFILE = $(BUILDDIR)/lib/libsim.so

all: lib app

app: $(BUILDDIR) lib
	cd apps/web-app; make

lib: $(BUILDDIR)
	cd src; make

lib/CppWebServer:
	cd lib && git clone https://github.com/dtorban/CppWebServer.git
	cd lib/CppWebServer && mkdir -p build
	cd lib/CppWebServer/build && cmake ..

lib/CppWebServer/build/install/include: lib/CppWebServer
	cd lib/CppWebServer/build && make install

$(BUILDDIR): lib/CppWebServer/build/install/include
	mkdir -p build/obj
	mkdir -p build/bin
	mkdir -p build/lib

run:
	./bin/start.sh
	
submission:
	zip -r project.zip include src

clean:
	rm -rf build
	rm -rf project.zip
