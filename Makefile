ifeq ($(OS),Windows_NT)
    AUDIO_CMD = yt-dlp -f bestaudio --extract-audio --audio-quality 64K --audio-format mp3 -o "site/assets/audio/keygen-Uh-p3TOIrOc.%%(ext)s" "https://www.youtube.com/watch?v=tPY-I3RX10c"
    RM = del /f /q
    MKDIR = mkdir 2> NUL || exit 0
    MOVE = move /Y
    RMDIR = if exist build rmdir /s /q build
else
    AUDIO_CMD = if [ ! -f "site/assets/audio/keygen-Uh-p3TOIrOc.mp3" ]; then yt-dlp -f bestaudio --extract-audio --audio-quality 64K --audio-format mp3 -o "site/assets/audio/keygen-Uh-p3TOIrOc.%(ext)s" "https://www.youtube.com/watch?v=tPY-I3RX10c"; fi
    RM = rm -f
    MKDIR = mkdir -p
    MOVE = mv
    RMDIR = if [ -d "build" ]; then rm -rf build; fi
endif

all: build

prepare:
	$(AUDIO_CMD)
	python src/helpers/modify.py site/index.html
	python src/helpers/xxd.py index.modified.html

	$(MKDIR) build
	$(RM) index.modified.html
	$(MOVE) index.modified.c build

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
