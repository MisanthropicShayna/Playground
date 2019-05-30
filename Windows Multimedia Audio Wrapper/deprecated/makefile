cmd = clang++
lcm = llvm-ar
arg = -std=c++17 -lwinmm.lib -m32
bin = wmaw.lib

wmaw.o: wmaw.cc headers/wmaw.hh
	$(cmd) -c wmaw.cc $(arg)

build: *.o
	$(lcm) rcs $(bin) *.o

clean:
	rm -f *.o *.a $(bin)