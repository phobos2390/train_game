
#include <train_game/line_utils.h>
#include <glm/geometric.hpp>

namespace train_game
{
namespace utils
{

float rejection_dot(glm::vec2 p, glm::vec2 q)
{
    return p.x * q.y - p.y * q.x;
}
    
bool straddling(glm::vec2 b, glm::vec2 s, glm::vec2 e, float& r_s, float& r_e)
{
    bool is_straddling = false;
    r_s = rejection_dot(b, s);
    r_e = rejection_dot(b, e);
    if((r_s > 0) && (r_e <= 0))
    {
        is_straddling = true;
    }
    else if((r_s <= 0) && (r_e > 0))
    {
        is_straddling = true;
    }
    return is_straddling;
}

bool straddling(glm::vec2 b, glm::vec2 s, glm::vec2 e)
{
    float r_s = 0;
    float r_e = 0;
    return straddling(b,s,e,r_s,r_e);
}

bool intersects(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 line2_start, glm::vec2 line2_end, float& r_s1, float& r_e1, float& r_s2, float& r_e2)
{
    return straddling(line1_end - line1_start, line2_end - line1_start, line2_start - line1_start, r_s1, r_e1)
        && straddling(line2_end - line2_start, line1_end - line2_start, line1_start - line2_start, r_s2, r_e2);
}

glm::vec2 intersect(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 line2_start, glm::vec2 line2_end)
{
    float r_s1 = 0;
    float r_e1 = 0;
    float r_s2 = 0;
    float r_e2 = 0;
    glm::vec2 retval(0,0);
    if(intersects(line1_start, line1_end, line2_start, line2_end, r_s1, r_e1, r_s2, r_e2))
    {
        glm::vec2 p = line2_end - line1_start;
        glm::vec2 m = line1_end - line1_start;
        glm::vec2 q = line2_end - line2_start;
        glm::vec2 n (-q.y, q.x);
        
        float p_n = glm::dot(p,n);
        float m_n = glm::dot(m,n);
        
        if(m_n != 0)
        {
            retval = line1_start + (p_n * m) / m_n;
        }
    }
    return retval;
}

bool intersects(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 line2_start, glm::vec2 line2_end)
{
    float r_s1 = 0;
    float r_e1 = 0;
    float r_s2 = 0;
    float r_e2 = 0;
    return intersects(line1_start, line1_end, line2_start, line2_end, r_s1, r_e1, r_s2, r_e2);
}

float distance_square(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 test_point)
{
    float normal_squared = 1;
    return distance_square(line1_start, line1_end, test_point, normal_squared);
}

float distance_square(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 test_point, float& normal_squared)
{
    glm::vec2 line = line1_end - line1_start;
    glm::vec2 point = test_point - line1_start;
    glm::vec2 opp_point = test_point - line1_end;
    glm::vec2 normal(-line.y, line.x);
    
    float across = glm::dot(line, point);
    float retval = 0;
    normal_squared = 1;
    
    if(across < 0)
    {
        retval = glm::dot(point,point);
    }
    else if (across > glm::dot(line, line))
    {
        retval = glm::dot(opp_point,opp_point);
    }
    else
    {
        retval = glm::dot(normal, point) * glm::dot(normal, point);
        normal_squared = glm::dot(normal, normal);
    }
    return retval;
}

}
}
