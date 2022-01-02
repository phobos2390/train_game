/// @file train_game/track.h

#ifndef TRAIN_GAME_TRACK_H
#define TRAIN_GAME_TRACK_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <sdl2/SDL.h>
#include <sdl2/SDL_image.h>

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

    /// Removes class value
    virtual ~track();
    
    void add_point(glm::vec2 point);
    
    void render(SDL_Renderer* p_renderer);
    
    bool intersect_split(glm_line2& line, std::vector<track>& other_tracks);
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_p_impl;
};

}

#endif /* TRAIN_GAME_TRACK_H */
