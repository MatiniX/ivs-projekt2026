#default target

.PHONY: all run test clean pack package-linux package-windows doc stddev debug-math profile-stddev profile-input-random-1e7 profile-stddev-heavy help

all: 
	cmake -S . -B build
	cmake --build  build

run: all
	./build/gui/ivs-kalkulacka

test: all
	./build/math/calculator_math_tests

clean:
	rm -rf build build-debug build-gprof
	rm -f gmon.out gmon.sum projekt.zip
	rm -f profiling/gmon_*.out profiling/gprof_*.txt

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
	@set -e; \
	TMP_DIR=$$(mktemp -d); \
	mkdir -p "$$TMP_DIR/doc" "$$TMP_DIR/install" "$$TMP_DIR/repo"; \
	if [ -d ../doc ]; then cp -a ../doc/. "$$TMP_DIR/doc/"; fi; \
	if ! find build -maxdepth 1 -type f \( -name "*.deb" -o -name "*.exe" -o -name "*.msi" \) | grep -q .; then \
		echo "Calculator installer not found in build/. Trying to generate it..."; \
		$(MAKE) package-linux; \
	fi; \
	find build -maxdepth 1 -type f \( -name "*.deb" -o -name "*.exe" -o -name "*.msi" \) -exec cp -a {} "$$TMP_DIR/install/" \; 2>/dev/null || true; \
	if ! find "$$TMP_DIR/install" -maxdepth 1 -type f \( -name "*.deb" -o -name "*.exe" -o -name "*.msi" \) | grep -q .; then \
		echo "No calculator installer was found (.deb/.exe/.msi)."; \
		rm -rf "$$TMP_DIR"; \
		exit 1; \
	fi; \
	if [ -f build/profiler/stddev ]; then cp -a build/profiler/stddev "$$TMP_DIR/install/"; fi; \
	find build -type f \( -name "*stddev*.deb" -o -name "*stddev*.exe" -o -name "*stddev*.msi" -o -name "*stddev*.zip" -o -name "*stddev*.tar.gz" \) -exec cp -a {} "$$TMP_DIR/install/" \; 2>/dev/null || true; \
	(cd .. && tar --exclude="./src/build" --exclude="./src/build-gprof" --exclude="./projekt.zip" --exclude="./src/projekt.zip" --exclude="./.DS_Store" -cf - .) | (cd "$$TMP_DIR/repo" && tar -xf -); \
	rm -f ../projekt.zip projekt.zip; \
	(cd "$$TMP_DIR" && zip -rq "$(CURDIR)/../projekt.zip" doc install repo); \
	rm -rf "$$TMP_DIR"; \
	echo "Archive created: ../projekt.zip"

doc:
	doxygen Doxyfile

stddev:
	cmake -S . -B build
	cmake --build build --target stddev
	@echo "Built executable: ./build/profiler/stddev"

debug-math:
	@command -v gdb >/dev/null || (echo "gdb is required but not installed"; exit 1)
	cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
	cmake --build build-debug --target calculator_math_tests
	gdb ./build-debug/src/math/calculator_math_tests

profile-stddev:
	@set -e; \
	command -v gprof >/dev/null || (echo "gprof is required but not installed"; exit 1); \
	test -f ../profiling/input_10.txt || (echo "Missing profiling/input_10.txt"; exit 1); \
	test -f ../profiling/input_1e3.txt || (echo "Missing profiling/input_1e3.txt"; exit 1); \
	test -f ../profiling/input_1e6.txt || (echo "Missing profiling/input_1e6.txt"; exit 1); \
	test -f ../profiling/input_random_1e6.txt || (echo "Missing profiling/input_random_1e6.txt"; exit 1); \
	INCLUDE_1E7=$${PROFILE_INCLUDE_1E7:-0}; \
	if [ "$$INCLUDE_1E7" = "1" ]; then \
		test -f ../profiling/input_random_1e7.txt || (echo "Missing profiling/input_random_1e7.txt"; exit 1); \
	fi; \
	rm -f gmon.out gmon.sum ../profiling/gmon_*.out ../profiling/gprof_*.txt; \
	cmake -S . -B build-gprof -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE="-O2 -pg" -DCMAKE_EXE_LINKER_FLAGS_RELEASE="-pg"; \
	cmake --build build-gprof --target stddev; \
	BIN=./build-gprof/profiler/stddev; \
	REPS_10=$${PROFILE_REPS_10:-150}; \
	REPS_1E3=$${PROFILE_REPS_1E3:-100}; \
	REPS_1E6=$${PROFILE_REPS_1E6:-10}; \
	REPS_RANDOM_1E6=$${PROFILE_REPS_RANDOM_1E6:-10}; \
	REPS_1E7=$${PROFILE_REPS_1E7:-5}; \
	run_profile() { \
		INPUT="$$1"; NAME="$$2"; REPS="$$3"; \
		TMP_RUN_DIR=$$(mktemp -d); \
		i=1; \
		echo "Profiling $$NAME ($$REPS runs)..."; \
		rm -f gmon.out gmon.sum; \
		while [ $$i -le $$REPS ]; do \
			"$$BIN" < "$$INPUT" > /dev/null; \
			mv gmon.out "$$TMP_RUN_DIR/gmon_$$i.out"; \
			i=$$((i + 1)); \
		done; \
		gprof -s "$$BIN" "$$TMP_RUN_DIR"/gmon_*.out > /dev/null; \
		mv gmon.sum "../profiling/gmon_$$NAME.out"; \
		gprof "$$BIN" "../profiling/gmon_$$NAME.out" > "../profiling/gprof_$$NAME.txt"; \
		rm -rf "$$TMP_RUN_DIR"; \
	}; \
	run_profile ../profiling/input_10.txt 10 "$$REPS_10"; \
	run_profile ../profiling/input_1e3.txt 1e3 "$$REPS_1E3"; \
	run_profile ../profiling/input_1e6.txt 1e6 "$$REPS_1E6"; \
	run_profile ../profiling/input_random_1e6.txt random_1e6 "$$REPS_RANDOM_1E6"; \
	if [ "$$INCLUDE_1E7" = "1" ]; then \
		run_profile ../profiling/input_random_1e7.txt random_1e7 "$$REPS_1E7"; \
	fi; \
	echo "Profiling reports created in ./profiling"

profile-input-random-1e7:
	@mkdir -p profiling
	@if [ -f ../profiling/input_random_1e7.txt ]; then \
		echo "Input already exists: ../profiling/input_random_1e7.txt"; \
	else \
		echo "Generating ../profiling/input_random_1e7.txt (10^7 random values)..."; \
		awk 'BEGIN { srand(); for (i = 1; i <= 10000000; i++) print rand() * 1000000 }' > ../profiling/input_random_1e7.txt; \
		echo "Generated profiling/input_random_1e7.txt"; \
	fi

profile-stddev-heavy: profile-input-random-1e7
	PROFILE_INCLUDE_1E7=1 \
	PROFILE_REPS_10=500 \
	PROFILE_REPS_1E3=300 \
	PROFILE_REPS_1E6=30 \
	PROFILE_REPS_RANDOM_1E6=30 \
	PROFILE_REPS_1E7=10 \
	$(MAKE) profile-stddev

help:
	@echo "Postup pre uspesnu kompilaciu na Linux VM:" 
	@echo "  1) Nainstalujte zavislosti (Ubuntu/Debian):"
	@echo "     sudo apt update"
	@echo "     sudo apt install -y build-essential cmake qt6-base-dev qt6-tools-dev-tools zip"
	@echo "  2) V koreni repozitara spustite kompilaciu:"
	@echo "     make all"
	@echo "  3) Spustenie GUI kalkulacky:"
	@echo "     make run"
	@echo "  4) Spustenie testov matematickej kniznice:"
	@echo "     make test"
	@echo "  5) Vytvorenie Linux instalatora kalkulacky (.deb):"
	@echo "     make package-linux"
	@echo ""
	@echo "Dostupne prikazy projektu:"
	@echo "  make all                 - zostavi projekt"
	@echo "  make run                 - spusti aplikaciu"
	@echo "  make test                - spusti testy"
	@echo "  make clean               - odstrani build subory"
	@echo "  make package-linux       - vytvori instalator pre Ubuntu/Debian (.deb)"
	@echo "  make package-windows     - vytvori instalator pre Windows (NSIS, .exe, nutne spustit na Windows 11 x64 s nainstalovanym Qt a NSIS)"
	@echo "  make stddev              - zostavi profiler pre stddev"
	@echo "  make debug-math          - zostavi math testy v Debug a spusti ich v gdb"
	@echo "  make profile-stddev      - spusti gprof profilovanie pre stddev"
	@echo "  make profile-input-random-1e7 - vygeneruje nahodny vstup s 10^7 hodnotami"
	@echo "  make profile-stddev-heavy - spusti tazsie profilovanie vratane generovania nahodneho vstupu 1e7"
	@echo "  make pack                - vytvori odovzdavani archiv"
	@echo "  make doc                 - vygeneruje dokumentaciu"
