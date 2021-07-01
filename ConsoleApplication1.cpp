// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件


#include "stdafx.h"
using namespace sf;

int N = 60, M = 40;
int size = 16;
int w = size * N;
int h = size * M;

int dir, num = 2;

struct Snake
{
    int x, y;
}  s[100];

struct Fruit
{
    int x, y;
} f;

void Tick()
{
    for (int i = num; i > 0; --i)
    {
        s[i].x = s[i - 1].x; s[i].y = s[i - 1].y;
    }
    if (dir == 0) s[0].y += 1;
    if (dir == 1) s[0].x -= 1;
    if (dir == 2) s[0].x += 1;
    if (dir == 3) s[0].y -= 1;

    if ((s[0].x == f.x) && (s[0].y == f.y))
    {
        num++; f.x = rand() % N; f.y = rand() % M;
    }

    if (s[0].x > N) s[0].x = 0;  if (s[0].x < 0) s[0].x = N;
    if (s[0].y > M) s[0].y = 0;  if (s[0].y < 0) s[0].y = M;

    for (int i = 1; i < num; i++)
        if (s[0].x == s[i].x && s[0].y == s[i].y)  num = i;
}

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(w, h), "Snack of Sea-Mountain");

    Texture t1, t2;
    if(!(t1.loadFromFile("images/white.png") && t2.loadFromFile("images/red.png")))
    {
        Image t0, t;
        t0.create(16, 16, sf::Color::Red);
        t.create(16, 16, sf::Color::White);
        t1.loadFromImage(t);
        t2.loadFromImage(t0);
    }
    sf::Music music;
    if (!music.openFromFile("hellmusic.ogg"))
        return -1; // error
    music.play();
    //sf::CircleShape shape(8.f);
    //shape.setFillColor(sf::Color::White);
    Sprite sprite1(t1);
    Sprite sprite2(t2);

    Clock clock;
    float timer = 0, delay = 0.1;

    f.x = 10,f.y = 10;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)&&dir!=2) dir = 1;
        if (Keyboard::isKeyPressed(Keyboard::Right)&&dir != 1) dir = 2;
        if (Keyboard::isKeyPressed(Keyboard::Up) && dir != 0) dir = 3;
        if (Keyboard::isKeyPressed(Keyboard::Down) && dir != 3) dir = 0;

        if (timer > delay) { timer = 0; Tick(); }

        ////// draw  ///////
        window.clear();      
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
            {
                sprite1.setPosition(i * size, j * size);  window.draw(sprite1);
            }
        for (int i = 0; i < num; i++)
        {
            sprite2.setPosition(s[i].x * size, s[i].y * size);  window.draw(sprite2);
        }
        sprite2.setPosition(f.x * size, f.y * size);  window.draw(sprite2);
        window.display();
    }
    return 0;
}




// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
