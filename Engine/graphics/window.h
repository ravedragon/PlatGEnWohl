#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h> // SDL 2 Library
#include <SDL2/SDL_opengl.h>

#include <QString>

class PGE_Window
{
public:
    static int Width;
    static int Height;
    static bool init(QString WindowTitle="Platformer Game Engine by Wohldtand");
    static bool uninit();
    static bool isReady();

    static SDL_Window *window;
    static int SDL_ToggleFS(SDL_Window *win=NULL);

private:
    static bool IsInit;
    static SDL_bool IsFullScreen(SDL_Window *win);
};

#endif // WINDOW_H
