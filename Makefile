ifeq ($(OS),Windows_NT)
    AUDIO_CMD = yt-dlp -f bestaudio --extract-audio --audio-quality 64K --audio-format mp3 -o "site/assets/audio/keygen-Uh-p3TOIrOc.%%(ext)s" "https://www.youtube.com/watch?v=tPY-I3RX10c"
    MKDIR = mkdir 2> NUL || exit 0
    RMDIR = if exist build rmdir /s /q build
else
    AUDIO_CMD = if [ ! -f "site/assets/audio/keygen-Uh-p3TOIrOc.mp3" ]; then yt-dlp -f bestaudio --extract-audio --audio-quality 64K --audio-format mp3 -o "site/assets/audio/keygen-Uh-p3TOIrOc.%(ext)s" "https://www.youtube.com/watch?v=tPY-I3RX10c"; fi
    MKDIR = mkdir -p
    RMDIR = if [ -d "build" ]; then rm -rf build; fi
endif

all: build

prepare:
	$(AUDIO_CMD)

	$(MKDIR) build
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
