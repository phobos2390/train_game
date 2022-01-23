/// @file train_game/track.h

#ifndef TRAIN_GAME_TRACK_H
#define TRAIN_GAME_TRACK_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <sdl2/SDL.h>
#include <sdl2/SDL_image.h>
#include <train_game/I_view.h>
#include <track_data.h>

namespace train_game
{
enum track_type
{
    track_type_unidirectional,
    track_type_bidirectional
};

struct glm_line2
{
    glm::vec2 m_start;
    glm::vec2 m_end;
};

class track
{
public:
    /// Creates class value
    track();
    
    /// Creates a new track with points list and track type
    track(std::vector<glm::vec2>& points, track_type type);
    
    /// copy constructor
    track(const track& other);
    
    static track from_bezier(glm::vec2 start, glm::vec2 end, std::vector<glm::vec2> control_points, uint32_t segments = 4);
    
    static std::vector<track> from_quicktype(track_quicktype::track_data& data);

    /// Removes class value
    virtual ~track();
    
    void add_point(glm::vec2 point);
    
    void render(train_game::I_view& view);
    
    bool intersect_split(glm_line2& line, std::vector<track>& other_tracks);
    
    float closest_distance_squared(glm::vec2& check_point);
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_p_impl;
};

}

#endif /* TRAIN_GAME_TRACK_H */
