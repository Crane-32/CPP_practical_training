#include "stdafx.h"
#include <graphics.h>
#include "mmsystem.h"
#pragma comment (lib,"winmm.lib")
#include "Vfw.h"
#pragma comment (lib, "Vfw32.lib")
using namespace sf;
const int W = 1920;
const int H = 1080;
const int sleep1 = 6000;//10
const int sleep2 = 600;//100
const int sleep3 = 400;//150
const int sleep4 = 300;//200
const int sleep5 = 250;//240
const int sleep6 = 120;//500
const int sleep7 = 100;//600
const int sleep8 = 60;//1000
const int sleep9 = 30;//2000
const int radi1 = 400;
const int radi2 = 100;
const int radi3 = 80;//c57
const int radi4 = 50;
const int radi5 = 66;
const int radi6 = 20;
const int radi7 = 46;
const int radi8 = 10;
const int radi9 = 8;
float DEGTORAD = 0.017453f;
unsigned int mark1 = 0;
unsigned int life1 = 5;
unsigned int magic1 = 6;
unsigned int kill1 = 0;
float tx, ty, tdx, tdy, ta;
int tr, rank;
struct refile
{
    int mark0;
    int life0;
    int magic0;
    int kill0;
    float tx0;
    float ty0;
    float tdx0;
    float tdy0;
    float ta0;
    int tr0;
    int rank0;
    int num;
};
class Animation
{
public:
    float Frame, speed;
    Sprite sprite;
    std::vector<IntRect> frames;
    Animation() {}
    Animation(Texture& t, int x, int y, int w, int h, int count, float Speed)
    {
        Frame = 0;
        speed = Speed;

        for (int i = 0; i < count; i++)
            frames.push_back(IntRect(x + i * w, y, w, h));

        sprite.setTexture(t);
        sprite.setOrigin(w / 2, h / 2);
        sprite.setTextureRect(frames[0]);
    }

    void update()
    {
        Frame += speed;
        int n = frames.size();
        if (Frame >= n) Frame -= n;
        if (n > 0) sprite.setTextureRect(frames[int(Frame)]);
    }

    bool isEnd()
    {
        return Frame + speed >= frames.size();
    }

};
class Entity
{
public:
    float x, y, dx, dy, R, angle;
    bool life;
    std::string name;
    Animation anim;
    Entity()
    {
        life = 1;
    }
    void settings(Animation& a, int X, int Y, float Angle = 0, int radius = 1)
    {
        anim = a;
        x = X; y = Y;
        angle = Angle;
        R = radius;
    }
    virtual void update() {};
    void draw(RenderWindow& app)
    {
        anim.sprite.setPosition(x, y);
        anim.sprite.setRotation(angle + 90);
        app.draw(anim.sprite);
        CircleShape circle(R);
        circle.setFillColor(Color(255, 0, 0, 170));
        circle.setPosition(x, y);
        circle.setOrigin(R, R);
        //app.draw(circle);
    }
    virtual ~Entity() {};
};
class asteroid : public Entity
{
public:
    asteroid()
    {
        dx = rand() % 8 - 4;
        dy = rand() % 8 - 4;
        name = "asteroid";
    }
    void update()
    {
        x += dx;
        y += dy;
        if (x > W) x = 0;  if (x < 0) x = W;
        if (y > H) y = 0;  if (y < 0) y = H;
    }
};
class bullet : public Entity
{
public:
    bullet()
    {
        name = "bullet";
    }
    void  update()
    {
        dx = cos(angle * DEGTORAD) * 4;
        dy = sin(angle * DEGTORAD) * 4;
        x += dx;
        y += dy;
        if (x > W || x<0 || y>H || y < 0) life = 0;
    }
};
class bullet2 : public Entity
{
public:
    bullet2()
    {
        name = "bullet2";
    }
    void  update()
    {
        dx = cos(angle * DEGTORAD) * 10;
        dy = sin(angle * DEGTORAD) * 10;
        angle += rand() % 7 - 3;  /*try this*/
        x += dx;
        y += dy;
        if (x > W || x<0 || y>H || y < 0) life = 0;
    }
};
class player : public Entity
{
public:
    bool thrust;
    player()
    {
        name = "player";
    }
    void update()
    {
        if (thrust)
        {
            dx += cos(angle * DEGTORAD) * 0.2;
            dy += sin(angle * DEGTORAD) * 0.2;
        }
        else
        {
            dx *= 0.99;
            dy *= 0.99;
        }
        int maxSpeed = 15;
        float speed = sqrt(dx * dx + dy * dy);
        if (speed > maxSpeed)
        {
            dx *= maxSpeed / speed;
            dy *= maxSpeed / speed;
        }
        x += dx;
        y += dy;
        if (x > W) x = 0; if (x < 0) x = W;
        if (y > H) y = 0; if (y < 0) y = H;
    }
};
bool isCollide(Entity* a, Entity* b)
{
    return (b->x - a->x) * (b->x - a->x) +
        (b->y - a->y) * (b->y - a->y) <
        (a->R + b->R) * (a->R + b->R);
}

class Button
{
public:
    RenderWindow* window;
    Texture sprite_name;
    SoundBuffer audio_name;
    int pos_x, pos_y, size_x, size_y;
    Sprite button;
    Sound button_music;
    Button(RenderWindow* w) :window(w) {};
    void setButtonSize(float x, float y)
    {
        size_x = x; size_y = y;
        Vector2u true_size;
        true_size = sprite_name.getSize();
        x = x / true_size.x;
        y = y / true_size.y;
        button.setScale(Vector2f(x, y));
    }
    void setButtonPos(int x, int y)
    {
        pos_x = x; pos_y = y;
        button.setPosition(x, y);
    }
    void setAudio(String s)
    {
        audio_name.loadFromFile(s);
        button_music.setBuffer(audio_name);
    }
    void setSprite(String s)
    {
        sprite_name.loadFromFile(s);
        sprite_name.setSmooth(true);
        button.setTexture(sprite_name);
    }
    int buttonPressed();
    void show()
    {
        (*window).draw(button);
    }
    void show(String s_sprite, String s_music, int x, int y, float w, float h)
    {
        setSprite(s_sprite);
        setAudio(s_music);
        setButtonSize(w, h);
        setButtonPos(x, y);
        (*window).draw(button);
    }
};
int Button::buttonPressed()
{
    Vector2i pos = Mouse::getPosition((*window));
    Event e;
    while ((*window).pollEvent(e))
    {
        if (e.type = Event::Closed)
        {
            (*window).close(); return -1;
        }
        if (e.type = Event::MouseButtonPressed)
            if (pos.x >= pos_x && pos.x <= pos_x + size_x && pos.y >= pos_y && pos.y <= pos_y + size_y)
                if (e.key.code == Mouse::Left)
                {
                    return 1;//单击左键
                }
                else if (e.key.code == Mouse::Right)return 3;//单击右键
                else return 0;
    }
}

int start()
{
    initgraph(1920, 1080);
    system("color 4");
    mciSendString(TEXT("open audio/socialism.mp3 alias mysong"), NULL, 0, NULL);
    mciSendString(TEXT("play MySong"), NULL, 0, NULL);
    loadimage(NULL, _T("images/main.jpg"), 0, 0, false);
    setbkmode(TRANSPARENT);
    int k;

    settextcolor(RGB(100, 149, 237));
    settextstyle(64, 0, _T("Consolas"));
    outtextxy(450, 40, _T("必将实现的道路——太阳系的共产主义未来"));
    settextcolor(RGB(198, 226, 255));
    settextstyle(40, 0, _T("Impact"));
    outtextxy(640, 140, _T("powered&developed by:门门挂科的山山"));
    settextstyle(40, 0, _T("Impact"));
    roundrect(320, 280, 640, 320, 5, 5);
    roundrect(320, 330, 640, 370, 5, 5);
    roundrect(320, 380, 640, 420, 5, 5);
    roundrect(320, 430, 640, 470, 5, 5);
    roundrect(320, 480, 640, 520, 5, 5);

    settextstyle(36, 0, _T("Consolas"), 0, 0, 0, false, false, false);
    outtextxy(320, 280, _T("开始新游戏"));
    outtextxy(320, 330, _T(" 读取存档"));
    outtextxy(320, 380, _T("  帮  助  "));
    outtextxy(320, 430, _T(" 武器介绍"));
    outtextxy(320, 480, _T("  退  出  "));
    settextcolor(RGB(100, 149, 237));

    while (1)
    {
        MOUSEMSG m = GetMouseMsg();
        settextcolor(RGB(255, 179, 166));
        if (m.x < 640 && m.x>320 && m.y < 320 && m.y>280)
        {           
            outtextxy(320, 280, _T("开始新游戏"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 1; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 370 && m.y>330)
        {           
            outtextxy(320, 330, _T(" 读取存档"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 2; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 420 && m.y>380)
        {
            outtextxy(320, 380, _T("  帮  助  "));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 3; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 470 && m.y>430)
        {
            outtextxy(320, 430, _T(" 武器介绍"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 4; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 520 && m.y>480)
        {
            outtextxy(320, 480, _T("  退  出  "));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 5; break; }
        }
        else
        {
            settextcolor(RGB(100, 149, 237));
            outtextxy(320, 280, _T("开始新游戏"));
            outtextxy(320, 330, _T(" 读取存档"));
            outtextxy(320, 380, _T("  帮  助  "));
            outtextxy(320, 430, _T(" 武器介绍"));
            outtextxy(320, 480, _T("  退  出  "));
        }
        FlushMouseMsgBuffer();
    }
    mciSendString(TEXT("stop mysong"), NULL, 0, NULL);
    mciSendString(TEXT("close mysong"), NULL, 0, NULL);
    return k;
    closegraph();
}
int game_sle()

{
    initgraph(1920, 1080);
    system("color 4");
    loadimage(NULL, _T("images/main.jpg"), 0, 0, false);
    setbkmode(TRANSPARENT);
    int k;
    settextcolor(RGB(100, 149, 237));
    settextstyle(64, 0, _T("Consolas"));
    outtextxy(650, 40, _T("选择任务"));
    settextcolor(RGB(198, 226, 255));
    settextstyle(40, 0, _T("Impact"));
    outtextxy(640, 140, _T("powered&developed by:门门挂科的山山"));
    settextstyle(40, 0, _T("Impact"));
    roundrect(320, 280, 640, 320, 5, 5);
    roundrect(320, 330, 640, 370, 5, 5);
    roundrect(320, 380, 640, 420, 5, 5);
    roundrect(320, 430, 640, 470, 5, 5);
    roundrect(320, 480, 640, 520, 5, 5);
    roundrect(320, 530, 640, 570, 5, 5);
    roundrect(320, 580, 640, 620, 5, 5);
    roundrect(320, 630, 640, 670, 5, 5);
    roundrect(320, 680, 640, 720, 5, 5);
    settextstyle(36, 0, _T("Consolas"), 0, 0, 0, false, false, false);
    outtextxy(320, 280, _T("任务一"));
    outtextxy(320, 330, _T("任务二"));
    outtextxy(320, 380, _T("开发中"));
    outtextxy(320, 430, _T("开发中"));
    outtextxy(320, 480, _T("开发中"));
    outtextxy(320, 530, _T("开发中"));
    outtextxy(320, 580, _T("开发中"));
    outtextxy(320, 630, _T("开发中"));
    outtextxy(320, 680, _T("开发中"));
    settextcolor(RGB(100, 149, 237));
    while (1)
    {
        MOUSEMSG m = GetMouseMsg();
        settextcolor(RGB(255, 179, 166));
        if (m.x < 640 && m.x>320 && m.y < 320 && m.y>280)
        {
            outtextxy(320, 280, _T("任务一"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 1; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 370 && m.y>330)
        {
            outtextxy(320, 330, _T("任务二"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 2; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 420 && m.y>380)
        {
            outtextxy(320, 380, _T("开发中"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 3; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 470 && m.y>430)
        {
            outtextxy(320, 430, _T("开发中"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 4; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 520 && m.y>480)
        {
            outtextxy(320, 480, _T("开发中"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 5; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 570 && m.y>530)
        {
            outtextxy(320, 530, _T("开发中"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 6; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 620 && m.y>580)
        {
            outtextxy(320, 580, _T("开发中"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 7; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 670 && m.y>630)
        {
            outtextxy(320, 630, _T("开发中"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 8; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 720 && m.y>680)
        {
            outtextxy(320, 680, _T("开发中"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 9; break; }
        }
        else
        {
            settextcolor(RGB(100, 149, 237));
            outtextxy(320, 280, _T("任务一"));
            outtextxy(320, 330, _T("任务二"));
            outtextxy(320, 380, _T("开发中"));
            outtextxy(320, 430, _T("开发中"));
            outtextxy(320, 480, _T("开发中"));
            outtextxy(320, 530, _T("开发中"));
            outtextxy(320, 580, _T("开发中"));
            outtextxy(320, 630, _T("开发中"));
            outtextxy(320, 680, _T("开发中"));
        }
        FlushMouseMsgBuffer();
    } 
    //closegraph();
    return k;
}

int help()
{
    RenderWindow apph(VideoMode(W, H), L"天问之旅——游戏帮助");
    apph.setFramerateLimit(60);
    Texture t2;
    t2.loadFromFile("images/help.jpg"); t2.setSmooth(true);
    Sprite help(t2);
    sf::Music musich;
    if (!musich.openFromFile("audio/rising.ogg"))
        return -1;
    musich.play();
    while (apph.isOpen())
    {
        Event event;
        while (apph.pollEvent(event))
        {
            if (event.type == Event::Closed)
                apph.close();
            if (event.type == Event::MouseButtonPressed)
            {
                Vector2i pos;
                pos=Mouse::getPosition();
                if (pos.x >= 10 && pos.x <= 1910 && pos.y >= 10 && pos.y <= 1070)
                    if (event.key.code == Mouse::Left)
                    {
                        apph.close(); 
                        return 0;
                    }
            }
        }
        apph.draw(help);
        apph.display();
    }
}
int intro()
{
    RenderWindow apph(VideoMode(W, H), L"天问之旅——武器介绍");
    apph.setFramerateLimit(60);
    Texture t2;
    t2.loadFromFile("images/intro.jpg"); t2.setSmooth(true);
    Sprite help(t2);
    sf::Music musich;
    if (!musich.openFromFile("audio/rising.ogg"))
        return -1;
    musich.play();
    while (apph.isOpen())
    {
        Event event;
        while (apph.pollEvent(event))
        {
            if (event.type == Event::Closed)
                apph.close();
            if (event.type == Event::MouseButtonPressed)
            {
                Vector2i pos;
                pos = Mouse::getPosition();
                if (pos.x >= 10 && pos.x <= 1910 && pos.y >= 10 && pos.y <= 1070)
                    if (event.key.code == Mouse::Left)
                    {
                        apph.close();
                        return 0;
                    }
            }
        }
        apph.draw(help);
        apph.display();
    }
}
int docu(int mark, int life, int magic, int kill, float tx, float ty, float tdx, float tdy, float ta, int tr, int rank)
{
     initgraph(1920, 810);
    system("color 4");
    loadimage(NULL, _T("images/document.jpg"), 0, 0, false);
    setbkmode(TRANSPARENT);
    int k;
    settextcolor(RGB(100, 149, 237));
    settextstyle(64, 0, _T("Consolas"));
    outtextxy(720, 40, _T("选择保存位置"));
    settextcolor(RGB(198, 226, 255));
    settextstyle(40, 0, _T("Impact"));
    outtextxy(640, 140, _T("powered&developed by:门门挂科的山山"));
    settextstyle(40, 0, _T("Impact"));
    roundrect(320, 280, 640, 320, 5, 5);
    roundrect(320, 330, 640, 370, 5, 5);
    roundrect(320, 380, 640, 420, 5, 5);
    roundrect(320, 430, 640, 470, 5, 5);
    roundrect(320, 480, 640, 520, 5, 5);
    roundrect(320, 530, 640, 570, 5, 5);
    roundrect(320, 580, 640, 620, 5, 5);
    roundrect(320, 630, 640, 670, 5, 5);
    roundrect(320, 680, 640, 720, 5, 5);
    settextstyle(36, 0, _T("Consolas"), 0, 0, 0, false, false, false);
    outtextxy(320, 280, _T("存档一"));
    outtextxy(320, 330, _T("存档二"));
    outtextxy(320, 380, _T("存档三"));
    outtextxy(320, 430, _T("存档四"));
    outtextxy(320, 480, _T("存档五"));
    outtextxy(320, 530, _T("存档六"));
    outtextxy(320, 580, _T("存档七"));
    outtextxy(320, 630, _T("存档八"));
    outtextxy(320, 680, _T("存档九"));
    settextcolor(RGB(100, 149, 237));
    while (1)
    {
        MOUSEMSG m = GetMouseMsg();
        settextcolor(RGB(255, 179, 166));
        if (m.x < 640 && m.x>320 && m.y < 320 && m.y>280)
        {
            outtextxy(320, 280, _T("存档一"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 1; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 370 && m.y>330)
        {
            outtextxy(320, 330, _T("存档二"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 2; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 420 && m.y>380)
        {
            outtextxy(320, 380, _T("存档三"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 3; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 470 && m.y>430)
        {
            outtextxy(320, 430, _T("存档四"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 4; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 520 && m.y>480)
        {
            outtextxy(320, 480, _T("存档五"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 5; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 570 && m.y>530)
        {
            outtextxy(320, 530, _T("存档六"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 6; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 620 && m.y>580)
        {
            outtextxy(320, 580, _T("存档七"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 7; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 670 && m.y>630)
        {
            outtextxy(320, 630, _T("存档八"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 8; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 720 && m.y>680)
        {
            outtextxy(320, 680, _T("存档九"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 9; break; }
        }
        else
        {
            settextcolor(RGB(100, 149, 237));
            outtextxy(320, 280, _T("存档一"));
            outtextxy(320, 330, _T("存档二"));
            outtextxy(320, 380, _T("存档三"));
            outtextxy(320, 430, _T("存档四"));
            outtextxy(320, 480, _T("存档五"));
            outtextxy(320, 530, _T("存档六"));
            outtextxy(320, 580, _T("存档七"));
            outtextxy(320, 630, _T("存档八"));
            outtextxy(320, 680, _T("存档九"));
        }
        FlushMouseMsgBuffer();
    } 
    FILE* fps;
    std::string sc = "\docu" + std::to_string(k) + ".txt";
    fps = fopen(sc.c_str(), "w");
    fprintf(fps, "%d %d %d %d %f %f %f %f %f %d %d\n", mark, life, magic, kill, tx, ty, tdx, tdy, ta, tr, rank);
    fclose(fps);
    return k;
}
refile docu_read()
{  
     initgraph(1920, 810);
    system("color 4");
    loadimage(NULL, _T("images/docuread.jpg"), 0, 0, false);
    setbkmode(TRANSPARENT);
    int k;
    settextcolor(RGB(100, 149, 237));
    settextstyle(64, 0, _T("Consolas"));
    outtextxy(720, 40, _T("选择读取记录"));
    settextcolor(RGB(198, 226, 255));
    settextstyle(40, 0, _T("Impact"));
    outtextxy(640, 140, _T("powered&developed by:门门挂科的山山"));
    settextstyle(40, 0, _T("Impact"));
    roundrect(320, 280, 640, 320, 5, 5);
    roundrect(320, 330, 640, 370, 5, 5);
    roundrect(320, 380, 640, 420, 5, 5);
    roundrect(320, 430, 640, 470, 5, 5);
    roundrect(320, 480, 640, 520, 5, 5);
    roundrect(320, 530, 640, 570, 5, 5);
    roundrect(320, 580, 640, 620, 5, 5);
    roundrect(320, 630, 640, 670, 5, 5);
    roundrect(320, 680, 640, 720, 5, 5);
    settextstyle(36, 0, _T("Consolas"), 0, 0, 0, false, false, false);
    outtextxy(320, 280, _T("存档一"));
    outtextxy(320, 330, _T("存档二"));
    outtextxy(320, 380, _T("存档三"));
    outtextxy(320, 430, _T("存档四"));
    outtextxy(320, 480, _T("存档五"));
    outtextxy(320, 530, _T("存档六"));
    outtextxy(320, 580, _T("存档七"));
    outtextxy(320, 630, _T("存档八"));
    outtextxy(320, 680, _T("存档九"));
    settextcolor(RGB(100, 149, 237));
    while (1)
    {
        MOUSEMSG m = GetMouseMsg();
        settextcolor(RGB(255, 179, 166));
        if (m.x < 640 && m.x>320 && m.y < 320 && m.y>280)
        {
            outtextxy(320, 280, _T("存档一"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 1; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 370 && m.y>330)
        {
            outtextxy(320, 330, _T("存档二"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 2; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 420 && m.y>380)
        {
            outtextxy(320, 380, _T("存档三"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 3; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 470 && m.y>430)
        {
            outtextxy(320, 430, _T("存档四"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 4; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 520 && m.y>480)
        {
            outtextxy(320, 480, _T("存档五"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 5; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 570 && m.y>530)
        {
            outtextxy(320, 530, _T("存档六"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 6; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 620 && m.y>580)
        {
            outtextxy(320, 580, _T("存档七"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 7; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 670 && m.y>630)
        {
            outtextxy(320, 630, _T("存档八"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 8; break; }
        }
        else if (m.x < 640 && m.x>320 && m.y < 720 && m.y>680)
        {
            outtextxy(320, 680, _T("存档九"));
            if (m.uMsg == WM_LBUTTONDOWN) { k = 9; break; }
        }
        else
        {
            settextcolor(RGB(100, 149, 237));
            outtextxy(320, 280, _T("存档一"));
            outtextxy(320, 330, _T("存档二"));
            outtextxy(320, 380, _T("存档三"));
            outtextxy(320, 430, _T("存档四"));
            outtextxy(320, 480, _T("存档五"));
            outtextxy(320, 530, _T("存档六"));
            outtextxy(320, 580, _T("存档七"));
            outtextxy(320, 630, _T("存档八"));
            outtextxy(320, 680, _T("存档九"));
        }
        FlushMouseMsgBuffer();
    } 
    refile tA;
    tA.num = k;
    FILE* fps;    
    std::string sc = "\docu" + std::to_string(k) + ".txt";
    fps = fopen(sc.c_str(), "r");
    int mark = 0, life = 0, magic = 0, kill = 0, tr = 0, rank = 0;
    float tx=0, ty=0, tdx=0, tdy=0, ta=0;
    fscanf(fps, "%d %d %d %d %f %f %f %f %f %d %d", &mark, &life, &magic, &kill, &tx, &ty, &tdx, &tdy, &ta, &tr, &rank);
    tA.mark0 = mark;
    tA.life0 = life;
    tA.magic0 = magic;
    tA.kill0 = kill;
    tA.tx0 = tx;
    tA.ty0 = ty;
    tA.tdx0 = tdx;
    tA.tdy0 = tdy;
    tA.ta0 = ta;
    tA.tr0 = tr;
    tA.rank0 = rank;
    fclose(fps);
    return tA;
}

int main()
{
     srand(time(0));
ZZ:
    int k = start();
    int k1 = 0;

    switch (k)
    {
    case 1:rank = game_sle(); goto AA; break;
    case 2: goto AA; break;
    case 3: k1 = help(); goto ZZ; break;
    case 4: k1 = intro(); goto ZZ; break;
    default: return 0;
    }

AA:
    refile tA = { 0,5,60,0,W / 2,H / 2,0,0,0,0,rank };
    if (k == 2) tA = docu_read();
    mark1 = tA.mark0;
    life1 = tA.life0;
    magic1 = tA.magic0;
    kill1 = tA.kill0;
    tx = tA.tx0;
    ty = tA.ty0;
    tdx = tA.tdx0;
    tdy = tA.tdy0;
    ta = tA.ta0;
    tr = tA.tr0;
    //rank = tA.rank0;
    RenderWindow app(VideoMode(W, H), L"天问之旅");
    app.setFramerateLimit(60);

    std::string sd = "images/background" + std::to_string(rank) + ".jpg";
    Texture t1, t2, t3, t4, t5, t52, t6, t7;
    t1.loadFromFile("images/spaceship.png"); t1.setSmooth(true);
    t2.loadFromFile("images/background1.jpg"); t2.setSmooth(true);
    //t2.loadFromFile(sd); t2.setSmooth(true);
    Sprite background(t2);

    t3.loadFromFile("images/explosions/type_C.png");
    t4.loadFromFile("images/rock.png");
    t5.loadFromFile("images/fire_red.png");
    t52.loadFromFile("images/fire_blue.png");
    t6.loadFromFile("images/rock_small.png");
    t7.loadFromFile("images/explosions/type_B.png");

    Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
    Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
    Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
    Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
    Animation sBullet2(t52, 0, 0, 32, 64, 16, 0.8);
    Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
    Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
    Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);


    std::list<Entity*> entities;

    for (int i = 0; i < 15; i++)
    {
        asteroid* a = new asteroid();
        a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
        entities.push_back(a);
    }
    player* p = new player();
    p->settings(sPlayer, W / 2, H / 2, 0, 20);
    entities.push_back(p);

    Music music1;
    if (!music1.openFromFile("audio/appletree.ogg"))
        return -1;
    music1.play();
    SoundBuffer sBuffer;
    if (!sBuffer.loadFromFile("audio/cannon2.wav"))
        return -1;
    Sound cannon2(sBuffer);
    SoundBuffer sBuffer1;
    if (!sBuffer1.loadFromFile("audio/cannon.ogg"))
        return -1;
    Sound cannon1(sBuffer1);
    Font font;
    if (!font.loadFromFile("fonts/times.ttf"))
        return -1;
    std::string sa1 = "score:";
    std::string sa2 = "\nlives:";
    std::string sa3 = "\nmagic:";
    std::string sa4 = "\nx:";
    std::string sa5 = "\ny:";
    std::string sa6 = "\ndx:";
    std::string sa7 = "\ndy:";
    std::string sa8 = "\nda:";
    std::string sa10 = "\nweapon:";
    //main loop
    int sleep0=sleep3;
    int radi0 = radi3;
    clock_t sta1, end1;
    sta1 = clock();
    end1 = clock()-10;
    int sat=1;
    while (app.isOpen()) 
    {
        Event event;
        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();
            if (Keyboard::isKeyPressed(Keyboard::P))
            { 
                docu(mark1, life1, magic1, kill1, tx, ty, tdx, tdy, ta, tr, rank);
                event.type = Event::Closed;
                goto ZZ;
            }
            sat = end1 - sta1;
            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::E)
                {
                    end1 = clock();
                    if (sat > sleep0)
                    {
                        bullet2* b = new bullet2();
                        b->settings(sBullet2, p->x, p->y, p->angle, radi0);
                        entities.push_back(b);
                        cannon2.play();
                        sta1 = clock();
                    }
                    else continue;
                }
            if (event.key.code == Keyboard::Q && magic1 > 0)
            {
                magic1 -= 1;
                bullet* b0 = new bullet(); b0->settings(sBullet, p->x, p->y, p->angle, 20); entities.push_back(b0);
                bullet* b1 = new bullet(); b1->settings(sBullet, p->x, p->y, p->angle + 10, 20); entities.push_back(b1);
                bullet* b2 = new bullet(); b2->settings(sBullet, p->x, p->y, p->angle + 20, 20); entities.push_back(b2);
                bullet* b3 = new bullet(); b3->settings(sBullet, p->x, p->y, p->angle + 30, 20); entities.push_back(b3);
                bullet* b4 = new bullet(); b4->settings(sBullet, p->x, p->y, p->angle + 40, 20); entities.push_back(b4);
                bullet* b5 = new bullet(); b5->settings(sBullet, p->x, p->y, p->angle + 50, 20); entities.push_back(b5);
                bullet* b6 = new bullet(); b6->settings(sBullet, p->x, p->y, p->angle + 60, 20); entities.push_back(b6);
                bullet* b7 = new bullet(); b7->settings(sBullet, p->x, p->y, p->angle + 70, 20); entities.push_back(b7);
                bullet* b8 = new bullet(); b8->settings(sBullet, p->x, p->y, p->angle + 80, 20); entities.push_back(b8);
                bullet* b9 = new bullet(); b9->settings(sBullet, p->x, p->y, p->angle + 90, 20); entities.push_back(b9);
                bullet* b10 = new bullet(); b10->settings(sBullet, p->x, p->y, p->angle + 100, 20); entities.push_back(b10);
                bullet* b11 = new bullet(); b11->settings(sBullet, p->x, p->y, p->angle + 110, 20); entities.push_back(b11);
                bullet* b12 = new bullet(); b12->settings(sBullet, p->x, p->y, p->angle + 120, 20); entities.push_back(b12);
                bullet* b13 = new bullet(); b13->settings(sBullet, p->x, p->y, p->angle + 130, 20); entities.push_back(b13);
                bullet* b14 = new bullet(); b14->settings(sBullet, p->x, p->y, p->angle + 140, 20); entities.push_back(b14);
                bullet* b15 = new bullet(); b15->settings(sBullet, p->x, p->y, p->angle + 150, 20); entities.push_back(b15);
                bullet* b16 = new bullet(); b16->settings(sBullet, p->x, p->y, p->angle + 160, 20); entities.push_back(b16);
                bullet* b17 = new bullet(); b17->settings(sBullet, p->x, p->y, p->angle + 170, 20); entities.push_back(b17);
                bullet* b18 = new bullet(); b18->settings(sBullet, p->x, p->y, p->angle + 180, 20); entities.push_back(b18);
                bullet* b19 = new bullet(); b19->settings(sBullet, p->x, p->y, p->angle + 190, 20); entities.push_back(b19);
                bullet* b20 = new bullet(); b20->settings(sBullet, p->x, p->y, p->angle + 200, 20); entities.push_back(b20);
                bullet* b21 = new bullet(); b21->settings(sBullet, p->x, p->y, p->angle + 210, 20); entities.push_back(b21);
                bullet* b22 = new bullet(); b22->settings(sBullet, p->x, p->y, p->angle + 220, 20); entities.push_back(b22);
                bullet* b23 = new bullet(); b23->settings(sBullet, p->x, p->y, p->angle + 230, 20); entities.push_back(b23);
                bullet* b24 = new bullet(); b24->settings(sBullet, p->x, p->y, p->angle + 240, 20); entities.push_back(b24);
                bullet* b25 = new bullet(); b25->settings(sBullet, p->x, p->y, p->angle + 250, 20); entities.push_back(b25);
                bullet* b26 = new bullet(); b26->settings(sBullet, p->x, p->y, p->angle + 260, 20); entities.push_back(b26);
                bullet* b27 = new bullet(); b27->settings(sBullet, p->x, p->y, p->angle + 270, 20); entities.push_back(b27);
                bullet* b28 = new bullet(); b28->settings(sBullet, p->x, p->y, p->angle + 280, 20); entities.push_back(b28);
                bullet* b29 = new bullet(); b29->settings(sBullet, p->x, p->y, p->angle + 290, 20); entities.push_back(b29);
                bullet* b30 = new bullet(); b30->settings(sBullet, p->x, p->y, p->angle + 300, 20); entities.push_back(b30);
                bullet* b31 = new bullet(); b31->settings(sBullet, p->x, p->y, p->angle + 310, 20); entities.push_back(b31);
                bullet* b32 = new bullet(); b32->settings(sBullet, p->x, p->y, p->angle + 320, 20); entities.push_back(b32);
                bullet* b33 = new bullet(); b33->settings(sBullet, p->x, p->y, p->angle + 330, 20); entities.push_back(b33);
                bullet* b34 = new bullet(); b34->settings(sBullet, p->x, p->y, p->angle + 340, 20); entities.push_back(b34);
                bullet* b35 = new bullet(); b35->settings(sBullet, p->x, p->y, p->angle + 350, 20); entities.push_back(b35);
                cannon1.play();
            }
            if (Keyboard::isKeyPressed(Keyboard::Num1)) { sleep0 = sleep1; radi0 = radi1; sta1 -= 3000; }
            if (Keyboard::isKeyPressed(Keyboard::Num2)) { sleep0 = sleep2; radi0 = radi2; }
            if (Keyboard::isKeyPressed(Keyboard::Num3)) { sleep0 = sleep3; radi0 = radi3; }
            if (Keyboard::isKeyPressed(Keyboard::Num4)) { sleep0 = sleep4; radi0 = radi4; }
            if (Keyboard::isKeyPressed(Keyboard::Num5)) { sleep0 = sleep6; radi0 = radi6; }
            if (Keyboard::isKeyPressed(Keyboard::Num6)) { sleep0 = sleep8; radi0 = radi8; }

            if (Keyboard::isKeyPressed(Keyboard::Num7)) { sleep0 = sleep5; radi0 = radi5; }
            if (Keyboard::isKeyPressed(Keyboard::Num8)) { sleep0 = sleep7; radi0 = radi7; }
            if (Keyboard::isKeyPressed(Keyboard::Num9)) { sleep0 = sleep9; radi0 = radi9; }
            if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) p->angle += 5;
            if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))  p->angle -= 5;
            if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) p->thrust = true;
            else p->thrust = false;
        }

        for (auto a : entities)
            for (auto b : entities)
            {
                if (a->name == "asteroid" && b->name == "bullet")
                    if (isCollide(a, b))
                    {
                        a->life = false;
                        b->life = false;
                        mark1 += 10;
                        Entity* e = new Entity();
                        e->settings(sExplosion, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);
                        for (int i = 0; i < rand() % 5 + 2; i++)
                        {
                            if (a->R == 15) continue;
                            Entity* e = new asteroid();
                            e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
                            entities.push_back(e);
                        }
                    }
                if (a->name == "asteroid" && b->name == "bullet2")
                    if (isCollide(a, b))
                    {
                        a->life = false;
                        b->life = false;
                        mark1 += 10;
                        Entity* e = new Entity();
                        e->settings(sExplosion, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);
                        for (int i = 0; i < rand() % 5 + 2; i++)
                        {
                            if (a->R == 15) continue;
                            Entity* e = new asteroid();
                            e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
                            entities.push_back(e);
                        }

                    }

                if (a->name == "player" && b->name == "asteroid")
                    if (isCollide(a, b))
                    {
                        b->life = false;
                        life1 -= 1;
                        Entity* e = new Entity();
                        e->settings(sExplosion_ship, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);

                        p->settings(sPlayer, W / 2, H / 2, 0, 20);
                        p->dx = 0; p->dy = 0;
                    }
            }

        if (p->thrust)  p->anim = sPlayer_go;
        else   p->anim = sPlayer;

        for (auto e : entities)
            if (e->name == "explosion")
                if (e->anim.isEnd()) { e->life = 0; }

        if (rand() % 150 == 0)
        {
            asteroid* a = new asteroid();
            a->settings(sRock, 0, rand() % H, rand() % 360, 25);
            entities.push_back(a);
        }

        for (auto i = entities.begin(); i != entities.end();)
        {
            Entity* e = *i;
            e->update();
            e->anim.update();
            if (e->life == false) { i = entities.erase(i); delete e; }
            else i++;
        }
        tx = p->x;
        ty = p->y;
        tdx = p->dx;
        tdy = p->dy;
        ta = p->angle;
        tr = p->R;
        while (1)
        {
            if (ta > 180)ta -= 180;
            if (ta < -180)ta += 180;
            else break;
        }
        //draw
        std::string sa9;
        switch (radi0)
        {
        case(radi1):sa9 = "Yakhont-Missle"; break;//30
        case(radi2):sa9 = "RedArray-Rocket"; break;
        case(radi3):sa9 = "57mm-Cannon"; break;
        case(radi4):sa9 = "37mm-Cannon"; break;//300
        case(radi5):sa9 = "37mm-Cannon-E"; break;
        case(radi6):sa9 = "23mm-Cannon"; break;//500
        case(radi7):sa9 = "23mm-Cannon-E"; break;
        case(radi8):sa9 = "GatlingGun"; break;
        case(radi9):sa9 = "GatlingGun-E"; break;
        }

        std::string sc = sa1 + std::to_string(mark1) + sa2 + std::to_string(life1) + sa3 + std::to_string(magic1 / 2)
            + sa4 + std::to_string(tx) + sa5 + std::to_string(ty) + sa6 + std::to_string(tdx)
            + sa7 + std::to_string(tdy) + sa8 + std::to_string(-ta)
            + sa10+sa9+ std::to_string(sat%10000);
        Text text(sc, font);
        app.draw(background);
        app.draw(text);
        for (auto i : entities) i->draw(app);
        app.display();
    }
    goto ZZ;
    return 0;
}
