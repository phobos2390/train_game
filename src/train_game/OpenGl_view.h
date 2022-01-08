/// @file train_game/OpenGl_view.h

#ifndef TRAIN_GAME_OPENGL_VIEW_H
#define TRAIN_GAME_OPENGL_VIEW_H

#include <train_game/I_view.h>
#include <string>

namespace train_game
{

class OpenGl_view:public I_view
{
public:
    /// Creates class value
    OpenGl_view(std::string window_name);

    /// Removes class value
    virtual ~OpenGl_view();
    
    void set_scale(float scale);
    
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

#endif /* TRAIN_GAME_OPENGL_VIEW_H */
