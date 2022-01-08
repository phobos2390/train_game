/// @file train_game/test/track_test.cpp

#include <train_game/track.h>
#include <catch2/catch.hpp>
#include <train_game/track.h> // Testing include guard
#include <train_game/SDLRender_view.h>

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
    
    SDLRender_view view(p_renderer);
    
    glm::vec2 start (25, 25);
    
    track t;
    t.add_point(glm::vec2(10,10));
    t.add_point(glm::vec2(36,36));
    t.add_point(glm::vec2(45,51));
    t.add_point(glm::vec2(45,60));
    t.add_point(glm::vec2(40,70));
    
    t.render(view);
    
    SDL_RenderPresent(p_renderer);
    IMG_SavePNG(p_surface, "test_files/basic_track_render_test.png");
    
    SDL_DestroyRenderer(p_renderer);
    SDL_DestroyTexture(p_texture);
    SDL_FreeSurface(p_surface);
}

TEST_CASE("render_test.basic_track_split_test", "render_test.basic_track_split_test")
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

    SDLRender_view view(p_renderer);
    
    glm::vec2 start (25, 25);
    
    track t;
    t.add_point(glm::vec2(10,10));
    t.add_point(glm::vec2(36,36));
    t.add_point(glm::vec2(45,51));
    t.add_point(glm::vec2(45,60));
    t.add_point(glm::vec2(40,70));
    
    std::vector<track> others;
    
    train_game::glm_line2 splitting_line { glm::vec2(40,55), glm::vec2(50,55) };
    
    REQUIRE(t.intersect_split(splitting_line, others));
    
    SDL_SetRenderDrawColor(p_renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);

    t.render(view);
    REQUIRE(others.size() == 1);
    
    uint8_t rgb [][3] = {{255,0,255},{0,255,255}};
    size_t s_rgb = sizeof(rgb)/sizeof(rgb[0]);
    size_t i = 0;
    
    std::for_each(others.begin()
                 ,others.end()
                 ,[&view,&p_renderer,s_rgb,&i,rgb](track& subtrack)
    {
        SDL_SetRenderDrawColor(p_renderer, rgb[i][0], rgb[i][1], rgb[i][2], SDL_ALPHA_OPAQUE);
        i = (i + 1) % s_rgb;
        subtrack.render(view);
    });
    
    SDL_SetRenderDrawColor(p_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    
    SDL_RenderDrawLine(p_renderer, splitting_line.m_start.x, splitting_line.m_start.y, splitting_line.m_end.x, splitting_line.m_end.y);
    
    SDL_RenderPresent(p_renderer);
    IMG_SavePNG(p_surface, "test_files/basic_track_split_test.png");
    
    SDL_DestroyRenderer(p_renderer);
    SDL_DestroyTexture(p_texture);
    SDL_FreeSurface(p_surface);
}

TEST_CASE("render_test.complex_split_test", "render_test.complex_split_test")
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
    
    SDLRender_view view(p_renderer);
    
    glm::vec2 start (25, 25);
    
    track t;
    t.add_point(glm::vec2(10,10));
    t.add_point(glm::vec2(10,20));
    t.add_point(glm::vec2(20,20));
    t.add_point(glm::vec2(20,10));
    t.add_point(glm::vec2(30,10));
    t.add_point(glm::vec2(30,20));
    t.add_point(glm::vec2(40,20));
    t.add_point(glm::vec2(40,10));
    t.add_point(glm::vec2(50,10));
    t.add_point(glm::vec2(50,20));
    t.add_point(glm::vec2(60,20));
    t.add_point(glm::vec2(60,10));
    
    std::vector<track> others;
    
    train_game::glm_line2 splitting_line { glm::vec2(5,15), glm::vec2(80,15) };
    
    REQUIRE(t.intersect_split(splitting_line, others));
    
    SDL_SetRenderDrawColor(p_renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);

    t.render(view);
    REQUIRE(others.size() >= 1);
    
    uint8_t rgb [][3] = {{255,0,255},{0,255,255},{0,255,0},{255,255,0},{255,0,0},{255,128,128},{0,0,255}};
    size_t s_rgb = sizeof(rgb)/sizeof(rgb[0]);
    size_t i = 0;
    
    std::for_each(others.begin()
                 ,others.end()
                 ,[&view,&p_renderer,s_rgb,&i,rgb](track& subtrack)
    {
        SDL_SetRenderDrawColor(p_renderer, rgb[i][0], rgb[i][1], rgb[i][2], SDL_ALPHA_OPAQUE);
        i = (i + 1) % s_rgb;
        subtrack.render(view);
    });
    
    SDL_SetRenderDrawColor(p_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    
    SDL_RenderDrawLine(p_renderer, splitting_line.m_start.x, splitting_line.m_start.y, splitting_line.m_end.x, splitting_line.m_end.y);
    
    SDL_RenderPresent(p_renderer);
    IMG_SavePNG(p_surface, "test_files/complex_track_split_test.png");
    
    SDL_DestroyRenderer(p_renderer);
    SDL_DestroyTexture(p_texture);
    SDL_FreeSurface(p_surface);
}
