CC = g++
ALLOBJ = obj/main.o obj/snake.o


all: $(ALLOBJ) app


obj/main.o: include/snake.hpp src/main.cpp
	$(CC) -c -lncurses src/main.cpp -o obj/main.o

obj/snake.o: include/snake.hpp src/snake.cpp
	$(CC) -c -lncurses src/snake.cpp -o obj/snake.o

app: $(ALLOBJ)
	$(CC) -lncurses $(ALLOBJ) -o snake

deb:
	$(CC) -DDEBUG -lncurses src/main.cpp src/snake.cpp -o snake
	./snake 2> txts/a.txt

clean:
	rm obj/*.o txts/a.txt snake

