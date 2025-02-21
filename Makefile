ifeq ($(OS), Windows_NT)
    RM = del /f /q || exit 0
else
    RM = rm -rf
endif

all:
	# The HTML assets should be appended afterwards to the html.c file,
	# rather than being directly embedded as base64 inside the index.html
	xxd -i site/index.html > html.c

	cmake -G Ninja -B build -S . -D CMAKE_BUILD_TYPE=Release
	cmake --build build

run:
	./build/bin/activator

clean:
	$(RM) -rf build html.c
