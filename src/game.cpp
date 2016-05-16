#include "game.h"

using namespace std;

void Game::playSound(const char * fileName, int repeat,bool musicSound, int channel){
      if(musicSound){
          // Init
          SDL_Init(SDL_INIT_AUDIO);

          // Open Audio device
          Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);

          // Set Volume
          Mix_VolumeMusic(100);

          // Open Audio File
          music = Mix_LoadMUS(fileName);

          Mix_PlayMusic(music, repeat);

          while (Mix_PlayingMusic())
          {  
                SDL_Delay(1000);
          }

           // Free File
          Mix_FreeMusic(music);
          music = 0;

          // End
          Mix_CloseAudio();
      }else{
            Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
            // Mix_OpenAudio(44100,8,2,1400);
            Mix_Chunk *beat1;


            beat1 = Mix_LoadWAV(fileName);

            // cout << Mix_GetError() << endl;
            // cout << "beat1 = " << beat1 << endl;


            Mix_VolumeChunk(beat1,80);

            Mix_PlayChannel(channel,beat1,0);
      
            SDL_Delay(1000);
      }
}

// void Game::playSound(const char * fileName, int repeat){
//       // Init
//       SDL_Init(SDL_INIT_AUDIO);

//       // Open Audio device
//       Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);

//       // Set Volume
//       Mix_VolumeMusic(100);

//       // Open Audio File
//       Mix_Music* music = Mix_LoadMUS(fileName);

//       Mix_PlayMusic(music, repeat);

//       while (Mix_PlayingMusic())
//       {  
//             SDL_Delay(1000);
//       }

//        // Free File
//       Mix_FreeMusic(music);
//       music = 0;

//       // End
//       Mix_CloseAudio();
// }

static float to_fps(float fps, int value)
{
    if (fps < 1)
        fps = 60;
    return value * fps / 60;
}

Game::Game()
{
    fps = 0;
    frameCount = 0;
    currentTime = 0,
    previousTime = 0;

    is_game_over = false;
    is_running = false;
    paused = false;

    level = 1;

    m = to_fps(fps, 30);
    m2 = to_fps(fps, 10);

    tick = 30;
    tick2 = 10;
    restore_highest_scores();

    scenario = new Scenario();
}

Game::~Game()
{
    delete scenario;
}

void Game::pause()
{
    paused = true;
}

void Game::start()
{
    paused = false;
}

void Game::stop()
{
    is_running = false;
    is_game_over = false;
    paused = false;
}

void Game::reset()
{
    m = to_fps(fps, 30);
    m2 = to_fps(fps, 10);

    score = 0;
    ate = false;
    is_game_over = false;
    play_game_over = false;
    paused = false;
    play_level = false;

    tick = 30;
    tick2 = 10;

    scenario->reset();
    is_running = true;
}

void Game::draw_menu()
{
    enable_2D_texture();
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, textures[MENU_TEXTURE]);
        glBegin(GL_QUADS);
            glNormal3f(0.0, 1.0, 0.0);
            glTexCoord2f(0, 1);
            glVertex3f(-8, 0.0f,  8);
            glTexCoord2f(1, 1);
            glVertex3f( 8, 0.0f,  8);
            glTexCoord2f(1, 0);
            glVertex3f( 8, 0.0f, -8);
            glTexCoord2f(0, 0);
            glVertex3f(-8, 0.0f, -8);
        glEnd();
    glPopMatrix();
    disable_2D_texture();

    Point p;

    p.x = -2.3f;
    p.y = 0.5f;
    p.z = -3.5f;
    draw_text("SELECT YOUR LEVEL", p, 1.0f, 1.0f, 1.0f);

    p.x = -1.0f;
    p.y = 0.5f;
    p.z = -2.0f;

    if (level == EASY)
    {
        p.x -= 0.3;
        draw_text("< EASY >", p, 1.0f, 1.0f, 0.0f);
    }
    else
    {
        draw_text("EASY", p, 1.0f, 1.0f, 1.0f);
    }

    p.x = -1.3f;
    p.y = 0.5f;
    p.z = -1.0f;

    if (level == MEDIUM)
    {
        p.x -= 0.3;
        draw_text("< MEDIUM >", p, 1.0f, 1.0f, 0.0f);
    }
    else
    {
        draw_text("MEDIUM", p, 1.0f, 1.0f, 1.0f);
    }

    p.x = -1.0f;
    p.y = 0.5f;
    p.z = 0.0f;

    if (level == HARD)
    {
        p.x -= 0.3;
        draw_text("< HARD >", p, 1.0f, 1.0f, 0.0f);
    }
    else
    {
        draw_text("HARD", p, 1.0f, 1.0f, 1.0f);
    }

    p.x = -1.7f;
    p.y = 0.5f;
    p.z = 1.0f;

    if (level == VERY_HARD)
    {
        p.x -= 0.3;
        draw_text("< VERY HARD >", p, 1.0f, 1.0f, 0.0f);
    }
    else
    {
        draw_text("VERY HARD", p, 1.0f, 1.0f, 1.0f);
    }

    draw_text("", p, 1.0f, 1.0f, 1.0f);

    if (wait())
    {
        p.x = -2.6f;
        p.y = 0.5f;
        p.z = 3.0f;
        draw_text("PRESS ENTER TO START", p, 1.0f, 1.0f, 1.0f);
    }
}

void Game::display()
{
    int old_cam = scenario->camera_mode;

    scenario->camera_mode = 3;
    scenario->set_camera();

    calculateFPS();
    Point p;

    char s [50];
    char h [100];

    if (is_running)
    {

        scenario->camera_mode = old_cam;
        scenario->set_camera();

        if (clock2())
        {
            if (scenario->a > 360)
            {
                scenario->a = 0;
            }

            scenario->a += 5;

            if (scenario->m > 0.1f)
            {
                scenario->m = 0.0;
            }

            scenario->m += 0.05;
        }

        scenario->draw_objects();

        glColor3f(0.0f, 0.0f, 0.0f);
        glRectf(0,0, 0.75f, -0.1f);

        scenario->camera_mode = 3;
        scenario->set_camera();

        if (clock())
        {
            run();
        }

        if (is_game_over)
        {
            if (wait())
            {
                if(!play_game_over){
                    std::thread gameOver(&Game::playSound, this , "./resources/GameOver.wav", 1, false, 1);
                    gameOver.detach(); 
                    play_game_over = true;
                }
                p.x = -1.25f;
                p.y = 0.5f;
                p.z = 0.15f;
                draw_text("GAME OVER", p, 0.0f, 0.0f, 0.0f);
                update_highest_score();
            }
        }

        int highest_score;
        sprintf(s, "SCORE: %d", score * 10);

        switch (level)
        {
            case EASY:
                highest_score = highest_scores[0];
            break;
            case MEDIUM:
                highest_score = highest_scores[1];
            break;
            case HARD:
                highest_score = highest_scores[2];
            break;
            case VERY_HARD:
                highest_score = highest_scores[3];
            break;
        }
        sprintf(h, "HIGHEST SCORE: %d", highest_score);

        p.x = -1.0f;
        p.y = 0.5f;
        p.z = -7.0f;

        if (ate)
        {
            if(!play_ate){
                std::thread eat(&Game::playSound, this , "./resources/Eat.wav", 1, false, 2);
                eat.detach(); 
                // t1.join();
                play_ate = true;
            }

            if (wait2())
            {
                draw_text(s, p, 0.0f, 0.0f, 0.0f);
            }
        }
        else
        {
            draw_text(s, p, 0.0f, 0.0f, 0.0f);
        }
        p.x = 3.0f;
        p.y = 0.5f;
        p.z = -7.0f;
        draw_text(h, p, 0.0f, 0.0f, 0.0f);
    }
    else
    {
        draw_menu();
    }

    scenario->camera_mode = old_cam;
}

void Game::update_highest_score()
{
    switch (level)
    {
        case EASY:
            if((score*10) > highest_scores[0]){
                highest_scores[0] = score * 10;
            }
        break;
        case MEDIUM:
            if((score*10) > highest_scores[1]){
                highest_scores[1] = score * 10;
            }
        break;
        case HARD:
            if((score*10) > highest_scores[2]){
                highest_scores[2] = score * 10;
            }
        break;
        case VERY_HARD:
            if((score*10) > highest_scores[3]){
                highest_scores[3] = score * 10;
            }
        break;
    }
    persist_scores();
}

void Game::run()
{
    if (paused || is_game_over || !is_running) return;

    if(is_running && !play_level && !is_game_over && !paused){
        switch (level)
        {
            case EASY:
            {
                std::thread level1(&Game::playSound, this , "./resources/Level1.mp3", -1, true , 0);
                level1.detach(); 
            }
            break;
            case MEDIUM:
            {
                std::thread level2(&Game::playSound, this , "./resources/Level2.mp3", -1, true , 0);
                level2.detach(); 
            }
            break;
            case HARD:
            {
                std::thread level3(&Game::playSound, this , "./resources/Level3.mp3", -1, true , 0);
                level3.detach(); 
            }
            break;
            case VERY_HARD:
            {
               std::thread level4(&Game::playSound, this , "./resources/Level4.mp3", -1, true , 0);
                level4.detach(); 
            }
            break;
        }
        play_level = true;
    }

    ObjectType o = scenario->has_collision(scenario->snake.head());
    ate = false;
    key_pressed = false;

    switch (o)
    {
        case NONE:
            scenario->snake.move();
        break;
        case FOOD1:
            ate = true;
            score++;
            scenario->snake.setFoodType(1);
            scenario->snake.grow(true);
            scenario->snake.move();
            scenario->change_food_pos();
            play_ate = false;
            switch (level)
            {
                case EASY:
                    if (scenario->snake.size() % 6 == 0)
                    {
                        scenario->add_barrier();
                    }
                break;
                case MEDIUM:
                    if (scenario->snake.size() % 4 == 0)
                    {
                        scenario->add_barrier();
                    }
                break;
                case HARD:
                    if (scenario->snake.size() % 4 == 0)
                    {
                        int v = random_range(1, 3);
                        for (int i = 0; i < v; ++i)
                        {
                            scenario->add_barrier();
                        }
                    }
                break;
                case VERY_HARD:
                {
                    int v = random_range(1, 5);
                    for (int i = 0; i < v; ++i)
                    {
                        scenario->add_barrier();
                    }
                }
                break;
            }
        break;
        case FOOD2:
            ate = true;
            score+= 2;
            scenario->snake.setFoodType(2);
            scenario->snake.grow(true);
            scenario->snake.move();
            scenario->change_food_pos();
            play_ate = false;
            switch (level)
            {
                case EASY:
                    if (scenario->snake.size() % 6 == 0)
                    {
                        scenario->add_barrier();
                    }
                break;
                case MEDIUM:
                    if (scenario->snake.size() % 4 == 0)
                    {
                        scenario->add_barrier();
                    }
                break;
                case HARD:
                    if (scenario->snake.size() % 4 == 0)
                    {
                        int v = random_range(1, 3);
                        for (int i = 0; i < v; ++i)
                        {
                            scenario->add_barrier();
                        }
                    }
                break;
                case VERY_HARD:
                {
                    int v = random_range(1, 5);
                    for (int i = 0; i < v; ++i)
                    {
                        scenario->add_barrier();
                    }
                }
                break;
            }
        break;
        case FOOD3:
            ate = true;
            score+= 3;
            scenario->snake.setFoodType(3);
            scenario->snake.grow(true);
            scenario->snake.move();
            scenario->change_food_pos();
            play_ate = false;
            switch (level)
            {
                case EASY:
                    if (scenario->snake.size() % 6 == 0)
                    {
                        scenario->add_barrier();
                    }
                break;
                case MEDIUM:
                    if (scenario->snake.size() % 4 == 0)
                    {
                        scenario->add_barrier();
                    }
                break;
                case HARD:
                    if (scenario->snake.size() % 4 == 0)
                    {
                        int v = random_range(1, 3);
                        for (int i = 0; i < v; ++i)
                        {
                            scenario->add_barrier();
                        }
                    }
                break;
                case VERY_HARD:
                {
                    int v = random_range(1, 5);
                    for (int i = 0; i < v; ++i)
                    {
                        scenario->add_barrier();
                    }
                }
                break;
            }
        break;
        case BARRIER:
        case BOARD:
        case SNAKE:
        {
            is_game_over = true;
            Mix_FreeMusic(music);
            music = 0;
        }
        break;
        default:
        break;
    }
}

void Game::on_key_pressed(int key)
{
    switch (key)
    {
        case KEY_CAMERA:
            if (!is_running) return;
            scenario->change_camera_pos();
        break;
        case KEY_PAUSE:
            if (is_running && !paused && !is_game_over){
                pause();
                std::thread pause(&Game::playSound, this , "./resources/Pause.wav", 1, false, 3);
                pause.detach();
            }
            else if(paused && !is_game_over){
                start();
                std::thread unPause(&Game::playSound, this , "./resources/Unpause.wav", 1, false, 4);
                unPause.detach();
            }
        break;
        case KEY_QUIT:
            exit(0);
        break;
        case KEY_SELECT:
            if(!is_running){
                reset();
                std::thread levelSelect(&Game::playSound, this , "./resources/LevelSelect.wav", 1, false, 5);
                levelSelect.detach(); 
            }
        break;
        // case KEY_START:
        //     if (!paused) return;
        //     start();
        // break;
        case KEY_STOP:
            if (!is_running) return;
            else{
                std::thread stop_(&Game::playSound, this , "./resources/Stop.wav", 1, false, 6);
                stop_.detach(); 
                stop();
                Mix_FreeMusic(music);
                music = 0;
            }
        break;
        case KEY_RESET:
            if (!is_running) return;
            reset();
        break;
        case KEY_LEFT:
            if (!is_running || key_pressed) return;
            scenario->snake.set_direction(LEFT);
            key_pressed = true;
        break;
        case KEY_UP:
            if (!is_running && !is_game_over)
            {
                std::thread menuScroll1(&Game::playSound, this , "./resources/MenuScroll.wav", 1, false, 7);
                menuScroll1.detach(); 
                level--;
                if (level < 1) level = 4;
            }
            if (!is_running || key_pressed) return;
            scenario->snake.set_direction(UP);
            key_pressed = true;
        break;
        case KEY_RIGHT:
            if (!is_running || key_pressed) return;
            scenario->snake.set_direction(RIGHT);
            key_pressed = true;
        break;
        case KEY_DOWN:
            if (!is_running && !is_game_over)
            {
                std::thread menuScroll2(&Game::playSound, this , "./resources/MenuScroll.wav", 1, false, 7);
                menuScroll2.detach(); 
                level++;
                if (level > 4) level = 1;
            }
            if (!is_running || key_pressed) return;
            scenario->snake.set_direction(DOWN);
            key_pressed = true;
        break;
        case KEY_0:
            if(!key0){
                glEnable(GL_LIGHT0);
                key0 = true;
            }else{
                glDisable(GL_LIGHT0);
                key0 = false;
            }
        break;
        case KEY_1:
            if(!key1){
                glEnable(GL_LIGHT1);
                key1 = true;
            }else{
                glDisable(GL_LIGHT1);
                key1 = false;
            }
        break;
        case KEY_2:
            if(!key2){
                glEnable(GL_LIGHT2);
                key2 = true;
            }else{
                glDisable(GL_LIGHT2);
                key2 = false;
            }     
        break;
        case KEY_3:
            if(!key3){
                glEnable(GL_LIGHT3);
                key3 = true;
            }else{
                glDisable(GL_LIGHT3);
                key3 = false;
            }  
        break;
        default:
            cout << "key = " << key << "\n";
        break;
    }
}

void Game::persist_scores()
{
    std::ofstream myfile;
    myfile.open ("highest_scores.txt");
    int i;
    for(i=0;i<4;i++){
        myfile << highest_scores[i] << endl;
    }
    myfile.close();
}

void Game::restore_highest_scores()
{
    string line;
    std::ifstream infile;
    infile.open ("highest_scores.txt");
    int i = 0;
    while(!infile.eof())
    {
        getline(infile, line);
        highest_scores[i++] = std::atoi(line.c_str());
    }

    infile.close();
}

bool Game::wait()
{
    bool wait = m > 0 && m < to_fps(fps, 30);
    m++;
    if (m > to_fps(fps, 30)) m = -to_fps(fps, 30);
    return wait;
}

bool Game::wait2()
{
    bool wait = m2 > 0 && m2 < to_fps(fps, 10);
    m2++;
    if (m2 > to_fps(fps, 10)) m2 = -to_fps(fps, 10);
    return wait;
}

void Game::calculateFPS(void)
{
    //  Increase frame count
    frameCount++;

    //  Get the number of milliseconds since glutInit called
    //  (or first call to glutGet(GLUT ELAPSED TIME)).
    currentTime = glutGet(GLUT_ELAPSED_TIME);

    //  Calculate time passed
    int timeInterval = currentTime - previousTime;

    if(timeInterval > 1000)
    {
        //  calculate the number of frames per second
        fps = frameCount / (timeInterval / 1000.0f);
        //  Set time
        previousTime = currentTime;
        //  Reset frame count
        frameCount = 0;
    }
}

bool Game::clock()
{
    // Speed up every time grows.
    tick += (level + (scenario->snake.size() / 10));
    bool wait = tick < to_fps(fps, 30);
    if (tick > to_fps(fps, 30)) tick = 0;
    return !wait;
}

bool Game::clock2()
{
    tick2++;
    bool wait = tick2 < to_fps(fps, 10);
    if (tick2 > to_fps(fps, 10)) tick2 = 0;
    return !wait;
}