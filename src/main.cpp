/*
    main.c: example SDL2 entry point
    Copyright (C) 2018 Malte Kieﬂling

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <SDL.h>
#include <SDL_ttf.h>

#include <iostream>


int main(int argc, char* argv[])
{
#ifdef _DEBUG
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
#endif
    int rc = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_VIDEO);
    if (rc < 0) {
        std::cerr << "Cannot Init SDL! " << SDL_GetError() << std::endl;
        exit(1);
    }

    rc = TTF_Init();
    if (rc < 0) {
        std::cerr << "Cannot init TTF!" << TTF_GetError() << std::endl;
        exit(2);
    }

    // we are runnable. put in own scope so we run some destructors of stack objects before everything explodes
    {
        SDL_Window* window;
        SDL_Renderer* renderer;
        rc = SDL_CreateWindowAndRenderer(1024, 768, SDL_WINDOW_RESIZABLE, &window, &renderer);
        if (rc) {
            std::cerr << "Cannot create window and renderer" << SDL_GetError() << std::endl;
            exit(3);
        }
        auto basePathP = SDL_GetBasePath();
        SDL_Log("Base Path: %s", basePathP);
        SDL_free(basePathP);

        SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
        
        bool running = true;
        double deltaTime = 0.01;

        uint64_t now = SDL_GetPerformanceCounter();
        uint64_t last = 0;
        while (running) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false;
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_RenderPresent(renderer);
           
            deltaTime = (double)((now - last) / (double)SDL_GetPerformanceFrequency());
            last = now;
            now = SDL_GetPerformanceCounter();
            if (deltaTime < 0.008) {
                SDL_Delay(static_cast<int>((0.008-deltaTime)*1000));
            }
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }

    TTF_Quit();
    SDL_Quit();

    return 0;
}