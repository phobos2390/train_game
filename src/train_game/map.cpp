#include <train_game/map.h>

namespace train_game
{

struct Map::Impl
{
	Impl()
	{
	}

	virtual ~Impl()
	{
	}
};

Map::Map()
:m_p_impl(std::make_unique<Impl>())
{
}

Map::~Map()
{
}

}
