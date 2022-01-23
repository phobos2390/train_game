/// @file train_game/junction.h

#ifndef TRAIN_GAME_JUNCTION_H
#define TRAIN_GAME_JUNCTION_H

#include <memory>
#include <track_data.h>
#include <vector>
#include <glm/glm.hpp>
#include <train_game/I_view.h>

namespace train_game
{

class junction
{
public:
    /// Creates class value
    junction();

    junction(const junction& other);

    junction( const track_quicktype::junction& junct
            , const std::vector<track_quicktype::point>& point_list);
    
    /// Removes class value
    virtual ~junction();

    static std::vector<junction> from_quicktype(track_quicktype::track_data& data);
    
    float closest_distance_squared(glm::vec2& check_point);
    
    void render(train_game::I_view& view);
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_p_impl;
};

}

#endif /* TRAIN_GAME_JUNCTION_H */
