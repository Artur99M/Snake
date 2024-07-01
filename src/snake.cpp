#include "../include/snake.hpp"
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <iostream>
using std::cerr;
using std::endl;
#ifdef DEBUG
#define DEBUG(); cerr << __func__ << ": head = {" << head.x << ", " << head.y << "}, finish = " << finish << ", dir = " << dir << ", fruit = {" << fruit.x << ", " << fruit.y << "}, tail_len = " << tail_len << ", MAX = " << MAX << ", tail:" << endl; \
        for (int j = 0; j < tail_len; j++) \
            cerr << "{" << tail[j].x << ", " << tail[j].y << "} "; \
            cerr << endl;
#else
#define DEBUG()
#endif

namespace snake
{
    Snake::Snake ()
    {
        std::ifstream infile;
        infile.open("snake.txt");
        infile >> MAX;
        infile.close();
        tail_len = 1;
        srand (time(NULL));
        head.x = MAX_X / 2;
        head.y = MAX_Y / 2;
        dir = 'd';
        tail = new poz;
        tail->x = head.x - 1;
        tail->y = head.y;
        finish = 0;
        speed = 400000;
        refruit();
        DEBUG();
    }

    Snake::~Snake ()
    {
        delete[] tail;
        if (tail_len > MAX)
        {
            std::ofstream outfile;
            outfile.open("snake.txt");
            outfile << MAX;
            outfile.close();
        }
    }

    void Snake::play()
    {
        initscr();
        cbreak();
        noecho();
        printw ("TAP ANOTHER KEY FOR START");
        long q = speed;
        raw();
        getch();
        noraw();
        clear();
        win = newwin (MAX_Y + 1, MAX_X + 1, 2, 2);
        box (win, 0, 0);
        wrefresh (win);
        while (!finish)
        {
            clear();
            print();
            if (dir == 'a' || dir == 'd')
                usleep (q);
            else usleep(5 * q / 3);
            step();
        }
        clear ();

        endwin();
    }

    void Snake::step()
    {
        timeout (1);
        char x = getch();
        if (x == 'w' || x == 'a' || x == 's' || x == 'd')
            switch (x)
            {
                case 'w':
                    if (dir != 's') dir = 'w';
                    break;
                case 's':
                    if (dir != 'w') dir = 's';
                    break;
                case 'a':
                    if (dir != 'd') dir = 'a';
                    break;
                case 'd':
                    if (dir != 'a') dir = 'd';
            }

        DEBUG();

        poz old_head = head;
        switch(dir)
        {
            case 'w':
                head.y = (head.y - 1) % MAX_Y;
                if (head.y == 0) head.y = MAX_Y - 1;
                break;
            case 'a':
                head.x = (head.x - 1) % MAX_X;
                break;
            case 's':
                head.y = (head.y + 1) % MAX_Y;
                if (head.y == 0) head.y = 1;
                break;
            case 'd':
                head.x = (head.x + 1) % MAX_X;
                if (head.x == 0) head.x++;
                break;
        }
        retail(old_head);
        if (head.x == fruit.x && head.y == fruit.y)
            refruit();

        for (int i = 0; i < tail_len; i++)
        {
            if(tail[i].x == head.x && tail[i].y == head.y)
            {
                finish = 1;
                return;
            }
        }
        DEBUG();

    }

    void Snake::print()
    {
        clear();
        box (win, 0, 0);
        for (int i = 1; i < MAX_X; i++)
            for (int j = 1; j < MAX_Y; j++)
                mvwprintw (win, j, i, " ");
        for (int i = 0; i < tail_len; i++)
            mvwprintw (win, tail[i].y, tail[i].x, "*");
        mvwprintw (win, head.y, head.x, "$");
        mvwprintw (win, fruit.y, fruit.x, "O");
        wrefresh(win);
    }

    void Snake::refruit()
    {
        int a = 1;
        while(a)
        {
            srand(time(NULL));
            fruit.x = rand() % MAX_X + 1;
            fruit.y = rand() % MAX_Y + 1;
            if (fruit.x == MAX_X || fruit.y == MAX_Y) continue;
            for (int i = 0; i < tail_len; i++)
                if (tail[i].x == fruit.x && tail[i].y == fruit.y)
                    { a = 2; break; }
            if (a == 2) {a = 1; continue;}

            if (head.x != fruit.x && head.y != fruit.y)
                a = 0;
        }
        DEBUG();
    }

    void Snake::retail(const poz & old_head)
    {
        int a = 0;
        for (int i = 0; i < tail_len; i++)
            if (head.x == fruit.x && head.y == fruit.y) a = 1;
        if (a)
        {
            poz* new_tail = new poz[tail_len + 1];
            memcpy(new_tail, tail, tail_len * sizeof (poz));
            new_tail[tail_len] = old_head;
            delete[] tail;
            tail = new_tail;
            tail_len++;
            DEBUG();
        }
        else
        {
            for (int i = 0; i < tail_len - 1; i++)
                tail[i] = tail[i+1];

            tail[tail_len - 1] = old_head;
            DEBUG();
        }


    }
}
