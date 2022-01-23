/// @file train_game/track.cpp

#include <train_game/track.h>
#include <functional>
#include <algorithm>
#include <train_game/line_utils.h>
#include <limits>

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

uint64_t factorial(uint8_t value)
{
    uint64_t retval = 1;
    for(uint8_t i = 1; i < value; i++)
    {
        retval *= (i+1);
    }
    return retval;
}

uint32_t combination(uint32_t max, uint32_t value)
{
    uint32_t denom = (factorial(value) * factorial(max - value));
    return factorial(max) / denom;
}

track track::from_bezier(glm::vec2 start, glm::vec2 end, std::vector<glm::vec2> control_points, uint32_t segments)
{
    track ret_track;
    std::vector<glm::vec2> points;
    points.push_back(start);
    points.insert(points.end(),control_points.begin(), control_points.end());
    points.push_back(end);
    for(uint32_t i = 0; i <= segments; i++)
    {
        float t = (i*1.0)/segments;
        float r = 1.0 - t;
        glm::vec2 position(0,0);
        for(uint32_t j = 0; j < points.size(); j++)
        {
            float tt = 1.0;
            for(uint32_t k = j; k < points.size() - 1; k++)
            {
                tt *= r;
            }
            for(uint32_t l = 0; l < j; l++)
            {
                tt *= t;
            }
            position += combination(points.size() - 1, j) * tt * points[j];
        }
        ret_track.add_point(position);
    }
    return ret_track;
}

std::vector<track> track::from_quicktype(track_quicktype::track_data& data)
{
    std::vector<track> track_list;
    std::for_each( data.get_tracks().begin()
                 , data.get_tracks().end()
                 ,[&track_list, data](const track_quicktype::track& t)
    {
        track new_track;
        std::for_each( t.get_points().begin()
                     , t.get_points().end()
                     ,[&new_track,data](const uint64_t& p_i)
        {
            track_quicktype::point p = data.get_points()[p_i];
            new_track.add_point(glm::vec2(p.get_x(),p.get_y()));
        });
        track_list.push_back(new_track);
    });
    return track_list;
}

void track::add_point(glm::vec2 point)
{
    m_p_impl->m_points.push_back(point);
}

void track::render(train_game::I_view& view)
{
    view.set_color("tracks");
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

float track::closest_distance_squared(glm::vec2& check_point)
{
    float minimum = std::numeric_limits<float>::infinity();
    glm::vec2 previous = m_p_impl->m_points.front();
    std::for_each(m_p_impl->m_points.begin()
                 ,m_p_impl->m_points.end()
                 ,[check_point,&previous,&minimum](const glm::vec2& current)
    {
        float dist_sqr = train_game::utils::distance_square(previous, current, check_point);
        if(dist_sqr < minimum)
        {
            minimum = dist_sqr;
        }
        previous = current;
    });
    return minimum;
}
}
