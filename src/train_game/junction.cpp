/// @file train_game/junction.cpp

#include <train_game/junction.h>
#include <glm/glm.hpp>
#include <train_game/line_utils.h>

namespace train_game
{

struct junction::Impl
{
public:
    glm::vec2 m_parent;
    std::vector<glm::vec2> m_children;
    uint64_t m_selected_child;
    
    Impl()
        : m_parent(0,0)
        , m_children()
        , m_selected_child(0)
    {
    }
    
    Impl(const Impl& other)
        : m_parent(other.m_parent)
        , m_children()
        , m_selected_child(other.m_selected_child)
    {
        m_children.assign( other.m_children.begin()
                         , other.m_children.end());
    }
    
    Impl( const track_quicktype::junction& junct
        , const std::vector<track_quicktype::point>& point_list)
        : m_parent(point_list[junct.get_parent()].get_x(), point_list[junct.get_parent()].get_y())
        , m_children()
        , m_selected_child(junct.get_selected())
    {
        std::transform( junct.get_children().begin()
                      , junct.get_children().end()
                      , std::back_inserter(m_children)
                      , [point_list] (const uint64_t& index)
        {
            const track_quicktype::point& p = point_list[index];
            return glm::vec2(p.get_x(), p.get_y());
        });
    }

    virtual ~Impl(){}
    
    static bool validate_data( const track_quicktype::junction& junct
                             , const std::vector<track_quicktype::point>& point_list)
    {
        return (junct.get_parent() < point_list.size())
            && (junct.get_selected() < junct.get_children().size())
            && (junct.get_children().end() 
                    == std::find_if( junct.get_children().begin()
                                   , junct.get_children().end()
                                   , [point_list](const uint64_t& index)
                {
                    return index >= point_list.size();
                }));
    }
    
};

junction::junction()
    :m_p_impl(std::make_unique<Impl>())
{
}

junction::junction(const junction& other)
    :m_p_impl(std::make_unique<Impl>(*other.m_p_impl))
{
}

junction::junction( const track_quicktype::junction& junct
                  , const std::vector<track_quicktype::point>& point_list)
    :m_p_impl(std::make_unique<Impl>(junct, point_list))
{
    
}

std::vector<junction> junction::from_quicktype(track_quicktype::track_data& data)
{
    std::vector<junction> junct_list;
    std::transform( data.get_junctions().begin()
                  , data.get_junctions().end()
                  , std::back_inserter(junct_list)
                  , [data](const track_quicktype::junction& junct)
    {
        if(Impl::validate_data(junct, data.get_points()))
        {
            return junction(junct, data.get_points());
        }
        return junction();
    });
    return junct_list;
}

junction::~junction()
{
}

float junction::closest_distance_squared(glm::vec2& check_point)
{
    float minimum = std::numeric_limits<float>::infinity();
    glm::vec2 previous = m_p_impl->m_children.front();
    std::for_each(m_p_impl->m_children.begin()
                 ,m_p_impl->m_children.end()
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

void junction::render(train_game::I_view& view)
{
    uint64_t index = 0;
    std::for_each( m_p_impl->m_children.begin()
                 , m_p_impl->m_children.end()
                 ,[&view,&index,this]
                  (const glm::vec2& child)
    {
        if(m_p_impl->m_selected_child != index++)
        {
            view.set_color("junctions_unselected");
            view.render_line(m_p_impl->m_parent, child);
        }
    });
    if(m_p_impl->m_selected_child < m_p_impl->m_children.size())
    {
        view.set_color("junctions");
        view.render_line(m_p_impl->m_parent, m_p_impl->m_children[m_p_impl->m_selected_child]);
    }
}

}
