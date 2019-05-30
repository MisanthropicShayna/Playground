compiler := clang++
packager := llvm-ar
arguments := -std=c++17 -m32
libraries := winmm.lib
binary = wmmaw.lib

binary/$(binary): objects/*.o
	llvm-ar rcs binary/$(binary) objects/*.o $(libraries)

objects/wmmaw.o: wmmaw.cc headers/wmmaw.hh
	$(compiler) -c wmmaw.cc -o objects/wmmaw.o $(arguments)