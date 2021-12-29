/// @file train_game/track.cpp

#include <train_game/track.h>
#include <functional>
#include <algorithm>

namespace train_game
{

struct track::Impl
{
public:
    std::vector<glm::vec2> m_points;
    track_type m_type;
    
    Impl()
        :m_points()
        ,m_type()
    {
    }

    Impl(std::vector<glm::vec2>& points, track_type type)
        :m_points(points)
        ,m_type(type)
    {
    }
    virtual ~Impl(){}
};

track::track()
    :m_p_impl(std::make_unique<Impl>())
{
}

track::track(std::vector<glm::vec2>& points, track_type type)
    :m_p_impl(std::make_unique<Impl>(points, type))
{
    
}

track::~track()
{
}

void track::add_point(glm::vec2 point)
{
    m_p_impl->m_points.push_back(point);
}

void track::render(SDL_Renderer* p_renderer)
{
    if(m_p_impl->m_points.size() > 0)
    {
        glm::vec2 prev = m_p_impl->m_points.front();
        std::for_each(m_p_impl->m_points.begin()
                     ,m_p_impl->m_points.end()
                     ,[&prev, p_renderer](glm::vec2& new_point)
        {
            if((prev.x != new_point.x) || (prev.y != new_point.y))
            {
                SDL_RenderDrawLine(p_renderer, prev.x, prev.y, new_point.x, new_point.y);
            }
            prev = new_point;
        });
    }
}


}
