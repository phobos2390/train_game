/// @file train_game/OpenGl_view.cpp

#include <train_game/OpenGl_view.h>
#include <sdl2/SDL.h>
#include <sdl2/SDL_opengl.h>
#include <sdl2/SDL_ttf.h>
#include <algorithm>
#include <map>

namespace train_game
{
struct color_t
{
    uint8_t m_red;
    uint8_t m_green;
    uint8_t m_blue;
    
    color_t(uint8_t red, uint8_t green, uint8_t blue)
        :m_red(red), m_green(green), m_blue(blue) {}

    color_t(const color_t& color)
        :m_red(color.m_red), m_green(color.m_green), m_blue(color.m_blue) {}
    
    color_t()
        :m_red(255),m_green(255),m_blue(255){}
};

struct OpenGl_view::Impl
{
public:
    SDL_Window* m_p_window;
    float m_window_scale;
    float m_scale;
    float m_line_thickness;
    glm::vec2 m_center;
    int32_t m_error;
    SDL_GLContext m_p_gl_context;
    uint32_t m_height;
    uint32_t m_width;
    std::map<std::string, color_t> m_colormap;
    color_t m_current_color;
    
    Impl(std::string window_name)
        : m_p_window(NULL)
        , m_window_scale(2.0)
        , m_scale(1.0)
        , m_line_thickness(2)
        , m_center(250,250)
        , m_error(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
        , m_p_gl_context(0)
        , m_height(500)
        , m_width(500)
        , m_colormap()
        , m_current_color(255,255,255)
    {
        if(m_error == 0)
        {
            m_p_window = SDL_CreateWindow( window_name.c_str()
                                         , SDL_WINDOWPOS_UNDEFINED
                                         , SDL_WINDOWPOS_UNDEFINED
                                         , m_height * m_window_scale
                                         , m_width * m_window_scale
                                         , SDL_WINDOW_OPENGL);

            m_p_gl_context = SDL_GL_CreateContext(m_p_window);
            TTF_Init();
        }
        ensure_color("default", m_current_color);
    }
    virtual ~Impl()
    {
        if(m_error == 0)
        {
            TTF_Quit();
            SDL_GL_DeleteContext(m_p_gl_context);
            SDL_DestroyWindow(m_p_window);
            SDL_Quit();
        }
    }
    
    void set_transform()
    {
        float width = m_width * m_scale;
        float height = m_height * m_scale;
        glLoadIdentity();
        //glTranslatef(-width/2,height/2,0);
        glScalef(1.0f/width,1.0f/-height,1.0);
        glTranslatef(-m_center.x, -m_center.y,0);
        //glTranslatef(-width/2.0,-height/2.0,0);
    }

    void ensure_color(const std::string& alias, const color_t& color)
    {
        if(m_colormap.find(alias) == m_colormap.end())
        {
            m_colormap.emplace(alias, color);
        }
        else
        {
            m_colormap[alias].m_red = color.m_red;
            m_colormap[alias].m_green = color.m_green;
            m_colormap[alias].m_blue = color.m_blue;
        }
    }
    
    color_t get_color(std::string& alias)
    {
        color_t color(m_colormap["default"]);
        if(m_colormap.find(alias) != m_colormap.end())
        {
            color = m_colormap[alias];
        }
        return color;
    }
    
    int32_t init_font(std::string ttf_filename)
    {
//        TTF_Font *font = TTF_OpenFont(ttf_filename.c_str(), 40);
//        if(!font) {
//            printf("Unable to load font: '%s'!\n"
//                   "SDL2_ttf Error: %s\n", ttf_filename.c_str(), TTF_GetError());
//            return -1;
//        }
//
//        SDL_Color textColor           = { 0x00, 0x00, 0x00, 0xFF };
//        SDL_Color textBackgroundColor = { 0xFF, 0xFF, 0xFF, 0xFF };
//        SDL_Texture *text = NULL;
//        SDL_Rect textRect;
//
//        SDL_Surface *textSurface = TTF_RenderText_Shaded(font, "", textColor, textBackgroundColor);
//        if(!textSurface) 
//        {
//            printf("Unable to render text surface!\n"
//                   "SDL2_ttf Error: %s\n", TTF_GetError());
//        }
//        else
//        {
//            // Create texture from surface pixels
//            text = SDL_CreateTextureFromSurface(renderer, textSurface);
//            if(!text) 
//            {
//                printf("Unable to create texture from rendered text!\n"
//                       "SDL2 Error: %s\n", SDL_GetError());
//                return -2;
//            }
//
//            // Get text dimensions
//            textRect.w = textSurface->w;
//            textRect.h = textSurface->h;
//
//            SDL_FreeSurface(textSurface);
//        }
//
//        textRect.x = (SCREEN_WIDTH - textRect.w) / 2;
//        textRect.y = squareRect.y - textRect.h - 10;
//        return 0;
        return -3;
    }
};

OpenGl_view::OpenGl_view(std::string window_name)
    :m_p_impl(new Impl(window_name))
{
}

OpenGl_view::~OpenGl_view()
{
    delete m_p_impl;
    m_p_impl = 0;
}

void OpenGl_view::per_frame_init()
{
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
            GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_p_impl->set_transform();
}

void OpenGl_view::per_frame_finish()
{
    SDL_GL_SwapWindow(m_p_impl->m_p_window);
}

void OpenGl_view::render_line(glm::vec2 start, glm::vec2 end)
{
    glLineWidth(m_p_impl->m_line_thickness / m_p_impl->m_scale);
    glBegin(GL_LINES);
    color_t c = m_p_impl->m_current_color;
    glColor3ub(c.m_red, c.m_green, c.m_blue);
    glVertex2f(start.x,start.y);
    glVertex2f(end.x,end.y);
    glEnd();
}

void OpenGl_view::render_polygon(std::vector<glm::vec2>& points)
{
    glBegin(GL_POLYGON);
    color_t c = m_p_impl->m_current_color;
    glColor3ub(c.m_red, c.m_green, c.m_blue);
    std::for_each(points.begin()
                 ,points.end()
                 ,[](const glm::vec2& current)
    {
        glVertex2f(current.x, current.y);
    });
    glEnd();
}

void OpenGl_view::render_polygon(glm::vec2 center, std::vector<glm::vec2>& points)
{
    std::vector<glm::vec2> polygon_points;
    std::transform(points.begin()
                  ,points.end()
                  ,std::back_inserter(polygon_points)
                  ,[center](const glm::vec2& point)
    {
        return point + center;
    });
    render_polygon(polygon_points);
}

void OpenGl_view::set_color(std::string alias)
{
    m_p_impl->m_current_color = m_p_impl->get_color(alias);
}

void OpenGl_view::set_color(std::string alias, uint8_t red, uint8_t green, uint8_t blue)
{
    color_t c(red,green,blue);
    m_p_impl->ensure_color(alias, c);
    m_p_impl->m_current_color.m_red = red;
    m_p_impl->m_current_color.m_green = green;
    m_p_impl->m_current_color.m_blue = blue;
}

void OpenGl_view::set_scale(float scale)
{
    m_p_impl->m_scale = scale;
}

void OpenGl_view::move_center(float dx, float dy)
{
    m_p_impl->m_center.x -= dx*m_p_impl->m_scale;
    m_p_impl->m_center.y -= dy*m_p_impl->m_scale;
}

glm::vec2 OpenGl_view::to_world_coord(glm::vec2 window_coord)
{
    glm::vec2 c_w(m_p_impl->m_window_scale * m_p_impl->m_height, m_p_impl->m_window_scale * m_p_impl->m_width);
    c_w = c_w / 2.0f;
    return ((window_coord - c_w) * m_p_impl->m_scale) + m_p_impl->m_center;
}


int32_t OpenGl_view::get_error()
{
    return m_p_impl->m_error;
}
}
