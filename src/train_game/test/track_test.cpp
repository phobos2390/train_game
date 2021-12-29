/// @file train_game/test/track_test.cpp

#include <train_game/track.h>
#include <catch2/catch.hpp>
#include <train_game/track.h> // Testing include guard

using namespace train_game;

TEST_CASE( "track_test", "stack" )
{
    track c;
}

TEST_CASE("render_test", "basic_track_render_test")
{    
    SDL_Surface * p_surface = SDL_CreateRGBSurface( 0
                                                  , 100
                                                  , 100
                                                  , 0x20
                                                  , 0x00FF0000
                                                  , 0x0000FF00
                                                  , 0x000000FF
                                                  , 0xFF000000);

    if(p_surface == NULL)
    {
        SDL_Log("Surface undefined %s", SDL_GetError());
        REQUIRE(false);
    }
    SDL_Renderer* p_renderer = SDL_CreateSoftwareRenderer(p_surface);
    if(p_renderer == NULL)
    {
        SDL_Log("Renderer undefined %s", SDL_GetError());
        REQUIRE(false);
    }
    SDL_Texture* p_texture = SDL_CreateTextureFromSurface(p_renderer, p_surface);
    if(p_texture == NULL)
    {
        SDL_Log("Texture undefined %s", SDL_GetError());
        REQUIRE(false);
    }
    
    SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(p_renderer);

    SDL_SetRenderDrawColor(p_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    
    glm::vec2 start (25, 25);
    
    track t;
    t.add_point(glm::vec2(10,10));
    t.add_point(glm::vec2(36,36));
    t.add_point(glm::vec2(45,51));
    t.add_point(glm::vec2(45,60));
    t.add_point(glm::vec2(40,70));
    
    t.render(p_renderer);
    
    SDL_RenderPresent(p_renderer);
    IMG_SavePNG(p_surface, "test_files/basic_track_render_test.png");
    
    SDL_DestroyRenderer(p_renderer);
    SDL_DestroyTexture(p_texture);
    SDL_FreeSurface(p_surface);
}
