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
#include <vector>
#include <algorithm>
#include <tuple>

int main(int argc, char** argv) 
{ 
    SDL_Window * p_window = NULL;
    SDL_Renderer * p_renderer = NULL;
    int32_t error = 0;
    
    if (argc > 0)
    {
        error = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    
        if(error == 0)
        {
            glm::vec2 c(250, 250);
            
            p_window = SDL_CreateWindow( "Segment drawing"
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
            
            bool drawing_line_segment = false;
            bool pressed_state = false;
            int32_t start_x=0;
            int32_t start_y=0;
            int32_t current_x=0;
            int32_t current_y=0;
            int32_t draw_x=0;
            int32_t draw_y=0;
            
            std::vector<std::pair<glm::vec2, glm::vec2> > line_segments;
            
            while(continuing)
            {
                while(SDL_PollEvent(&event)) 
                {
                    if (event.type == SDL_QUIT) 
                    {
                        printf("SDL_QUIT\n");
                        continuing = false;
                    }
                    else if(event.type == SDL_MOUSEBUTTONDOWN)
                    {
                        printf("Mouse button down: %d, %d\n", event.button.x, event.button.y);
                        current_x = event.button.x;
                        current_y = event.button.y;
                        if(!drawing_line_segment)
                        {
                            start_x = current_x;
                            start_y = current_y;
                        }
                        printf("%d,%d,%d\n", event.button.clicks, drawing_line_segment, pressed_state);
                    }
                    else if(event.type == SDL_MOUSEMOTION)
                    {
                        draw_x = event.button.x;
                        draw_y = event.button.y;
                        if((event.motion.state == SDL_PRESSED)
                         &&((draw_x != start_x)
                          ||(draw_y != start_y)))
                        {
                            drawing_line_segment = true;
                        }
//                        current_x = event.button.x;
//                        current_y = event.button.y;
//                        printf("Mouse moving: (%d, %d) (%d, %d)\n", start_x, start_y, draw_x, draw_y);
//                        printf("%d,%d,%d\n", event.button.clicks, drawing_line_segment, pressed_state);
                    }
                    else if(event.type == SDL_MOUSEBUTTONUP)
                    {
                        if((current_x == event.button.x)&&(current_y == event.button.y))
                        {
                            printf("Mouse unmoved: %d, %d, %d\n", event.button.clicks, event.button.x, event.button.y);
                            if(drawing_line_segment)
                            {
                                draw_x = event.button.x;
                                draw_y = event.button.y;
                                printf("Click Line: (%d,%d) (%d,%d)\n", start_x, start_y, event.button.x, event.button.y);
                                line_segments.emplace_back(glm::vec2(start_x, start_y), glm::vec2(event.button.x, event.button.y));
                            }
                            else
                            {
                                printf("Starting Line \n");
                            }
                            drawing_line_segment = !drawing_line_segment;
                        }
                        else 
                        {
                            printf("Drag Line: (%d,%d) (%d,%d)\n", start_x, start_y, event.button.x, event.button.y);
                            line_segments.emplace_back(glm::vec2(start_x, start_y), glm::vec2(event.button.x, event.button.y));
                            drawing_line_segment = false;
                        }
                        printf("%d,%d,%d\n", event.button.clicks, drawing_line_segment, pressed_state);
//                        drawing_line_segment = true;
//                        pressed_state = true;
                    }
//                    else
//                    {
//                        drawing_line_segment = !pressed_state;
//                    }
//                    }
                }
                
                SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(p_renderer);
                
                std::vector<std::tuple<uint8_t,uint8_t,uint8_t> > colors;
                colors.emplace_back(0,0,128);
                colors.emplace_back(0,0,255);
                colors.emplace_back(0,128,0);
                colors.emplace_back(0,255,0);
                colors.emplace_back(0,255,128);
                colors.emplace_back(0,128,255);
                colors.emplace_back(0,255,255);
                colors.emplace_back(128,0,0);
                colors.emplace_back(255,0,0);
                colors.emplace_back(255,0,128);
                colors.emplace_back(128,0,255);
                colors.emplace_back(255,0,255);
                colors.emplace_back(255,128,0);
                colors.emplace_back(128,255,0);
                colors.emplace_back(255,255,0);
                colors.emplace_back(255,255,128);
                colors.emplace_back(255,128,255);
                colors.emplace_back(255,128,128);
                colors.emplace_back(128,255,255);
                colors.emplace_back(128,255,128);
                colors.emplace_back(128,128,255);
                colors.emplace_back(128,128,128);
                colors.emplace_back(255,255,255);
                
                uint64_t color_index = 0;

                std::for_each(line_segments.begin()
                             ,line_segments.end()
                             ,[&p_renderer, &color_index, colors](std::pair<glm::vec2, glm::vec2>& line_segment)
                {
                    uint8_t red = 0;
                    uint8_t green = 0;
                    uint8_t blue = 0;
                    std::tie(red,green,blue) = colors[color_index];
                    SDL_SetRenderDrawColor(p_renderer, red, green, blue, SDL_ALPHA_OPAQUE);
                    SDL_RenderDrawLine(p_renderer,line_segment.first.x,line_segment.first.y, line_segment.second.x, line_segment.second.y);
                    SDL_RenderDrawLine(p_renderer,line_segment.first.x,line_segment.first.y, line_segment.second.x, line_segment.second.y);
                    color_index = (color_index + 1) % colors.size();
                });
                
                if(drawing_line_segment)
                {
                    uint8_t red = 0;
                    uint8_t green = 0;
                    uint8_t blue = 0;
                    std::tie(red,green,blue) = colors[color_index];
                    SDL_SetRenderDrawColor(p_renderer, red, green, blue, SDL_ALPHA_OPAQUE);
                    SDL_RenderDrawLine(p_renderer,start_x,start_y, draw_x, draw_y);
                }
                
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

