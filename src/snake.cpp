#include "../include/snake.hpp"
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <cctype>
using std::cerr;
using std::endl;
#ifdef DEBUG
#define DEBUG() cerr << __func__ << ": head = {" << head.x << ", " << head.y << "}, finish = " << finish << ", dir = " << dir << ", fruit = {" << fruit.x << ", " << fruit.y << "}, tail_len = " << tail_len << ", MAX[level] = " << MAX[level] << ", tail:" << endl; \
        for (int j = 0; j < tail_len; j++) \
            cerr << "{" << tail[j].x << ", " << tail[j].y << "} "; \
            cerr << endl
#else
#define DEBUG()
#endif

namespace snake
{
    Snake::Snake ()
    {
        std::ifstream infile;
        infile.open("txts/snake.txt");
        for (int i = 0; i < 10; i++)
            infile >> MAX[i];
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
        speed = 320000;
        level = 1;
        refruit();
        DEBUG();
    }

    Snake::~Snake ()
    {
        delete[] tail;

        std::ofstream outfile;
        outfile.open("txts/snake.txt");
        for (int i = 0; i < 10; i++)
        {
            if ((i == level) && (MAX[level] < tail_len))
                outfile << tail_len << ' ';
            else outfile << MAX[level] << ' ';
        }
        outfile.close();
    }

    void Snake::play()
    {
        initscr();
        cbreak();
        noecho();
        mvprintw (1 , 1, "   =====");
        mvprintw (2 , 1, " //     \\\\");
        mvprintw (3 , 1, "||       ||");
        mvprintw (4 , 1, "||");
        mvprintw (5 , 1, " \\\\");
        mvprintw (6 , 1, "   \\\\");
        mvprintw (7 , 1, "     \\\\");
        mvprintw (8 , 1, "       \\\\");
        mvprintw (9 , 1, "         ||");
        mvprintw (10, 1, "||       ||");
        mvprintw (11, 1, " \\\\     //");
        mvprintw (12, 1, "   =====");

        mvprintw (1 , 14, "||       ||");
        mvprintw (2 , 14, "||       ||");
        mvprintw (3 , 14, "||       ||");
        mvprintw (4 , 14, "||\\\\     ||");
        mvprintw (5 , 14, "|| \\\\    ||");
        mvprintw (6 , 14, "||  \\\\   ||");
        mvprintw (7 , 14, "||   \\\\  ||");
        mvprintw (8 , 14, "||    \\\\ ||");
        mvprintw (9 , 14, "||     \\\\||");
        mvprintw (10, 14, "||       ||");
        mvprintw (11, 14, "||       ||");
        mvprintw (12, 14, "||       ||");

        mvprintw (1 , 27, "    //\\\\       ");
        mvprintw (2 , 27, "   //  \\\\     ");
        mvprintw (3 , 27, "  //    \\\\  ");
        mvprintw (4 , 27, " //      \\\\");
        mvprintw (5 , 27, "||        ||");
        mvprintw (6 , 27, "||        ||");
        mvprintw (7 , 27, "||        ||");
        mvprintw (8 , 27, "||========||");
        mvprintw (9 , 27, "||        ||");
        mvprintw (10, 27, "||        ||");
        mvprintw (11, 27, "||        ||");
        mvprintw (12, 27, "||        ||");

        mvprintw (1 , 41, "||     //  ");
        mvprintw (2 , 41, "||    //   ");
        mvprintw (3 , 41, "||   //    ");
        mvprintw (4 , 41, "||  //");
        mvprintw (5 , 41, "|| //");
        mvprintw (6 , 41, "||//  ");
        mvprintw (7 , 41, "||\\\\   ");
        mvprintw (8 , 41, "|| \\\\   ");
        mvprintw (9 , 41, "||  \\\\   ");
        mvprintw (10, 41, "||   \\\\    ");
        mvprintw (11, 41, "||    \\\\   ");
        mvprintw (12, 41, "||     \\\\  ");

        mvprintw (1 , 52, "||==========");
        mvprintw (2 , 52, "||       ");
        mvprintw (3 , 52, "||       ");
        mvprintw (4 , 52, "||  ");
        mvprintw (5 , 52, "|| ");
        mvprintw (6 , 52, "||  ");
        mvprintw (7 , 52, "||=======   ");
        mvprintw (8 , 52, "||    ");
        mvprintw (9 , 52, "||     ");
        mvprintw (10, 52, "||       ");
        mvprintw (11, 52, "||       ");
        mvprintw (12, 52, "||==========");


        mvprintw (16, 20, "TAP ANOTHER KEY FOR START");
        mvprintw (17, 10, "IF YOU WANT PLAY WITH HIGH LEVEL TAP ON NUMBER");
        raw();
        int y = getch();
        long q = speed / level;
        if (isnumber(y))
        {
            if (y == '0') q /= 10;
            else q /= (y - '0');
            level = y - '0';
        }
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
        x = tolower (x);
        if (x == 'q') finish = 1;
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
                if (head.x == 0) head.x = MAX_X - 1;
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
        mvprintw (MAX_Y + 4, 1, "  YOUR RESULT: %d            BEST RESULT: %d", tail_len, MAX[level]);
        mvprintw (MAX_Y + 6, 1, "  TAP \'Q\' FOR END");
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
