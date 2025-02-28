ifeq ($(OS),Windows_NT)
	SHELL = cmd
	MKDIR = if not exist build mkdir build
	RMDIR = if exist build rmdir /s /q build

	AUDIO_CMD = if not exist site/assets/audio/keygen-Uh-p3TOIrOc.mp3 yt-dlp -f bestaudio --extract-audio --audio-quality 64K --audio-format mp3 -o "site/assets/audio/keygen-Uh-p3TOIrOc.mp3" "https://www.youtube.com/watch?v=tPY-I3RX10c"

	DOWNLOAD_OHOOK_DLL = echo Skipping ohook download on Windows
	EXTRACT_OHOOK_DLL = echo Skipping ohook extraction on Windows
	INCLUDE_OHOOK_DLL = echo Skipping ohook inclusion on Windows

	# will be replaced by a helper
	# PACK_USING_UPX = if exist build\bin\activator.exe (where upx >nul 2>nul && upx --best build\bin\activator.exe || echo UPX not installed) else echo activator.exe not found
else
    MKDIR = mkdir -p build build/_deps/ohook/src/ohook/
    RMDIR = rm -rf build
	AUDIO_CMD = [ -f site/assets/audio/keygen-Uh-p3TOIrOc.mp3 ] || yt-dlp -f bestaudio --extract-audio --audio-quality 64K --audio-format mp3 -o "site/assets/audio/keygen-Uh-p3TOIrOc.mp3" "https://www.youtube.com/watch?v=tPY-I3RX10c"

	DOWNLOAD_OHOOK_DLL = curl -Lo build/ohook.zip https://github.com/asdcorp/ohook/releases/download/0.5/ohook_0.5.zip
	EXTRACT_OHOOK_DLL = unzip -o build/ohook.zip -d build/_deps/ohook/src/ohook/
	INCLUDE_OHOOK_DLL = python src/helpers/xxd.py -i build/_deps/ohook/src/ohook/sppc64.dll -o build/sppc64.dll.c

	# will be replaced by a helper
	# PACK_USING_UPX = if [ -f build/bin/Release/activator.exe ]; then if [ -x "$(command -v upx)" ]; then upx --best build/bin/Release/activator; else echo "UPX not installed"; fi; else echo "activator.exe not found, likely not a cross build"; fi
endif

all: build

prepare:
	$(MKDIR)
	$(AUDIO_CMD)

	$(DOWNLOAD_OHOOK_DLL)
	$(EXTRACT_OHOOK_DLL)
	$(INCLUDE_OHOOK_DLL)

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
	./build/bin/activator

clean:
	$(RMDIR)
