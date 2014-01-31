
#include "SDLWrap.h"

bool SDLWrap::Init(uint32_t flags)
{
    SDL_SetMainReady();
    return SDL_Init(flags);
}

void SDLWrap::Shutdown()
{
    for (auto context : m_glcontexts)
    {
        SDL_GL_DeleteContext(context);
    }
    for (auto *window : m_windows)
    {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

SDL_Window *SDLWrap::CreateWindow(
    const char *title,
    int x, int y, int w,
    int h, Uint32 flags)
{
    SDL_Window *win = SDL_CreateWindow(title, x, y, w, h, flags);
    m_windows.emplace_back(win);
    return win;
}

SDL_GLContext SDLWrap::CreateGLContext(SDL_Window *window)
{
    SDL_GLContext c = SDL_GL_CreateContext(window);
    if (m_glcontexts.empty())
    {
        glewExperimental = true;
        glewInit();
    }
    m_glcontexts.emplace_back(c);
    return c;
}
