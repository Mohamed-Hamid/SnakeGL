#ifndef BASE_H
    #define BASE_H
    #include "base.h"
#endif

#include "scenario.cpp"
#include <ctime>
#include <cstdio>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <iostream>
#include <string>
#include <thread>

class Game
{
private:
    bool      is_game_over;
    bool      play_game_over = false;
    bool      is_running;
    bool      paused;
    bool      ate;
    bool      play_ate = false;
    bool      key_pressed;
    bool      key0 = true;
    bool      key1;
    bool      key2;
    bool      key3;
    bool      play_level = false;
    int       m, m2;
    int       frameCount;
    int       currentTime;
    int       previousTime;
    int       score;
    int       tick;
    int       tick2;
    int       level;
    float     fps;
    Scenario* scenario;
    bool      wait();
    bool      wait2();
    bool      clock();
    bool      clock2();
    void      calculateFPS();
    void      run();
    void      draw_menu();
    Mix_Music* music;
    // void playSound(const char * fileName, int repeat);
    void playSound(const char * fileName, int repeat,bool musicSound, int channel);
public:
    Game();
    ~Game();
    void display();
    void pause();
    void start();
    void stop();
    void reset();
    void on_key_pressed(int key);
};