# TODO: Instead of using xxd, embed the raw data directly into the binary
#		Adjust bitrate and image quality dynamically to stay within the 1,572,864 byte limit

SOURCE_DIR=src

ifeq ($(OS),Windows_NT)
AUDIO_CMD = yt-dlp -f bestaudio --extract-audio --audio-quality 64K --audio-format mp3 -o "site/assets/audio/keygen-Uh-p3TOIrOc.%%(ext)s" "https://www.youtube.com/watch?v=tPY-I3RX10c"
else
AUDIO_CMD = if [ ! -f "site/assets/audio/keygen-Uh-p3TOIrOc.mp3" ]; then yt-dlp -f bestaudio --extract-audio --audio-quality 64K --audio-format mp3 -o "site/assets/audio/keygen-Uh-p3TOIrOc.%(ext)s" "https://www.youtube.com/watch?v=tPY-I3RX10c"; fi
endif

all:
	$(AUDIO_CMD)
	python src/helpers/modify.py site/index.html
	python src/helpers/xxd.py index.modified.html

	mkdir -p build
	rm -f index.modified.html
	mv index.modified.c build

	cd $(SOURCE_DIR) && cmake -G Ninja -B ../build -S . -D CMAKE_BUILD_TYPE=Release
	cd $(SOURCE_DIR) && cmake --build ../build

cross: clean
	python src/helpers/modify.py site/index.html
	python src/helpers/xxd.py index.modified.html

	mkdir build 2> NUL || exit 0
	del /f /q index.modified.html 2> NUL
	move /Y index.modified.c build

	cd $(SOURCE_DIR) && cmake -G "Ninja Multi-Config" -B ../build -S . -D CMAKE_TOOLCHAIN_FILE=cmake/toolchains/x86_64-w64-mingw32.cmake
	cd $(SOURCE_DIR) && cmake --build ../build --config Release

run:
	./build/bin/activator

clean:
ifeq ($(OS), Windows_NT)
	if exist build rmdir /s /q build
else
	if [ -d "build" ]; then rm -rf build; fi
endif