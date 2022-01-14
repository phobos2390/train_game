/// @file train_game/locomotive.h

#ifndef TRAIN_GAME_LOCOMOTIVE_H
#define TRAIN_GAME_LOCOMOTIVE_H

#include <train_game/I_view.h>
#include <glm/glm.hpp>
namespace train_game
{

class locomotive
{
public:
    /// Creates class value
    locomotive();

    /// Removes class value
    virtual ~locomotive();
    
    void render(I_view& view);
    
    void set_position(glm::vec2 center);
    
    void set_orientation(uint16_t angle_semcircle);
private:
    struct Impl;
    Impl* m_p_impl;
};

}

#endif /* TRAIN_GAME_LOCOMOTIVE_H */
