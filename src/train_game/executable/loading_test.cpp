/* 
 * File:   train_game/executable/loading_test.cpp
 * Author: James
 *
 * Created on August 29, 2020, 9:27 PM
 */

#include <iostream>
#include <sdl2/SDL.h>
#include <sdl2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <train_game/line_utils.h>
#include <track_data.h>
#include <fstream>
#include <vector>
#include <map>

static const float s_c_tau = 2 * 3.1415926535f;

void render_bezier( glm::vec2 start
                  , glm::vec2 end
                  , glm::vec2 curve_point1
                  , glm::vec2 curve_point2
                  , uint32_t segments
                  , bool verbose)
{
//    glTranslatef(-cx, -cy, 0);
//    glScalef(1.0/radius, 1.0/radius, 1.0);
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0,1.0,1.0);
    //B(t)=(1-t)^3*P0+3(1-t)^2*t*P1+3(1-t)t^2*P2+t^3*P3
    for(uint32_t i = 0; i <= segments; i++)
    {
        float t = i * 1.0 / segments;
        float c = (1-t);
        glm::vec2 position = c*c*c*start + 3*c*c*t*curve_point1 + 3*c*t*t*curve_point2 + t*t*t*end;
        float px=position.x;
        float py=position.y;
        px = glm::round(px);
        py = glm::round(py);
        if(verbose)
        {
            printf("(%f,%f), %f, %f\n", px, py, t, c);
        }
        glVertex2f(px, py);
    }
    glEnd();
}

void render_arc(float angle_semicircle, float semicircle_start, uint32_t segments, float cx, float cy, float radius, bool verbose)
{
//    glTranslatef(-cx, -cy, 0);
//    glScalef(1.0/radius, 1.0/radius, 1.0);
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0,1.0,1.0);
    
    for(uint32_t i = 0; i <= segments; i++)
    {
        float angle_current = (s_c_tau * semicircle_start) + (s_c_tau * angle_semicircle * i) / segments;
//        float angle_start_radians = (s_c_tau * semicircle_start)/segments;
//        float angle_step_radians = (s_c_tau * angle_semicircle)/segments;
//        float px = glm::cos(i*angle_step_radians + angle_start_radians) * radius + cx;
//        float py = glm::sin(i*angle_step_radians + angle_start_radians) * radius + cy;
        float px = glm::cos(angle_current) * radius + cx;
        float py = glm::sin(angle_current) * radius + cy;
        px = glm::round(px);
        py = glm::round(py);
        if(verbose)
        {
            printf("(%f,%f)\n", px, py);
        }
        glVertex2f(px, py);
    }
    glEnd();
}

int main(int argc, char** argv) 
{ 
    SDL_Window * p_window = NULL;
    //SDL_Renderer * p_renderer = NULL;
    int32_t error = 0;
    
    float height = 500;
    float width = 500;
    
    float scale = 2.0;
    
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
                                       , height * scale
                                       , width * scale
                                       , SDL_WINDOW_OPENGL);

            SDL_GLContext glcontext = SDL_GL_CreateContext(p_window);

            glClearColor(0,0,0,1);
            glClear(GL_COLOR_BUFFER_BIT);
            SDL_GL_SwapWindow(p_window);
            
            SDL_Event event;
            event.type = 0;
            
            bool verbose = true;
    
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
                
                glClearColor(0,0,0,1);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                        GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

                glLoadIdentity();
                glScalef(2.0f/width,2.0f/-height,1.0);
                glTranslatef(-width/2.0,-height/2.0,0);
                
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
                

                std::for_each(track_data.get_tracks().begin()
                             ,track_data.get_tracks().end()
                             ,[scale, track_color,track_data](const track::track& t)
                {
                    track::point previous = track_data.get_points()[t.get_points().front()];
                    std::for_each( t.get_points().begin()
                                 , t.get_points().end()
                                 , [scale, track_color,track_data,&previous](const uint64_t& index)
                    {
                        track::point current = track_data.get_points()[index];
                        glLineWidth(2.5*scale);
                        glBegin(GL_LINES);
                        glColor3ub(track_color.get_red(), track_color.get_green(), track_color.get_blue());
                        glVertex2f(previous.get_x(),previous.get_y());
                        glVertex2f(current.get_x(),current.get_y());
                        glEnd();
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
                             ,[scale,track_data,junction_color,junction_unselected_color](const track::junction& j)
                {
                    int index = 0;
                    std::for_each(j.get_children().begin()
                                 ,j.get_children().end()
                                 ,[scale,track_data,junction_color,junction_unselected_color,j,&index]
                                  (const uint64_t& child)
                    {
                        track::color current_color;
                        if(j.get_selected() != index++)
                        {
                            current_color = junction_unselected_color;
                        }
                        else
                        {
                            current_color = junction_color;
                        }
                    
                        if( (j.get_parent() < track_data.get_points().size())
                         && (child < track_data.get_points().size()))
                        {
                            track::point p = track_data.get_points()[j.get_parent()];
                            track::point c = track_data.get_points()[child];
                            glLineWidth(2.5 * scale);
                            glBegin(GL_LINES);
                            glColor3ub(current_color.get_red(), current_color.get_green(), current_color.get_blue());
                            glVertex2f(p.get_x(),p.get_y());
                            glVertex2f(c.get_x(),c.get_y());
                            glEnd();
                        }
                    });
                });
                
                //1.0/2.0 - 1.0/16.0
                
                glm::vec2 start (190, 370);
                glm::vec2 end (225, 415);
                glm::vec2 c1 (185,395);
                glm::vec2 c2 (185,395);
                
                render_bezier(start, end, c1, c2, 5, verbose);
                
                render_arc(-1.0/8.0, 0, 16, 390, 150, 100, verbose);
//                render_arc(3.0/8.0,       0, 16, 390, 150, 100, verbose);
//                render_arc(1.0/4.0, 1.0/2.0, 16, 390, 150, 100, verbose);
//                render_arc(1.0/16.0, 1.0/2.0 - 1.0/16.0, 16, 390, 150, 100, verbose);
                verbose = false;
                
                // end render data
                
                SDL_GL_SwapWindow(p_window);
                
                SDL_Delay(10);
            }

            //SDL_DestroyRenderer(p_renderer);
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

