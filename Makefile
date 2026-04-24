#default target

.PHONY: all run test clean pack package-linux package-windows doc stddev help

all: 
	cmake -S . -B build
	cmake --build  build

run: all
	./build/src/gui/ivs-kalkulacka

test: all
	./build/src/math/calculator_math_tests

clean:
	rm -rf build

package-linux:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build --config Release --target gui
	cd build && cpack -G DEB

package-windows:
	@if [ "$(OS)" != "Windows_NT" ]; then \
		echo "Windows installer build must be run on Windows 11 x64 with Qt and NSIS installed."; \
		exit 1; \
	fi
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build --config Release --target gui
	cd build && cpack -G NSIS

pack:
	zip -r projekt.zip src Makefile README.md CMakeLists.txt -x "*.git*"

doc:
	@echo "Doxygen dorobit"

stddev:
	@echo "stddev dorobit"

help:
	@echo "make all 	- build project"
	@echo "make run 	- run application"
	@echo "make test	- run tests"
	@echo "make clean	- remove build files"
	@echo "make package-linux	- create Ubuntu/Debian .deb installer"
	@echo "make package-windows	- create Windows NSIS installer (.exe)"
	@echo "make pack	- create archive"
	@echo "make doc		- generate documentation"
	@echo "make stddev	- run profiling"
