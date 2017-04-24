make:
	g++ -c main.cpp
	make link

link:
	g++ -o sfml-app main.o -lsfml-system -lsfml-window -lsfml-graphics
	make clean

clean:
	rm main.o
