#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdlib.h>
#include <vector>
#include <iomanip>
#include <string>
#include <random>

enum class m_State{
  MENU, GRA, BOSS, LOADING, GAME_OVER
};


class Meteor{
    public:
        float x;
        float y;
        float vx;
        float vy;
        float r;
};

std::vector<Meteor> createMeteorObjects(int N){
    std::vector <Meteor> Meteors2;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> random(50,600);
    std::uniform_int_distribution<std::mt19937::result_type> random2(50,400);
    std::uniform_int_distribution<std::mt19937::result_type> random3(10,20);
    for(int i = 0; i < N; i++){
        Meteor Meteors1;
        Meteors1.x = random(rng);
        Meteors1.y = random(rng);
        Meteors1.vx = random2(rng);
        Meteors1.vy = random2(rng);
        Meteors1.r = random3(rng);
        Meteors2.push_back(Meteors1);
    }
    return Meteors2;
}


int main(){
    //Random seed initalization
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> random(1,30);
    //Random seed initalization

    //Window
    const float width = 800;
    const float height = 800;
    sf::RenderWindow window(sf::VideoMode(width, height), "Earth Defender");
    window.setVerticalSyncEnabled(true);
    //Window

    //State
    m_State state = m_State::MENU;
    //State

    //Sprites and shapes
    sf::Texture backgroundTexture;
    sf::Texture earthTexture;
    sf::Texture startButtonTexture;
    sf::Texture meteorTexture;
    sf::Texture replayButtonTexture;
    sf::Color GREY(220, 220, 220);

    sf::Sprite backgroundObject;
    sf::RectangleShape hpBarHelperObject(sf::Vector2f(500.f, 30.f));
    sf::RectangleShape hpBarObject(sf::Vector2f(500.f, 30.f));
    sf::CircleShape earthObject(200);
    sf::CircleShape bossObject(300);
    sf::RectangleShape startButtonObject(sf::Vector2f(200.f, 130.f));
    sf::RectangleShape replayButtonObject(sf::Vector2f(200.f, 130.f));

    backgroundTexture.loadFromFile("tlo.jpg");
    earthTexture.loadFromFile("swiat.jpg");
    startButtonTexture.loadFromFile("start.png");
    meteorTexture.loadFromFile("meteor.jpg");
    replayButtonTexture.loadFromFile("replay.png");

    backgroundObject.setTexture(backgroundTexture);
    earthObject.setTexture(&earthTexture);
    startButtonObject.setTexture(&startButtonTexture);
    bossObject.setTexture(&meteorTexture);
    hpBarHelperObject.setFillColor(GREY);
    hpBarObject.setFillColor(sf::Color::Red);
    replayButtonObject.setTexture(&replayButtonTexture);

    earthObject.setPosition(width/2 - 200, height/2 - 200);

    //Sprites and shapes

    //Sounds and music
    sf::SoundBuffer buffer;
    buffer.loadFromFile("laser.wav");
    sf::Sound laser;
    laser.setBuffer(buffer);

    sf::Music music;
    music.openFromFile("music.ogg");
    music.setLoop(true);
    music.setVolume(1.f);
    music.play();
    //Sounds and music

    //Text
    sf::Text text;
    sf::Text bossTimerText;
    sf::Font font;
    font.loadFromFile("GothicA1-Black.ttf");
    text.setFont(font);
    bossTimerText.setFont(font);
    //Text

    //Meteors
    std::vector <Meteor> Meteors;
    Meteors = createMeteorObjects(100);
    int meteorAmount = Meteors.size();
    //Meteors

    //Variables
    int points = 0;
    int bossHp = 500;
    double bossTime = 60;
    //Variables

    //Time handling
    sf::Clock deltaClock;
    sf::Time time;
    sf::Clock bossTimer;
    //Time handling

    //Main loop
    while (window.isOpen()){
        time = deltaClock.restart();
        double dt = time.asSeconds();
        //Menu state
        if(state == m_State::MENU){
            sf::Event event;
            while (window.pollEvent(event)){
                if (event.type == sf::Event::Closed){
                    window.close();
                }
                if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
                    double xm = sf::Mouse::getPosition(window).x;
                    double ym = sf::Mouse::getPosition(window).y;
                    if(xm >= width/2 - 100 and xm <= width/2 + 100 and ym >= height/2 - 65 and ym <= height/2 + 65){
                        state = m_State::LOADING;
                    }
                }
            }
            window.clear(sf::Color::White);
            startButtonObject.setPosition(width/2 - 100, height/2 - 65);
            window.draw(backgroundObject);
            window.draw(earthObject);
            window.draw(startButtonObject);
            window.display();
        }
        //Menu state

        //Loading state
        else if(state == m_State::LOADING){
            sf::Event event;
            while (window.pollEvent(event)){
                if (event.type == sf::Event::Closed){
                    window.close();
                }
                if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
                    double xm = sf::Mouse::getPosition(window).x;
                    double ym = sf::Mouse::getPosition(window).y;
                    if(xm >= width/2 - 100 and xm <= width/2 + 100 and ym >= height/2 + 200 and ym <= height/2 + 330){
                        state = m_State::GRA;
                    }
                }
            }
            window.clear(sf::Color::White);
            text.setString("W kierunku Ziemii leca meteoryty. Musisz je\n wszystkie zestrzelic!!");
            text.setPosition(100, height/2);
            startButtonObject.setPosition(width/2 - 100, height/2 + 200);
            window.draw(backgroundObject);
            window.draw(earthObject);
            window.draw(startButtonObject);
            window.draw(text);
            window.display();
        }
        //Loading state

        //Game state
        else if(state == m_State::GRA or state == m_State::BOSS){
            sf::Event event;
            while (window.pollEvent(event)){
                if (event.type == sf::Event::Closed){
                    window.close();
                }
                if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
                    double xm = sf::Mouse::getPosition(window).x;
                    double ym = sf::Mouse::getPosition(window).y;
                    laser.play();
                    if(state != m_State::BOSS){
                        for(int i = 0; i < Meteors.size(); i++){
                            if(xm >= Meteors[i].x and xm <= (Meteors[i].x + (Meteors[i].r * 2)) and ym >= Meteors[i].y and ym <= (Meteors[i].y + (Meteors[i].r * 2))){
                                Meteors.erase(Meteors.begin() + i);
                                points = points + (meteorAmount - Meteors.size());
                                meteorAmount -= 1;
                                if(Meteors.size() == 0){
                                    state = m_State::GAME_OVER;
                                }
                                int bossChance = random(rng);
                                if(bossChance == 15){
                                    bossHp = 500;
                                    bossTime = 60;
                                    bossTimer.restart();
                                    state = m_State::BOSS;
                                }
                            }
                        }
                    }
                    else{
                        if(xm >= bossObject.getPosition().x and xm <= (bossObject.getPosition().x + (300 * 2)) and ym >= bossObject.getPosition().y and ym <= (bossObject.getPosition().y + (300 * 2))){
                            bossHp -= 5;
                            if(bossHp <= 0){
                                points += 100;
                                state = m_State::GRA;
                            }
                        }
                    }
                }
            }

            if(state != m_State::BOSS){
                text.setString("Twoje punkty: " + std::to_string(points));
                text.setPosition(0, 0);
                window.clear(sf::Color::White);
                window.draw(backgroundObject);
                window.draw(text);
                for(int i = 0; i < Meteors.size(); i++){
                    sf::CircleShape meteor(Meteors[i].r);
                    meteor.setTexture(&meteorTexture);
                    meteor.setPosition(Meteors[i].x, Meteors[i].y);
                    Meteors[i].x = Meteors[i].x+(Meteors[i].vx*dt);
                    Meteors[i].y = Meteors[i].y+(Meteors[i].vy*dt);
                    if(Meteors[i].x >= width-(Meteors[i].r*2) || Meteors[i].x <= 0){
                        Meteors[i].vx *= -1;
                    }
                    if(Meteors[i].y >= height-(Meteors[i].r*2) || Meteors[i].y <= 0){
                        Meteors[i].vy *= -1;
                    }
                    window.draw(meteor);
                }
                window.display();
            }
            else{
                if(bossTime <= 0){
                    state = m_State::GAME_OVER;
                }
                window.clear(sf::Color::White);
                text.setString("W kierunku ziemii leci ogromna asteroida zestrzel ja!!");
                text.setPosition(0,750);
                sf::Time timer = bossTimer.getElapsedTime();
                double bossTimerInSeconds = timer.asSeconds();
                bossTime -= bossTimerInSeconds;
                bossTimer.restart();
                bossTimerText.setString(std::to_string((int)bossTime) + "s");
                window.draw(backgroundObject);
                window.draw(hpBarHelperObject);
                window.draw(hpBarObject);
                bossObject.setPosition((width/2)-300, (height/2)-300);
                hpBarHelperObject.setPosition(150, 10);
                hpBarObject.setPosition(150, 10);
                hpBarObject.setSize(sf::Vector2f(bossHp, 30.f));
                window.draw(bossObject);
                window.draw(text);
                window.draw(hpBarHelperObject);
                window.draw(bossTimerText);
                window.draw(hpBarObject);
                window.display();
            }
        }
        //Game state

        //Game over state
        else if(state == m_State::GAME_OVER){
            sf::Event event;
            while (window.pollEvent(event)){
                if (event.type == sf::Event::Closed){
                    window.close();
                }
                if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
                    double xm = sf::Mouse::getPosition(window).x;
                    double ym = sf::Mouse::getPosition(window).y;
                    laser.play();
                    if(xm >= replayButtonObject.getPosition().x and xm <= (replayButtonObject.getPosition().x + 200) and ym >= replayButtonObject.getPosition().y and ym <= (replayButtonObject.getPosition().y + 130)){
                        state = m_State::MENU;
                        Meteors = createMeteorObjects(100);
                        meteorAmount = Meteors.size();
                        points = 0;
                        bossHp = 500;
                        bossTime = 60;
                    }
                }
            }
            window.clear(sf::Color::White);
            replayButtonObject.setPosition(width/2 - 100, height/2 - 65);
            text.setString("Twoje punkty: " + std::to_string(points));
            text.setPosition(300, 0);
            window.draw(backgroundObject);
            window.draw(replayButtonObject);
            window.draw(text);
            window.display();
        }
        //Game over state
    }




    return 0;
}
