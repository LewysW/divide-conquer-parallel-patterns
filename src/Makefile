all: clean dac

dac:
	g++ main.cpp Worker.cpp -g -std=c++17 -lpthread -o dac	

clean:
	rm -rf dac *.o
