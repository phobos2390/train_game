#define CATCH_CONFIG_MAIN //This tells Catch to create a main file
#include <catch2/catch.hpp>
#include <train_game/map.h>

using namespace train_game;

TEST_CASE("boilerplate", "basic")
{
	REQUIRE( 1 == 1 );
}

TEST_CASE("map", "stack")
{
	Map test_map;
	Map* p_test_map = new Map;
	delete p_test_map;
}
