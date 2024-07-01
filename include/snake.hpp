#ifndef SNAKE_HPP_
#define SNAKE_HPP_

#include <ncurses.h>

namespace snake
{
    struct poz
    {
        int x;
        int y;
    };

    class Snake
    {
        private:
        enum {MAX_X = 100, MAX_Y = 30};
        poz* tail;
        int tail_len;
        poz head;
        poz fruit;
        WINDOW* win;
        int finish;
        int MAX[10];
        char dir;
        long speed;
        int level;
        public:
        Snake();
        ~Snake();
        void play();
        void print();
        void step();
        void retail(const poz & old_head);
        void refruit();
        int is_finish() const { return finish; }
        // const Snake & operator++();

    };
}
#endif
