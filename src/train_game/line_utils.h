/// @file train_game/line_utils.h

#ifndef TRAIN_GAME_LINE_UTILS_H
#define TRAIN_GAME_LINE_UTILS_H

#include <glm/glm.hpp>

namespace train_game
{
namespace utils
{

glm::vec2 intersect(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 line2_start, glm::vec2 line2_end);

bool intersects(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 line2_start, glm::vec2 line2_end);

float distance_square(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 test_point);

float distance_square(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 test_point, float& normal_squared);
}
}

#endif /* TRAIN_GAME_LINE_UTILS_H */

