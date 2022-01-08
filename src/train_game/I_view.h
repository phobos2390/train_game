/// @file train_game/I_view.h

#ifndef TRAIN_GAME_I_VIEW_H
#define TRAIN_GAME_I_VIEW_H

#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace train_game
{

class I_view
{
public:
    /// Creates class value
    I_view(){}

    /// Removes class value
    virtual ~I_view(){}
    
    virtual void per_frame_init()=0;

    virtual void per_frame_finish()=0;

    virtual int32_t get_error()=0;
    
    virtual void render_line(glm::vec2 start, glm::vec2 end) = 0;

    virtual void render_polygon(std::vector<glm::vec2>& points) = 0;

    virtual void set_color(std::string alias) = 0;
    
    virtual void set_color(std::string alias, uint8_t red, uint8_t green, uint8_t blue) = 0;
};

}

#endif /* TRAIN_GAME_I_VIEW_H */
