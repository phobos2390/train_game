/* 
 * File:   train_game/executable/loading_test.cpp
 * Author: James
 *
 * Created on August 29, 2020, 9:27 PM
 */

#include <iostream>
#include <sdl2/SDL.h>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <train_game/line_utils.h>
#include <track_data.h>
#include <fstream>
#include <vector>
#include <map>

int main(int argc, char** argv) 
{ 
    SDL_Window * p_window = NULL;
    SDL_Renderer * p_renderer = NULL;
    int32_t error = 0;
    
    if (argc > 1)
    {
        std::ifstream file_input;
        file_input.open(argv[1]);
        if(file_input.fail())
        {
            std::cout << "file doesn't exist: " << argv[1];
            error = -2;
        }
        else
        {
            error = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
        }
    
        if(error == 0)
        {
            p_window = SDL_CreateWindow( "Loading test"
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
                        
            nlohmann::json j;
            track::track_data track_data;
            std::map<std::string, track::color> colormap;
            try
            {
                file_input >> j;
                nlohmann::from_json(j, track_data);
                std::for_each(track_data.get_colors().begin()
                             ,track_data.get_colors().end()
                             ,[&colormap](const track::color& c)
                {
                    if(colormap.find(c.get_id()) == colormap.end())
                    {
                        colormap.emplace(c.get_id(), c);
                    }
                });
                if(colormap.find("default") == colormap.end())
                {
                    track::color c;
                    c.set_red(255);
                    c.set_green(255);
                    c.set_blue(255);
                    colormap.emplace("default", c);
                }
            }
            catch(std::exception& e)
            {
                SDL_Log("Exception loading track data: ");
                SDL_Log(e.what());
                continuing = false;
            }
            catch(...)
            {
                SDL_Log("Exception loading track data");
                continuing = false;
            }
            
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

                // render data
                
                track::color track_color;
                if(colormap.find("tracks") != colormap.end())
                {
                    track_color = colormap["tracks"];
                }
                else
                {
                    track_color = colormap["default"];
                }
                
                SDL_SetRenderDrawColor(p_renderer, track_color.get_red(), track_color.get_green(), track_color.get_blue(), SDL_ALPHA_OPAQUE);

                std::for_each(track_data.get_tracks().begin()
                             ,track_data.get_tracks().end()
                             ,[&p_renderer,track_data](const track::track& t)
                {
                    track::point previous = track_data.get_points()[t.get_points().front()];
                    std::for_each( t.get_points().begin()
                                 , t.get_points().end()
                                 , [&p_renderer,track_data,&previous](const uint64_t& index)
                    {
                        track::point current = track_data.get_points()[index];
                        SDL_RenderDrawLine(p_renderer, previous.get_x(), previous.get_y(), current.get_x(), current.get_y());
                        previous = current;
                    });
                });
                
                track::color junction_color;
                track::color junction_unselected_color;
                if(colormap.find("junctions") != colormap.end())
                {
                    junction_color = colormap["junctions"];
                }
                else
                {
                    junction_color = colormap["default"];
                }
                if(colormap.find("junctions_unselected") != colormap.end())
                {
                    junction_unselected_color = colormap["junctions_unselected"];
                }
                else
                {
                    junction_unselected_color = colormap["default"];
                }


                std::for_each(track_data.get_junctions().begin()
                             ,track_data.get_junctions().end()
                             ,[&p_renderer,track_data,junction_color,junction_unselected_color](const track::junction& j)
                {
                    int index = 0;
                    std::for_each(j.get_children().begin()
                                 ,j.get_children().end()
                                 ,[&p_renderer,track_data,junction_color,junction_unselected_color,j,&index]
                                  (const uint64_t& child)
                    {
                        if(j.get_selected() != index++)
                        {
                            SDL_SetRenderDrawColor(p_renderer, junction_unselected_color.get_red(), junction_unselected_color.get_green(), junction_unselected_color.get_blue(), SDL_ALPHA_OPAQUE);
                        }
                        else
                        {
                            SDL_SetRenderDrawColor(p_renderer, junction_color.get_red(), junction_color.get_green(), junction_color.get_blue(), SDL_ALPHA_OPAQUE);
                        }
                    
                        if( (j.get_parent() < track_data.get_points().size())
                         && (child < track_data.get_points().size()))
                        {
                            track::point p = track_data.get_points()[j.get_parent()];
                            track::point c = track_data.get_points()[child];
                            SDL_RenderDrawLine(p_renderer, p.get_x(), p.get_y(), c.get_x(), c.get_y());
                        }
                    });
                });

                // end render data
                
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
        std::cout << "Usage: ./loading_test 'filename.json' \n";
    }
    return 0;
}

