//
// Created by lenovo on 2021/12/26.
//
#include <windows.h> // 必须
#include <conio.h>   // 必须
#include <stdio.h>
#include <time.h>
HANDLE consoleHandle; // 操作控制台（也就是那个黑框框）需要的一个变量
int w, h;
int lastmove, last_lastmove, food_x, food_y;
int x[200], y[200], score;
int len, eaten, mode;
//打印字符
void mvaddch(int y, int x, char ch)
{
    COORD co = (COORD){.X = x, .Y = y};
    SetConsoleCursorPosition(consoleHandle, co);
    putchar(ch);
}
//打印字符串
void mvaddstr(int y, int x, char *s)
{
    COORD co = (COORD){.X = x, .Y = y};
    SetConsoleCursorPosition(consoleHandle, co); // 设置你的光标位置
    printf("%s\n", s);                           // 在这里打印一个字符串
}
//打印数字
void mvaddnum(int y, int x, int n)
{
    COORD co = (COORD){.X = x, .Y = y};
    SetConsoleCursorPosition(consoleHandle, co);
    printf("%d  ", n);
}
//打印封面
int Menu()
{
    system("cls");
    mvaddstr(h / 2 + 30, w / 2 + 10, "    \n");
    printf("                                                                       \n"
           "   d888888o.   b.             8          .8.          8 8888     ,88' 8 8888888888\n"
           " .`8888:' `88. 888o.          8         .888.         8 8888    ,88'  8 8888\n"
           " 8.`8888.   Y8 Y88888o.       8        :88888.        8 8888   ,88'   8 8888\n"
           " `8.`8888.     .`Y888888o.    8       . `88888.       8 8888  ,88'    8 8888\n"
           "  `8.`8888.    8o. `Y888888o. 8      .8. `88888.      8 8888 ,88'     8 888888888888\n"
           "   `8.`8888.   8`Y8o. `Y88888o8     .8`8. `88888.     8 8888 88'      8 8888\n"
           "    `8.`8888.  8   `Y8o. `Y8888    .8' `8. `88888.    8 888888<       8 8888\n"
           "8b   `8.`8888. 8      `Y8o. `Y8   .8'   `8. `88888.   8 8888 `Y8.     8 8888\n"
           "`8b.  ;8.`8888 8         `Y8o.`  .888888888. `88888.  8 8888   `Y8.   8 8888\n"
           " `Y8888P ,88P' 8            `Yo .8'       `8. `88888. 8 8888     `Y8. 8 888888888888");
    mvaddstr(2 / h + 30, 0, "Please press Enter to explore...");
    while (1)
    {
        int flag = 0;
        while (kbhit() != 0)
        {
            char ch = getchar();
            if (ch == '\n')
            {
                flag = 1;
                break;
            }
            if (ch == 'q')
            {
                my_exit();
                return 1;
                break;
            }
        }
        if (flag == 1)
            break;
    }
    Sleep(1000);

    char overlook1[25] = "**Gluttonous Snake**";
    char overlook2[20] = "Want to play?";
    char overlook3[30] = "Press i to start a normal mode";
    char overlook4[20] = "Game Of Fun";
    system("cls");
    mvaddstr(h / 2, w / 2 - 12, overlook1);
    mvaddstr(h / 2 + 2, w / 2 - 8, overlook2);
    mvaddstr(h / 2 + 4, w / 2 - 15, overlook3);
    mvaddstr(h / 2 - 2, w / 2 - 6, overlook4);
    mvaddstr(h / 2 + 5, w / 2 - 16, "Press o to start a strange mode");
    mvaddstr(h / 2 + 6, w / 2 - 8, "Press q to quit");

    eaten = 1, mode = 0;

    while (1)
    {
        int flag = 0;
        while (kbhit() != 0)
        {
            char ch = getch();
            if (ch == 'i')
            {
                system("cls");
                flag = 1;
                break;
            }
            if (ch == 'o')
            {
                system("cls");
                mode = 1;
                flag = 1;
                break;
            }
            if (ch == 'q')
            {
                my_exit();
                return 1;
            }
        }
        if (flag == 1)
            break;
    }
    Sleep(1000);
    background();
    SnakeInit();
    return 0;
}
void background()
{
    char c = '#';
    for (int i = 0; i < w - 1; i++)
    {
        mvaddch(h - 1, i, c);
        mvaddch(0, i, c);
    }

    for (int i = 1; i < h - 1; i++)
    {
        mvaddch(i, 0, c);
        mvaddch(i, w - 1, c);
        mvaddch(i, 2 * w / 3, c);
    }
    mvaddstr(h / 4, 3 * w / 4, "   ____               __\n");
    mvaddstr(h / 4 + 1, 3 * w / 4, "  / __/  ___  ___ _  / /__ ___\n");
    mvaddstr(h / 4 + 2, 3 * w / 4, " _\\ \\   / _ \\/ _ `/ /  '_// -_)\n");
    mvaddstr(h / 4 + 3, 3 * w / 4, "/___/  /_//_/\\_,_/ /_/\\_\\ \\__/\n");
    mvaddstr(h / 2 - 4, 3 * w / 4, "Score:");
    mvaddstr(h / 2 + 3, 3 * w / 4, "Press p to pause the game");
    mvaddstr(h / 2 + 4, 3 * w / 4, "Press p again to continue");
    mvaddstr(h / 2 + 6, 3 * w / 4, "Press b to return to the menu");
    mvaddstr(h / 2 + 8, 3 * w / 4, "Press q to quit");
    mvaddstr(h / 2 - 2, 3 * w / 4, "w : up");
    mvaddstr(h / 2 - 1, 3 * w / 4, "s : down");
    mvaddstr(h / 2, 3 * w / 4, "a : left");
    mvaddstr(h / 2 + 1, 3 * w / 4, "d : right");

    mvaddnum(h / 2 - 4, 3 * w / 4 + 8, score);
}

void SnakeInit()
{
    score = 0;
    len = 3;
    eaten = 1;
    last_lastmove = 0;
    lastmove = 1;
    int i = len;
    while (i--)
    {
        y[i] = h / 2;
        x[i] = w / 3 - i;
    }
    SnakePrint();
    Sleep(500);
}

void SnakeMove(int move, int lastmove)
{
    mvaddch(y[len - 1], x[len - 1], ' ');
    for (int i = len - 1; i > 0; i--)
    {
        x[i] = x[i - 1];
        y[i] = y[i - 1];
    }
    if (move == 1)
        y[0]--;
    if (move == 2)
        y[0]++;
    if (move == 3)
        x[0]--;
    if (move == 4)
        x[0]++;
    SnakePrint();
    if (score > 15 || lastmove == move)
        Sleep(50);
    else
        Sleep(300 - 50 * (score / 3));
}

void SnakePrint()
{
    for (int i = 0; i < len; i++)
    {
        if (i == 0)
            mvaddch(y[i], x[i], '&');
        else
            mvaddch(y[i], x[i], 'o');
    }
}

int GameOver()
{
    int flag1 = 0;
    for (int i = 1; i < len; i++)
    {
        if (x[0] == x[i] && y[0] == y[i])
        {
            flag1 = 1;
            break;
        }
    }
    if (x[0] == 0 || x[0] == 2 * w / 3 || y[0] == 0 || y[0] == h - 1)
    {
        flag1 = 1;
    }
    if (flag1 == 1)
    {
        system("cls");
        mvaddstr(h / 2 - 4, w / 2 - 4, "Game Over!");
        mvaddstr(h / 2 - 2, w / 2 - 6, "Your score is:");
        mvaddnum(h / 2 - 2, w / 2 + 8, score);
        mvaddstr(h / 2, w / 2 - 12, "Press Space to return to menu");
        mvaddstr(h / 2 + 2, w / 2 - 7, "Press q to quit");
    }
    return flag1;
}
void my_exit()
{
    system("cls");
    mvaddstr(h / 2 - 5, w / 2 - 13, "What does the snake say?");
    Sleep(1000);
    mvaddstr(h / 2, w / 2 - 13, "\"Speed is not everything.");
    Sleep(1000);
    mvaddstr(h / 2 + 2, w / 2 - 19, "Fast speed comes with extreme danger.");
    Sleep(1000);
    mvaddstr(h / 2 + 4, w / 2 - 18, "Watch out when you enjoy the speed!\"");
    Sleep(1500);
}
int main()
{
    system("color 71");
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);  // 初始化这个操作器
    CONSOLE_SCREEN_BUFFER_INFO csbi;                  // 屏幕的信息
    GetConsoleScreenBufferInfo(consoleHandle, &csbi); // 获取屏幕信息
    w = csbi.dwSize.X;
    h = csbi.dwSize.Y; // 得到宽度高度

    CONSOLE_CURSOR_INFO cci; // 光标信息
    cci.dwSize = 100;
    cci.bVisible = FALSE;                      // 不可见
    SetConsoleCursorInfo(consoleHandle, &cci); // 将光标特性应用到控制台
    // 到这里，闪烁的光标就消失了。

    //标题界面
    if (Menu())
        return 0;

    while (1)
    {
        background();
        int record = 0;
        SnakeMove(lastmove, last_lastmove);
        record = lastmove;
        while (kbhit() != 0)
        {
            char ch = getch();
            last_lastmove = lastmove;     //记录最近两步
            if (ch == 'w' && record != 2) //蛇不能180度转身
                lastmove = 1;             //向上
            if (ch == 'a' && record != 4)
                lastmove = 3; //向左
            if (ch == 's' && record != 1)
                lastmove = 2; //向下
            if (ch == 'd' && record != 3)
                lastmove = 4; //向右
            if (ch == 'p')
            {
                while (1)
                {
                    int flag2 = 0;
                    while (kbhit() != 0)
                    {
                        char c = getch();
                        if (c == 'p')
                        {
                            flag2 = 1;
                            break;
                        }
                        if (c == 'q')
                        {
                            my_exit();
                            return 0;
                        }
                        if (c == 'b')
                        {
                            if (Menu())
                                return 0;
                            flag2 = 1;
                            break;
                        }
                    }
                    if (flag2 == 1)
                    {
                        break;
                    }
                }
            }
            if (ch == 'b')
            {
                if (Menu())
                    return 0;
                break;
            }
            if (ch == 'q')
            {
                my_exit();
                return 0;
            }
        }
        if (eaten == 1)
        {
            srand((unsigned)time(NULL));
            food_x = rand() % (2 * w / 3 - 1) + 1;
            food_y = rand() % (h - 2) + 1;
            eaten = 0;
        }
        mvaddch(food_y, food_x, '$');
        //吃到食物
        if (x[0] == food_x && y[0] == food_y)
        {
            eaten = 1;
            score++;
            if (mode == 0)
            {
                if (x[len - 1] == x[len - 2])
                {
                    x[len] = x[len - 1];
                    y[len] = 2 * y[len - 1] - y[len - 2];
                }
                else
                {
                    x[len] = 2 * x[len - 1] - x[len - 2];
                    y[len] = y[len - 1];
                }
                len++;
            }
            else if (mode == 1)
            {
                if (x[len - 1] == x[len - 2])
                {
                    for (int l = 0; l < 10; l++)
                    {
                        x[len + l] = x[len - 1];
                        y[len + l] = 2 * y[len - 1 + l] - y[len - 2 + l];
                    }
                }
                else
                {
                    for (int l = 0; l < 10; l++)
                    {
                        x[len + l] = 2 * x[len - 1 + l] - x[len - 2 + l];
                        y[len + l] = y[len - 1];
                    }
                }
                len += 10;
            }
        }
        //游戏结束
        if (GameOver())
        {
            while (1)
            {
                int flag3 = 1;
                while (kbhit() != 0)
                {
                    char c = getch();
                    if (c == ' ')
                    {
                        if (Menu())
                            return 0;
                        flag3 = 0;
                        break;
                    }
                    if (c == 'q')
                    {
                        my_exit();
                        return 0;
                    }
                }
                if (flag3 == 0)
                    break;
            }
        }
    }
    cci.bVisible = TRUE;                       // 可见
    SetConsoleCursorInfo(consoleHandle, &cci); // 重新设置
    return 0;
}
