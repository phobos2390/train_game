/* 
 * File:   hex_grid_demo.cpp
 * Author: James
 *
 * Created on August 29, 2020, 9:27 PM
 */

#include <iostream>
#include <sdl2/SDL.h>
#include <glm/glm.hpp>
#include <train_game/line_utils.h>

int main(int argc, char** argv) 
{ 
    SDL_Window * p_window = NULL;
    SDL_Renderer * p_renderer = NULL;
    int32_t error = 0;
    
    if (argc > 2)
    {
        error = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    
        if(error == 0)
        {
            glm::vec2 c(250, 250);
            
            p_window = SDL_CreateWindow( "Distance to segment demo"
                                       , SDL_WINDOWPOS_UNDEFINED
                                       , SDL_WINDOWPOS_UNDEFINED
                                       , 500
                                       , 500
                                       , 0);

            p_renderer = SDL_CreateRenderer( p_window
                                           , -1
                                           , SDL_RENDERER_SOFTWARE);
            
            SDL_Event event;
            event.type = 0;
    
            bool continuing = true;
            
            float l1x = 0;
            float l1y = 0;
            float l2x = 0;
            float l2y = 0;
            float hysteresis = 0;
            
            sscanf(argv[1],"(%f,%f)", &l1x, &l1y);
            sscanf(argv[2],"(%f,%f)", &l2x, &l2y);
            sscanf(argv[3],"%f", &hysteresis);
            
            hysteresis *= hysteresis;
            
            while(continuing)
            {
                while(SDL_PollEvent(&event)) 
                {
                    if (event.type == SDL_QUIT) 
                    {
                        printf("SDL_QUIT\n");
                        continuing = false;
                    }
                }
                
                SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(p_renderer);

                int32_t x = 0;
                int32_t y = 0;
                
                SDL_GetMouseState(&x, &y);
                
                glm::vec2 m(x,y);
                glm::vec2 l1s(l1x,l1y);
                glm::vec2 l1e(l2x,l2y);
                float norm_sqr = 1;
                
                float dist_sqr = train_game::utils::distance_square(l1s,l1e,m,norm_sqr);
                
                if(dist_sqr <= norm_sqr * hysteresis)
                {
                    SDL_SetRenderDrawColor(p_renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
                }
                else
                {
                    SDL_SetRenderDrawColor(p_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                }
                
                SDL_RenderDrawLine(p_renderer, l1x, l1y, l2x, l2y);
                
                SDL_RenderPresent(p_renderer);
                
                SDL_Delay(10);
            }

            SDL_DestroyRenderer(p_renderer);
            SDL_DestroyWindow(p_window);
            SDL_Quit();
        
        }
        else
        {
            SDL_Log("Error: %s\n", SDL_GetError());
        }
    }
    else
    {
        std::cout << "Usage: ./demo '(1,2)' '(2,1)' '1.0' \n";
    }
    return 0;
}

