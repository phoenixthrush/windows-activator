# TODO: Instead of using xxd,
# 		embed the raw data directly into the binary

ifeq ($(OS),Windows_NT)
AUDIO_CMD = yt-dlp -f bestaudio --extract-audio --audio-format mp3 -o "site/assets/audio/keygen-Uh-p3TOIrOc.%%(ext)s" "https://www.youtube.com/watch?v=tPY-I3RX10c"
else
AUDIO_CMD = if [ ! -f "site/assets/audio/keygen-Uh-p3TOIrOc.mp3" ]; then yt-dlp -f bestaudio --extract-audio --audio-format mp3 -o "site/assets/audio/keygen-Uh-p3TOIrOc.%%(ext)s" "https://www.youtube.com/watch?v=tPY-I3RX10c"; fi
endif

all:
	$(AUDIO_CMD)
	python helpers/modify.py site/index.html
	python helpers/xxd.py index.modified.html

	cmake -G Ninja -B build -S . -D CMAKE_BUILD_TYPE=Release
	cmake --build build

cross: clean
	python helpers/modify.py site/index.html
	python helpers/xxd.py index.modified.html

	cmake -G "Ninja Multi-Config" -B build -S . -D CMAKE_TOOLCHAIN_FILE=cmake/toolchains/x86_64-w64-mingw32.cmake
	cmake --build build --config Release

run:
	./build/bin/activator

clean:
ifeq ($(OS), Windows_NT)
	if exist build rmdir /s /q build
	if exist index.modified.html del /f /q index.modified.html
	if exist index.modified.c del /f /q index.modified.c
else
	if [ -d "build" ]; then rm -rf build; fi
	if [ -f "index.modified.html" ]; then rm -f index.modified.html; fi
	if [ -f "index.modified.c" ]; then rm -f index.modified.c; fi
endif