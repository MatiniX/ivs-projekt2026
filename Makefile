#default target

all: 
	cmake -S . -B build
	cmake --build  build

run: all
	./build/src/gui/gui

test: all
	./build/src/math/calculator_math_tests

clean:
	rm -rf build

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
	@echo "make pack	- create archive"
	@echo "make doc		- generate documentation"
	@echo "make stddev	- run profiling"
