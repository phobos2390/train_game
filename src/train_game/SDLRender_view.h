/// @file train_game/SDLRender_view.h

#ifndef TRAIN_GAME_SDLRENDER_VIEW_H
#define TRAIN_GAME_SDLRENDER_VIEW_H

#include <train_game/I_view.h>
#include <sdl2/SDL.h>

namespace train_game
{

class SDLRender_view : public I_view
{
public:
    /// Creates class value
    SDLRender_view(std::string name);
    
    SDLRender_view(SDL_Renderer* p_renderer);

    /// Removes class value
    virtual ~SDLRender_view();
    
    virtual void per_frame_init();

    virtual void per_frame_finish();

    virtual int32_t get_error();
    
    virtual void render_line(glm::vec2 start, glm::vec2 end);

    virtual void render_polygon(std::vector<glm::vec2>& points);

    virtual void set_color(std::string alias);
    
    virtual void set_color(std::string alias, uint8_t red, uint8_t green, uint8_t blue);
private:
    struct Impl;
    Impl* m_p_impl;
};

}

#endif /* TRAIN_GAME_SDLRENDER_VIEW_H */
