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
#include <train_game/track.h>
#include <train_game/junction.h>
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
            
            std::vector<train_game::track> m_tracks;
            std::vector<train_game::junction> m_junctions;
            
            std::vector<glm::vec2> mouse_shadow;
            mouse_shadow.push_back(glm::vec2(-5,0));
            mouse_shadow.push_back(glm::vec2(0,-5));
            mouse_shadow.push_back(glm::vec2(5,0));
            mouse_shadow.push_back(glm::vec2(0,5));
            mouse_shadow.push_back(glm::vec2(-5,0));

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
                m_tracks = train_game::track::from_quicktype(track_data);
                m_junctions = train_game::junction::from_quicktype(track_data);
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
                
                int32_t x = 0;
                int32_t y = 0;
                SDL_GetMouseState(&x, &y);
                glm::vec2 point(x,y);

                std::for_each( m_tracks.begin()
                             , m_tracks.end()
                             , [&view](train_game::track& track)
                {
                    track.render(view);
                });

                std::for_each( m_junctions.begin()
                             , m_junctions.end()
                             , [&view](train_game::junction& junct)
                {
                    junct.render(view);
                });

                train_game::locomotive loco;
                
                glm::vec2 loco_pos(170, 250);
                loco.set_position(loco_pos);
                loco.set_orientation(loco_orientation);
                loco.render(view);
                
                view.set_color("default");
                view.render_polygon(view.to_world_coord(point), mouse_shadow);
                
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

