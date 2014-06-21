#include "game.h"

#define to_fps(fps, value) value * fps / 60

Game::Game()
{
    fps = 60;
    currentTime = glutGet(GLUT_ELAPSED_TIME),
    previousTime = currentTime - 1001;
    is_game_over = false;
    is_running = false;
    paused = false;
    level = 1;
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
}

void Game::reset()
{
    m = to_fps(fps, 30);
    m2 = to_fps(fps, 10);

    score = 0;
    ate = false;
    is_game_over = false;
    paused = false;

    tick = 30;

    scenario->reset();
    is_running = true;
}

void Game::display()
{
    int old_cam = scenario->camera_mode;

    scenario->camera_mode = 3;
    scenario->set_camera();

    calculateFPS();
    Point p;

#ifdef DEBUG
    char s [50];
    sprintf(s, "FPS: %f", fps);

    p.x = -7.0f;
    p.y = 0.5f;
    p.z = 7.0f;
    draw_text(s, p, 0.0f, 0.0f, 0.0f);
#endif

    if (is_running)
    {
        scenario->camera_mode = old_cam;
        scenario->set_camera();

        scenario->draw_objects();

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
                p.x = -1.25f;
                p.y = 0.5f;
                p.z = 0.15f;
                draw_text("GAME OVER", p, 0.0f, 0.0f, 0.0f);
            }
        }

        sprintf(s, "SCORE: %d", score * 10);

        p.x = -1.0f;
        p.y = 0.5f;
        p.z = -7.0f;

        if (ate)
        {
            if (wait2())
            {
                draw_text(s, p, 0.0f, 0.0f, 0.0f);
            }
        }
        else
        {
            draw_text(s, p, 0.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        p.x = -2.3f;
        p.y = 0.5f;
        p.z = -5.0f;
        draw_text("SELECT YOUR LEVEL", p, 0.3f, 0.0f, 1.0f);

        p.x = -1.0f;
        p.y = 0.5f;
        p.z = -3.0f;
        if (level == VIRGIN)
        {
            p.x -= 0.3;
            draw_text("< VIRGIN >", p, 1.0f, 0.0f, 0.0f);
        }
        else
        {
            draw_text("VIRGIN", p, 0.0f, 0.0f, 0.0f);
        }

        p.x = -0.7f;
        p.y = 0.5f;
        p.z = -2.0f;

        if (level == JEDI)
        {
            p.x -= 0.3;
            draw_text("< JEDI >", p, 1.0f, 0.0f, 0.0f);
        }
        else
        {
            draw_text("JEDI", p, 0.0f, 0.0f, 0.0f);
        }

        p.x = -0.85f;
        p.y = 0.5f;
        p.z = -1.0f;

        if (level == ASIAN)
        {
            p.x -= 0.3;
            draw_text("< ASIAN >", p, 1.0f, 0.0f, 0.0f);
        }
        else
        {
            draw_text("ASIAN", p, 0.0f, 0.0f, 0.0f);
        }

        p.x = -1.7f;
        p.y = 0.5f;
        p.z = 0.0f;

        if (level == CHUCK_NORRIS)
        {
            p.x -= 0.3;
            draw_text("< CHUCK NORRIS >", p, 1.0f, 0.0f, 0.0f);
        }
        else
        {
            draw_text("CHUCK NORRIS", p, 0.0f, 0.0f, 0.0f);
        }
    }

    scenario->camera_mode = old_cam;
}

void Game::run()
{
    if (paused || is_game_over || !is_running) return;

    ObjectType o = scenario->has_collision(scenario->snake.head());
    ate = false;
    key_pressed = false;

    switch (o)
    {
        case NONE:
            scenario->snake.move();
        break;
        case FOOD:
            ate = true;
            score++;
            scenario->change_food_pos();
            scenario->snake.grow();
            scenario->snake.move();
            if (scenario->snake.size() % 6 == 0)
            {
                scenario->add_barrier();
            }
        break;
        case BARRIER:
        case BOARD:
        case SNAKE:
            is_game_over = true;
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
            if (!is_running) return;
            pause();
        break;
        case KEY_QUIT:
            exit(0);
        break;
        case KEY_SELECT:
            reset();
        break;
        case KEY_START:
            if (!paused) return;
            start();
        break;
        case KEY_STOP:
            if (!is_running) return;
            stop();
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
                level++;
                if (level > 4) level = 1;
            }
            if (!is_running || key_pressed) return;
            scenario->snake.set_direction(DOWN);
            key_pressed = true;
        break;
        default:
            cout << "key = " << key << "\n";
        break;
    }
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
    tick++;
    bool wait = tick < to_fps(fps, 30);
    if (tick > to_fps(fps, 30)) tick = 0;
    return !wait;
}