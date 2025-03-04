ifeq ($(OS),Windows_NT)
	SHELL = cmd

	CREATE_BUILD_DIRECTORY = if not exist build mkdir build
	DELETE_BUILD_DIRECTORY = if exist build rmdir /s /q build

	CHECK_DEPENDENCIES = powershell -ExecutionPolicy Bypass -File "src/helpers/deps.ps1"
	PACK_USING_UPX = powershell -ExecutionPolicy Bypass -File "src/helpers/pack.ps1"

	RUN = /C "set __COMPAT_LAYER=RUNASINVOKER && start /wait ./build/bin/activator"
else
    CREATE_BUILD_DIRECTORY = mkdir -p build build/_deps/ohook/src/ohook/
    DELETE_BUILD_DIRECTORY = rm -rf build

	CHECK_DEPENDENCIES = bash ./src/helpers/deps.sh
	PACK_USING_UPX = bash ./src/helpers/pack.sh

	RUN = ./build/bin/activator
endif

all: build

prepare:
	$(CREATE_BUILD_DIRECTORY)
	$(CHECK_DEPENDENCIES)

	python src/helpers/modify.py -i site/index.html -o build/index.modified.html
	python src/helpers/xxd.py -i build/index.modified.html -o build/index.modified.c

build: prepare
	cd src && cmake -G Ninja -B ../build -S . -D CMAKE_BUILD_TYPE=Release -Wno-dev
	cd src && cmake --build ../build

	$(PACK_USING_UPX)

cross: prepare
	cd src && cmake -G "Ninja Multi-Config" -B ../build -S . -D CMAKE_TOOLCHAIN_FILE=cmake/toolchains/x86_64-w64-mingw32.cmake -Wno-dev
	cd src && cmake --build ../build --config Release

	$(PACK_USING_UPX)

run:
	$(RUN)

clean:
	$(DELETE_BUILD_DIRECTORY)
