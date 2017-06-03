GXX=g++ $(FLAGS)
FLAGS=-Wall -pedantic -Wextra -std=c++11

ifeq ($(input),)
	input =input.txt
endif

ifeq ($(output),)
	output =output.png
endif

build: simulator

simulator: Main.o FileLoader.o Gate.o Wire.o Calendar.o
	$(GXX) Main.o FileLoader.o Gate.o Wire.o Calendar.o -o ims

Main.o: Main.cpp
	$(GXX) -c Main.cpp -o Main.o

FileLoader.o: FileLoader.cpp
	$(GXX) -c FileLoader.cpp -o FileLoader.o

Gate.o: Gate.cpp
	$(GXX) -c Gate.cpp -o Gate.o 

Wire.o: Wire.cpp
	$(GXX) -c Wire.cpp -o Wire.o

Calendar.o : Calendar.cpp
	$(GXX) -c Calendar.cpp -o Calendar.o

clean:
	rm *.o ims data.dat script.gp *.png
	 	
run:
	@./ims $(input) $(output)
	@gnuplot script.gp

kombinacni:
	@./ims kombinacni.txt kombinacni.png
	@gnuplot script.gp
	
sekvencni:
	@./ims sekvencni.txt sekvencni.png
	@gnuplot script.gp