all:
	cmake -S . -B build
	cmake --build build

run : 
	./build/AoC

release:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build