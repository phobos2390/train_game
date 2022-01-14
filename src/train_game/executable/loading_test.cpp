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
#include <train_game/OpenGl_view.h>
#include <train_game/locomotive.h>
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
//    SDL_Window * p_window = NULL;
//    //SDL_Renderer * p_renderer = NULL;
    int32_t error = 0;
//    
//    float height = 500;
//    float width = 500;
    
    float scale = 1.0;
    
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
            
        }
        
        train_game::OpenGl_view view("Loading test");

        uint16_t loco_orientation = 0;
                
        if(view.get_error() == 0)
        {
            SDL_Event event;
            event.type = 0;
            
            bool verbose = true;
    
            bool continuing = true;
                        
            nlohmann::json j;
            track_quicktype::track_data track_data;
            std::map<std::string, track_quicktype::color> colormap;
            try
            {
                file_input >> j;
                nlohmann::from_json(j, track_data);
                std::for_each(track_data.get_colors().begin()
                             ,track_data.get_colors().end()
                             ,[&view,&colormap](const track_quicktype::color& c)
                {
                    view.set_color(c.get_id(),c.get_red(),c.get_green(),c.get_blue());
                });
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
                    else if(event.type == SDL_MOUSEWHEEL)
                    {
                        scale -= event.button.x / 10.0;
                        if(scale < 0.1)
                        {
                            scale = 0.1;
                        }
                        if(scale > 10.0)
                        {
                            scale = 10.0;
                        }
                        view.set_scale(scale);
                    }
                    else if( (event.type == SDL_MOUSEMOTION)
                          && (SDL_BUTTON_LEFT == (SDL_GetMouseState(0,0) & SDL_BUTTON_LEFT)))
                    {
                        
                        view.move_center(event.motion.xrel, event.motion.yrel);
                    }
                }
                
                view.per_frame_init();
                
                // render data

                std::for_each(track_data.get_tracks().begin()
                             ,track_data.get_tracks().end()
                             ,[&view, scale, track_data](const track_quicktype::track& t)
                {
                    track_quicktype::point previous = track_data.get_points()[t.get_points().front()];
                    std::for_each( t.get_points().begin()
                                 , t.get_points().end()
                                 , [&view, scale, track_data,&previous](const uint64_t& index)
                    {
                        track_quicktype::point current = track_data.get_points()[index];
                        view.set_color("tracks");
                        view.render_line(glm::vec2(previous.get_x(), previous.get_y()), glm::vec2(current.get_x(), current.get_y()));
                        previous = current;
                    });
                });

                std::for_each(track_data.get_junctions().begin()
                             ,track_data.get_junctions().end()
                             ,[&view,scale,track_data](const track_quicktype::junction& j)
                {
                    int index = 0;
                    std::for_each(j.get_children().begin()
                                 ,j.get_children().end()
                                 ,[&view,scale,track_data,j,&index]
                                  (const uint64_t& child)
                    {
                        if(j.get_selected() != index++)
                        {
                            view.set_color("junctions_unselected");
                        }
                        else
                        {
                            view.set_color("junctions");
                        }
                    
                        if( (j.get_parent() < track_data.get_points().size())
                         && (child < track_data.get_points().size()))
                        {
                            track_quicktype::point p = track_data.get_points()[j.get_parent()];
                            track_quicktype::point c = track_data.get_points()[child];
                            view.render_line(glm::vec2(p.get_x(),p.get_y()), glm::vec2(c.get_x(),c.get_y()));
                        }
                    });
                });
                
                train_game::locomotive loco;
                
                glm::vec2 loco_pos(170, 250);
                loco.set_position(loco_pos);
                loco.set_orientation(loco_orientation);
                //loco_orientation += 0x10000/0x168; // d360 == x168
                
//                glm::vec2 pos_front(0,10);
//                glm::vec2 pos_back_1(-4,-10);
//                glm::vec2 pos_back_2(4,-10);
//                
//                std::vector<glm::vec2> locomotive_polygon;
//                locomotive_polygon.push_back(loco_pos + pos_front);
//                locomotive_polygon.push_back(loco_pos + pos_back_1);
//                locomotive_polygon.push_back(loco_pos + pos_back_2);
                
//                view.set_color("locomotives");
                loco.render(view);
//                view.render_polygon(locomotive_polygon);
                
                //1.0/2.0 - 1.0/16.0
                
                glm::vec2 start (190, 370);
                glm::vec2 end (215, 405);
                glm::vec2 c1 (197,397);
                glm::vec2 c2 (197,397);
                
                render_bezier(start, end, c1, c2, 4, verbose);
                
                render_arc(-1.0/8.0, 0, 16, 390, 150, 100, verbose);
//                render_arc(3.0/8.0,       0, 16, 390, 150, 100, verbose);
//                render_arc(1.0/4.0, 1.0/2.0, 16, 390, 150, 100, verbose);
//                render_arc(1.0/16.0, 1.0/2.0 - 1.0/16.0, 16, 390, 150, 100, verbose);
                verbose = false;
                
                // end render data
                
                view.per_frame_finish();
                
                SDL_Delay(10);
            }
            
            SDL_Quit();
        
        }
        else
        {
            SDL_Log("Error: %s, %s\n", SDL_GetError(), view.get_error());
        }
    }
    else
    {
        std::cout << "Usage: ./loading_test 'filename.json' \n";
    }
    return 0;
}

