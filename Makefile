CMAKE ?= cmake

ifeq ($(OS),Windows_NT)
SHELL := cmd
PYTHON ?= python

CREATE_BUILD_DIRECTORY = if not exist build mkdir build
DELETE_BUILD_DIRECTORY = if exist build rmdir /s /q build
CHECK_DEPENDENCIES = powershell -ExecutionPolicy Bypass -File "scripts/deps.ps1"
PACK_BINARY = powershell -ExecutionPolicy Bypass -File "scripts/pack.ps1"
RUN_BINARY = /C "set __COMPAT_LAYER=RUNASINVOKER && start /wait ./build/bin/activator"
else
PYTHON ?= python3

CREATE_BUILD_DIRECTORY = mkdir -p build build/_deps/ohook/src/ohook
DELETE_BUILD_DIRECTORY = rm -rf build
CHECK_DEPENDENCIES = bash ./scripts/deps.sh
PACK_BINARY = bash ./scripts/pack.sh
RUN_BINARY = ./build/bin/activator
endif

.PHONY: all prepare build cross test run clean

all: build

prepare:
	$(CREATE_BUILD_DIRECTORY)
	$(CHECK_DEPENDENCIES)
	$(PYTHON) scripts/bundle_html.py --input site/index.html --output build/index.modified.html
	$(PYTHON) scripts/binary_to_c.py --input build/index.modified.html --output build/index.modified.c

build: prepare
	$(CMAKE) -G Ninja -B build -S src -D CMAKE_BUILD_TYPE=Release
	$(CMAKE) --build build
	$(PACK_BINARY)

cross: prepare
	$(CMAKE) -G "Ninja Multi-Config" -B build -S src -D CMAKE_TOOLCHAIN_FILE="$(CURDIR)/src/cmake/toolchains/x86_64-w64-mingw32.cmake"
	$(CMAKE) --build build --config Release
	$(PACK_BINARY)

test:
	$(PYTHON) -m unittest discover -s tests -v

run:
	$(RUN_BINARY)

clean:
	$(DELETE_BUILD_DIRECTORY)
