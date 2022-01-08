/// @file train_game/SDLRender_view.cpp

#include <train_game/SDLRender_view.h>
#include <sdl2/SDL.h>
#include <sdl2/SDL_Image.h>

namespace train_game
{

struct SDLRender_view::Impl
{
public:
    SDL_Renderer* m_p_renderer;
    Impl(std::string name)
        : m_p_renderer(NULL)
    {}
    Impl(SDL_Renderer* p_renderer)
        : m_p_renderer(p_renderer)
    {}
    virtual ~Impl(){}
};

SDLRender_view::SDLRender_view(std::string name)
    :m_p_impl(new Impl(name))
{
}

SDLRender_view::SDLRender_view(SDL_Renderer* p_renderer)
    :m_p_impl(new Impl(p_renderer))
{
    
}


SDLRender_view::~SDLRender_view()
{
    delete m_p_impl;
    m_p_impl = 0;
}

void SDLRender_view::per_frame_init(){}

void SDLRender_view::per_frame_finish(){}

int32_t SDLRender_view::get_error(){return -1;}

void SDLRender_view::render_line(glm::vec2 start, glm::vec2 end)
{
    SDL_RenderDrawLine(m_p_impl->m_p_renderer, start.x, start.y, end.x, end.y);
}

void SDLRender_view::render_polygon(std::vector<glm::vec2>& points){}

void SDLRender_view::set_color(std::string alias){}
    
void SDLRender_view::set_color(std::string alias, uint8_t red, uint8_t green, uint8_t blue){}
}
