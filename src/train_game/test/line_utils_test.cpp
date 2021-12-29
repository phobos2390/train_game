/// @file /home/James/phobos2390/train_game/src/train_game/test/track_test.cpp

#include <train_game/line_utils.h>
#include <catch2/catch.hpp>
#include <train_game/line_utils.h> // Testing include guard

using namespace train_game::utils;

TEST_CASE( "line_utils_test.basic_intersect", "line_utils_test.basic_intersect" )
{
    glm::vec2 g11(50, 0);
    glm::vec2 g12( 0,50);
    glm::vec2 g21( 0, 0);
    glm::vec2 g22(50,50);
    
    REQUIRE(train_game::utils::intersects(g11,g12,g21,g22));
}

TEST_CASE( "line_utils_test.intersect_point", "line_utils_test.intersect_point" )
{
    glm::vec2 g11(50, 0);
    glm::vec2 g12( 0,50);
    glm::vec2 g21( 0, 0);
    glm::vec2 g22(50,50);
    
    glm::vec2 intersection_point = train_game::utils::intersect(g11,g12,g21,g22);
    REQUIRE(intersection_point.x == 25);
    REQUIRE(intersection_point.y == 25);
}

TEST_CASE( "line_utils_test.intersect_point2", "line_utils_test.intersect_point2" )
{
    glm::vec2 g11(25, 0);
    glm::vec2 g12( 0,80);
    glm::vec2 g21( 0, 0);
    glm::vec2 g22(50,50);
    
    glm::vec2 intersection_point = train_game::utils::intersect(g11,g12,g21,g22);
    REQUIRE(intersection_point.x != 25);
    REQUIRE(intersection_point.y != 25);
}

TEST_CASE( "line_utils_test.distance1", "line_utils_test.distance1")
{
    glm::vec2 line_s(1,2);
    glm::vec2 line_e(7,2);
    glm::vec2 test_point_mid_above(3,1);
    glm::vec2 test_point_mid_below(3,3);
    float dist_a = train_game::utils::distance_square(line_s, line_e, test_point_mid_above);
    float dist_b = train_game::utils::distance_square(line_s, line_e, test_point_mid_below);
    
    REQUIRE(dist_a == dist_b);
    
}
        
TEST_CASE( "line_utils_test.distance2", "line_utils_test.distance2")
{
    glm::vec2 line_s(1,2);
    glm::vec2 line_e(7,2);
    glm::vec2 test_point_short(0,2);
    glm::vec2 test_point_long(8,2);
    float dist_a = train_game::utils::distance_square(line_s, line_e, test_point_short);
    float dist_b = train_game::utils::distance_square(line_s, line_e, test_point_long);
    
    REQUIRE(dist_a <= 1.05);
    REQUIRE(dist_b <= 1.05);
}
        
        