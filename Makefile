AUDIO_CMD = yt-dlp -f bestaudio --extract-audio --audio-quality 64K --audio-format mp3 -o "site/assets/audio/keygen-Uh-p3TOIrOc.mp3" "https://www.youtube.com/watch?v=tPY-I3RX10c"

ifeq ($(OS),Windows_NT)
	MKDIR = if not exist build mkdir build
	RMDIR = if exist build rmdir /s /q build
else
    MKDIR = mkdir -p build
    RMDIR = rm -rf build
endif

all: build

prepare:
	$(MKDIR)
	$(AUDIO_CMD)

	python src/helpers/modify.py -i site/index.html -o build/index.modified.html
	python src/helpers/xxd.py -i build/index.modified.html -o build/index.modified.c

build: clean prepare
	cd src && cmake -G Ninja -B ../build -S . -D CMAKE_BUILD_TYPE=Release -Wno-dev
	cd src && cmake --build ../build

cross: clean prepare
	cd src && cmake -G "Ninja Multi-Config" -B ../build -S . -D CMAKE_TOOLCHAIN_FILE=cmake/toolchains/x86_64-w64-mingw32.cmake -Wno-dev
	cd src && cmake --build ../build --config Release

run:
	./build/bin/activator

clean:
	$(RMDIR)
