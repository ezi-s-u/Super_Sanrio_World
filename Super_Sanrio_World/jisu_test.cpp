#include<SFML/Graphics.hpp>
#include <windows.h>
#include <iostream>

#define WIDTH 840
#define HEIGHT 480
using namespace std;
using namespace sf;

struct Position {
    int x;
    int y;
};
int main(void)
{
    RenderWindow window(VideoMode(840, 480), "Super Sanrio World");
    window.setFramerateLimit(60);

    const int changeCharacter = 5;
    int index = 0;
    float frame = 0.f;
    float frameSpeed = 0.4f;

    const int gravity = 10;
    bool isJumping = false;
    bool isBottom = true;

    Texture map;
    map.loadFromFile("resources/sanrio_map.png");
    Sprite mapSprite(map);
    mapSprite.setTextureRect(IntRect(0, 0, WIDTH, HEIGHT));

    Texture kitty1;
    kitty1.loadFromFile("resources/character1.png");
    Texture kitty2;
    kitty2.loadFromFile("resources/character2.png");
    Sprite kittySprite[2];
    kittySprite[0] = Sprite(kitty1);
    kittySprite[1] = Sprite(kitty2);

    const int KITTY_Y_BOTTOM = HEIGHT - 150;

    Position kittyPos;
    kittyPos.x = 70;
    kittyPos.y = KITTY_Y_BOTTOM;

   

    const int cloudCnt = 2;

    Texture cloud[cloudCnt];
    Position cloudPos[cloudCnt];
    Sprite cloudSprite[cloudCnt];

    cloud[0].loadFromFile("resources/cloud1.png");
    cloud[1].loadFromFile("resources/cloud2.png");


    for (int i = 0; i < cloudCnt; i++) {
        cloudPos[i].x = 10;
        cloudPos[i].y = 60;
        cloudSprite[i].setTexture(cloud[i]);
    }

    Clock clock;
    float seconds = 0.0f;
    int score = 0;

    Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\H2GSRB.ttf"))
    {
        printf("폰트 불러오기 실패\n");
        return -1;
    }
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(0, 0);
    
    bool appleExist = false;
    bool tupliExist = false;
    float appleTimer = 0.0f;
    const float appleDelay = 1.0f;

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {

            if (e.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (isBottom == true && isJumping == false) {
                isJumping = true;
                isBottom = false;
            }
        }
        if (isJumping == true)
        {
            kittyPos.y -= gravity;
        }
        else {
            kittyPos.y += gravity;
        }

        //점프하고 있지 않을 시 Y값에 있도록
        if (kittyPos.y >= KITTY_Y_BOTTOM) {
            kittyPos.y = KITTY_Y_BOTTOM;
            isBottom = true;
        }
        //점프 높이 제한
        if (kittyPos.y <= KITTY_Y_BOTTOM -150)
        {
            isJumping = false;
        }
        kittySprite[index].setPosition(kittyPos.x, kittyPos.y);

        if (!appleExist) {
            appleTimer += clock.restart().asSeconds();
            if (appleTimer >= appleDelay) {
                appleExist = true;
                appleTimer = 0.0f;
            }
        }
        

        for (int i = 0; i < cloudCnt; i++) {
            if (cloudPos[i].x <= 0)
            {
                cloudPos[i].x = WIDTH;
            }
            else
            {
                cloudPos[i].x -= 2 + (i * 2);
            }
            cloudSprite[i].setPosition(cloudPos[i].x, cloudPos[i].y);
        }

       

        //캐릭터 다리움직임
        frame += frameSpeed;
        if (frame > changeCharacter) {
            frame -= changeCharacter;
            index++;
            if (index >= 2) index = 0;
        }

        //점수
        seconds += clock.restart().asSeconds();
        if (seconds >= 0.3f) {
            score++;
            seconds = 0.0f;
        }
        scoreText.setString("Score: " + to_string(score));
        HWND hWndConsole = GetConsoleWindow();
        ShowWindow(hWndConsole, SW_HIDE);

        window.clear();
        window.draw(mapSprite);
        window.draw(kittySprite[index]);
        window.draw(scoreText);
        for (int i = 0; i < cloudCnt; i++) {
            window.draw(cloudSprite[i]);
        }
    
        window.display();
    }

    return 0;
}