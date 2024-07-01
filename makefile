CC = g++
ALLOBJ = obj/main.o obj/snake.o


all: $(ALLOBJ) app


obj/main.o: include/snake.hpp src/main.cpp
	$(CC) -c -lncurses src/main.cpp -o obj/main.o

obj/snake.o: include/snake.hpp src/snake.cpp
	$(CC) -c -lncurses src/snake.cpp -o obj/snake.o

app:
	$(CC) -lncurses $(ALLOBJ) -o snake

snake:
	./snake

deb:
	$(CC) -lncurses -DDEBUG src/main.cpp src/snake.cpp -o snake
	./snake 2> txts/a.txt

clean:
	rm obj/*.o txts/a.txt

