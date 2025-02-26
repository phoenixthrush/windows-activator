ifeq ($(OS),Windows_NT)
	SHELL = cmd
	MKDIR = if not exist build mkdir build
	RMDIR = if exist build rmdir /s /q build

	AUDIO_CMD = if not exist site/assets/audio/keygen-Uh-p3TOIrOc.mp3 yt-dlp -f bestaudio --extract-audio --audio-quality 64K --audio-format mp3 -o "site/assets/audio/keygen-Uh-p3TOIrOc.mp3" "https://www.youtube.com/watch?v=tPY-I3RX10c"

	APPDATA_WINLIBS_DIR := $(APPDATA)/winlibs
	WINLIBS_X86_64 := $(APPDATA)/winlibs/winlibs-x86_64.zip
	WINLIBS_I686 := $(APPDATA)/winlibs/winlibs-i686.zip
	WINLIBS_URL_X86_64 := https://github.com/brechtsanders/winlibs_mingw/releases/download/14.2.0posix-19.1.7-12.0.0-msvcrt-r3/winlibs-x86_64-posix-seh-gcc-14.2.0-mingw-w64msvcrt-12.0.0-r3.zip
	WINLIBS_URL_I686 := https://github.com/brechtsanders/winlibs_mingw/releases/download/14.2.0posix-19.1.7-12.0.0-msvcrt-r3/winlibs-i686-posix-dwarf-gcc-14.2.0-mingw-w64msvcrt-12.0.0-r3.zip
	
	DOWNLOAD_WINLIBS = if not exist "$(APPDATA_WINLIBS_DIR)" ( \
		mkdir "$(APPDATA_WINLIBS_DIR)" && \
		curl -L $(WINLIBS_URL_X86_64) -o "$(WINLIBS_X86_64)" && \
		curl -L $(WINLIBS_URL_I686) -o "$(WINLIBS_I686)" \
	)

	EXTRACT_WINLIBS = if not exist "$(APPDATA)/winlibs/mingw64" tar -xf "$(WINLIBS_X86_64)" -C "$(APPDATA_WINLIBS_DIR)" && \
	                  if not exist "$(APPDATA)/winlibs/mingw32" tar -xf "$(WINLIBS_I686)" -C "$(APPDATA_WINLIBS_DIR)"

	DOWNLOAD_OHOOK_DLL = echo Skipping ohook download
	EXTRACT_OHOOK_DLL = echo Skipping ohook extraction
	INCLUDE_OHOOK_DLL = echo Skipping ohook inclusion

else
    MKDIR = mkdir -p build build/_deps/ohook/src/ohook/
    RMDIR = rm -rf build
	AUDIO_CMD = [ -f site/assets/audio/keygen-Uh-p3TOIrOc.mp3 ] || yt-dlp -f bestaudio --extract-audio --audio-quality 64K --audio-format mp3 -o "site/assets/audio/keygen-Uh-p3TOIrOc.mp3" "https://www.youtube.com/watch?v=tPY-I3RX10c"
	DOWNLOAD_WINLIBS = echo "Skipping MinGW-w64 winlibs download"
	EXTRACT_WINLIBS = echo "Skipping MinGW-w64 winlibs extraction"

	DOWNLOAD_OHOOK_DLL = curl -Lo build/ohook.zip https://github.com/asdcorp/ohook/releases/download/0.5/ohook_0.5.zip
	EXTRACT_OHOOK_DLL = unzip build/ohook.zip -d build/_deps/ohook/src/ohook/
	INCLUDE_OHOOK_DLL = python src/helpers/xxd.py -i build/_deps/ohook/src/ohook/sppc64.dll -o build/sppc64.dll.c
endif

all: build

prepare:
	$(MKDIR)
	$(AUDIO_CMD)

	$(DOWNLOAD_WINLIBS)
	$(EXTRACT_WINLIBS)

	$(DOWNLOAD_OHOOK_DLL)
	$(EXTRACT_OHOOK_DLL)
	$(INCLUDE_OHOOK_DLL)

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
