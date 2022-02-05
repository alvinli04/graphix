all: main.o utils.o utils.h
	g++ -o main main.o utils.o

main.o: main.cpp utils.h
	g++ -c main.cpp

utils.o: utils.cpp utils.h
	g++ -c utils.cpp

run:
	./main
	convert test.ppm test.png

clean:
	rm *.o
	rm *.ppm
	rm *.png
	rm main
