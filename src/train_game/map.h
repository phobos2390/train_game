#ifndef PHOBOS2390_TRAIN_GAME_MAP_H
#define PHOBOS2390_TRAIN_GAME_MAP_H

#include <memory>

namespace train_game
{

class Map
{
public:
	Map();
	virtual ~Map();
private:
	struct Impl;
	std::unique_ptr<Impl> m_p_impl;
};

}
#endif /* PHOBOS2390_TRAIN_GAME_MAP_H */
