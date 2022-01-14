/// @file train_game/locomotive.cpp

#include <train_game/locomotive.h>
#include <algorithm>
#include <glm/glm.hpp>

namespace train_game
{

struct locomotive::Impl
{
public:
    glm::vec2 m_center;
    uint16_t m_angle_semicircle;
    std::vector<glm::vec2> m_points;
    
    Impl():m_center(0,0),m_points()
    {
        m_points.emplace_back(-4,-5);
        m_points.emplace_back(4,-5);
        m_points.emplace_back(0,5);
    }
    virtual ~Impl(){}
};

locomotive::locomotive()
    :m_p_impl(new Impl)
{
}

locomotive::~locomotive()
{
    delete m_p_impl;
    m_p_impl = 0;
}

//template<typename dest, typename src, typename iterator, typename select_func_f>
//std::vector<dest> select(iterator begin, iterator end, select_func_f select_func)
//{
//    std::vector<dest> ret_val;
//    std::for_each(begin, end, [&ret_val,select_func](const src& s)
//    {
//        ret_val.push_back(select_func(s));
//    });
//    return ret_val;
//}
//
//template<typename dest, typename src, typename select_func_f>
//void select( std::vector<src>::iterator begin
//           , std::vector<src>::iterator end
//           , std::vector<dest>& result
//           , select_func_f select_func)
//{
//    std::transform(begin, end, [&result, select_func](const src& s)
//    {
//        result.push_back(select_func(s));
//    });
//}

void locomotive::render(I_view& view)
{
    view.set_color("locomotives");
    float s = glm::sin(3.141592653589793238462 * m_p_impl->m_angle_semicircle*2.0/0x10000);
    float c = glm::cos(3.141592653589793238462 * m_p_impl->m_angle_semicircle*2.0/0x10000);
    
    glm::mat2 transform ( glm::vec2( c, s)
                        , glm::vec2(-s, c));
    
    std::vector<glm::vec2> locomotive_points;
    std::transform( m_p_impl->m_points.begin()
                  , m_p_impl->m_points.end()
                  , std::back_inserter(locomotive_points)
                  , [this,transform] (const glm::vec2& vec) -> glm::vec2
    {
        return transform*vec + m_p_impl->m_center;
    });
    
    view.render_polygon(locomotive_points);
}

void locomotive::set_position(glm::vec2 center)
{
    m_p_impl->m_center = center;
}
void locomotive::set_orientation(uint16_t angle_semcircle)
{
    m_p_impl->m_angle_semicircle = angle_semcircle;
}

}
