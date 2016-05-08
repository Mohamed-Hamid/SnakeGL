#ifndef BASE_H
    #define BASE_H
    #include "base.h"
#endif

#include "game.cpp"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <iostream>


int width  = 640,
    height = 640;

bool is_game_over = false,
     is_running   = false;

Game* game = NULL;

void keyboard(unsigned char key, int x, int y)
{
    game->on_key_pressed((int)key);
    glutPostRedisplay();
}

void keyboardSpecial(int key, int x, int y)
{
    game->on_key_pressed((int)key);
    glutPostRedisplay();
}

void init()
{
    glEnable(GL_LIGHT0);

    float light_diffuse1[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    float pos_light1[] = { -5.0f, 0.0f, 0.0f, 0.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, pos_light1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse1);
    // glEnable(GL_LIGHT1);

    float light_diffuse2[] = { 0.0f, 1.0f, 0.0f, 1.0f };
    float pos_light2[] = { 0.0f, 1.0f, 0.0f, 0.0f };
    glLightfv(GL_LIGHT2, GL_POSITION, pos_light2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE,  light_diffuse2);
    // glEnable(GL_LIGHT2);

    float light_diffuse3[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    float pos_light3[] = { 5.0f, 0.0f, 0.0f, 0.0f };
    glLightfv(GL_LIGHT3, GL_POSITION, pos_light3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE,  light_diffuse3);
    // glEnable(GL_LIGHT3);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    load_resources();

    game = new Game();
}

void display()
{
    /*glViewport(width / 4, height / 4, 200, 200);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();*/

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    game->display();

    glutSwapBuffers();

}

void resize(int w, int h)
{
    /*glViewport(0.0f, 0.0f, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();*/
    glutPostRedisplay();
}

int main(int argc, char** argv)
{

    // glutInit(&argc, argv);

    // glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    // glutInitWindowSize(width, height);
    // glutInitWindowPosition(100, 100);
    // glutCreateWindow("SnakeGL");
    // glutDisplayFunc(display);
    // glutIdleFunc(display);
    // glutReshapeFunc(resize);
    // glutKeyboardFunc(keyboard);
    // glutSpecialFunc(keyboardSpecial);
    // init();
    // glutMainLoop();



    // Init
   if (SDL_Init(SDL_INIT_AUDIO) != 0)
   {
      std::cerr << "SDL_Init ERROR: " << SDL_GetError() << std::endl;
      return -1;
   }

   // Open Audio device
   if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048) != 0)
   {
      std::cerr << "Mix_OpenAudio ERROR: " << Mix_GetError() << std::endl;
      return -1;
   }

   // Set Volume
   Mix_VolumeMusic(100);

   // Open Audio File
   Mix_Music* music = Mix_LoadMUS(argv[1]);

   if (music)
   {
      // Start Playback
      if (Mix_PlayMusic(music, 1) == 0)
      {
         // unsigned int startTime = SDL_GetTicks();

         // Wait
         while (Mix_PlayingMusic())
         {
            SDL_Delay(1000);
            // std::cout << "Time: " << (SDL_GetTicks() - startTime) / 1000 << std::endl;
         }
      }
      else
      {
         std::cerr << "Mix_PlayMusic ERROR: " << Mix_GetError() << std::endl;
      }

      // Free File
      Mix_FreeMusic(music);
      music = 0;
   }
   else
   {
      std::cerr << "Mix_LoadMuS ERROR: " << Mix_GetError() << std::endl;
   }

   // End
   Mix_CloseAudio();

    return 0;
}