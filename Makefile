ifeq ($(OS), Windows_NT)
    RM = del /f /q || exit 0
else
    RM = rm -rf
endif

all:
	python helpers/modify.py site/index.html

	# This should likely be replaced by
	# embedding the raw data directly into the binary instead of using xxd
	python helpers/xxd.py index.modified.html

	cmake -G Ninja -B build -S . -D CMAKE_BUILD_TYPE=Release
	cmake --build build

run:
	./build/bin/activator

clean:
	$(RM) -rf build index.modified.html index.modified.c
