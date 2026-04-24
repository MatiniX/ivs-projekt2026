#default target

.PHONY: all run test clean pack package-linux package-windows doc stddev profile-stddev help

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
	cmake -S . -B build
	cmake --build build --target stddev
	@echo "Built executable: ./build/src/profiler/stddev"

profile-stddev:
	@command -v gprof >/dev/null || (echo "gprof is required but not installed"; exit 1)
	@test -f profiling/input_10.txt || (echo "Missing profiling/input_10.txt"; exit 1)
	@test -f profiling/input_1e3.txt || (echo "Missing profiling/input_1e3.txt"; exit 1)
	@test -f profiling/input_1e6.txt || (echo "Missing profiling/input_1e6.txt"; exit 1)
	rm -f gmon.out profiling/gmon_*.out profiling/gprof_*.txt
	cmake -S . -B build-gprof -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE="-O2 -pg" -DCMAKE_EXE_LINKER_FLAGS_RELEASE="-pg"
	cmake --build build-gprof --target stddev
	./build-gprof/src/profiler/stddev < profiling/input_10.txt > /dev/null
	mv gmon.out profiling/gmon_10.out
	gprof ./build-gprof/src/profiler/stddev profiling/gmon_10.out > profiling/gprof_10.txt
	./build-gprof/src/profiler/stddev < profiling/input_1e3.txt > /dev/null
	mv gmon.out profiling/gmon_1e3.out
	gprof ./build-gprof/src/profiler/stddev profiling/gmon_1e3.out > profiling/gprof_1e3.txt
	./build-gprof/src/profiler/stddev < profiling/input_1e6.txt > /dev/null
	mv gmon.out profiling/gmon_1e6.out
	gprof ./build-gprof/src/profiler/stddev profiling/gmon_1e6.out > profiling/gprof_1e6.txt
	@echo "Profiling reports created in ./profiling"

help:
	@echo "make all 	- build project"
	@echo "make run 	- run application"
	@echo "make test	- run tests"
	@echo "make clean	- remove build files"
	@echo "make package-linux	- create Ubuntu/Debian .deb installer"
	@echo "make package-windows	- create Windows NSIS installer (.exe)"
	@echo "make pack	- create archive"
	@echo "make doc		- generate documentation"
	@echo "make stddev	- build stddev profiler executable"
	@echo "make profile-stddev - run gprof profiling for stddev (10, 1e3, 1e6)"
