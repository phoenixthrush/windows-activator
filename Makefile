ifeq ($(OS), Windows_NT)
	RM = del /f /q
else
	RM = rm -rf
endif

all:
	xxd -i site/index.html > html.c
	cmake -G Ninja -B build -S . -D CMAKE_BUILD_TYPE=Release
	cmake --build build

run:
	./build/bin/example

clean:
	$(RM) -rf build html.c
