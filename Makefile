all: wBeing

wBeing: main.cpp
	g++ main.cpp -o wBeing `pkg-config --libs --cflags gtk+-2.0` -O3

clean:
	rm ./wBeing
