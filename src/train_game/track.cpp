/// @file train_game/track.cpp

#include <train_game/track.h>
#include <functional>
#include <algorithm>
#include <train_game/line_utils.h>

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

track::track(const track& other)
    :m_p_impl(std::make_unique<Impl>(other.m_p_impl->m_points,other.m_p_impl->m_type))
{
}

track::~track()
{
}

void track::add_point(glm::vec2 point)
{
    m_p_impl->m_points.push_back(point);
}

void track::render(train_game::I_view& view)
{
    if(m_p_impl->m_points.size() > 0)
    {
        glm::vec2 prev = m_p_impl->m_points.front();
        std::for_each(m_p_impl->m_points.begin()
                     ,m_p_impl->m_points.end()
                     ,[&prev, &view](glm::vec2& new_point)
        {
            if((prev.x != new_point.x) || (prev.y != new_point.y))
            {
                view.render_line(prev,new_point);
            }
            prev = new_point;
        });
    }
}

bool track::intersect_split(glm_line2& line, std::vector<track>& other)
{
    bool is_intersecting = false;
    glm::vec2 prev = m_p_impl->m_points.front();
    glm::vec2 separation_point;
    std::vector<glm::vec2> first;
    std::for_each( m_p_impl->m_points.begin()
                 , m_p_impl->m_points.end()
                 , [line,&first,&other,&separation_point,&prev,&is_intersecting](glm::vec2& current)
    {
        if(train_game::utils::intersects(line.m_start,line.m_end,prev,current))
        {
            separation_point = train_game::utils::intersect(line.m_start,line.m_end,prev,current);
            if(is_intersecting)
            {
                other.back().add_point(separation_point);
            }
            else
            {
                first.push_back(separation_point);
            }
            other.emplace_back(track());
            other.back().add_point(separation_point);
            is_intersecting = true;
        }
        if(is_intersecting)
        {
            other.back().add_point(current);
        }
        else
        {
            first.push_back(current);
        }
        prev = current;
    });
    if(is_intersecting)
    {
        m_p_impl->m_points.assign(first.begin(), first.end());
    }
    return is_intersecting;
}
}
