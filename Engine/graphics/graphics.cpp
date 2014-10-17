#include <SDL2/SDL.h> // SDL 2 Library
#include <SDL2/SDL_opengl.h>
#include <QtDebug>
#include "graphics.h"
#include "window.h"

QPointF mapToOpengl(QPoint s)
{
    qreal nx  =  s.x() - qreal(PGE_Window::Width)  /  2;
    qreal ny  =  s.y() - qreal(PGE_Window::Height)  /  2;
    //qreal( qreal(PGE_Window::Height) - qreal(s.y())  -  1)  /  qreal(PGE_Window::Height  /  2  -  1;
    return QPointF(nx, ny);
}


//SDL_Surface *load_image( std::string filename )
//{
//    //Temporary storage for the image that's loaded
//    SDL_Surface* loadedImage = NULL;

//    //The optimized image that will be used
//    SDL_Surface* optimizedImage = NULL;

//    //Load the image
//    loadedImage = SDL_LoadBMP( filename.c_str() );

//    //If nothing went wrong in loading the image
//    if( loadedImage != NULL )
//    {
//        //Create an optimized image
//        optimizedImage = SDL_ConvertSurfaceFormat( loadedImage, SDL_PIXELFORMAT_ARGB8888, 0 );

//        //Free the old image
//        SDL_FreeSurface( loadedImage );
//    }

//    //Return the optimized image
//    return optimizedImage;
//}

