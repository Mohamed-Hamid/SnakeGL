//
// Requires the use of libSDL_mixer; available from libsdl devel packages
//
// sudo apt-get install libsdl-mixer1.2-dev libsdl1.2-dev
//
// To build:  g++ `sdl-config --cflags --libs` PlayMusic.cpp -lSDL -lSDL_mixer -o play
//
// To build:  g++ `sdl-config --cflags --libs` -lSDL -lSDL_mixer
// 
// To run:  ./play file.mp3>
//

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <iostream>

int main(int argc, char** argv)
{
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
         unsigned int startTime = SDL_GetTicks();

         // Wait
         while (Mix_PlayingMusic())
         {
            SDL_Delay(1000);
            std::cout << "Time: " << (SDL_GetTicks() - startTime) / 1000 << std::endl;
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
}