#ifndef WINDOW_H
#define WINDOW_H


/*
* The MIT License (MIT)
*
* Copyright (c) 2014 Bryan Miller
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include <string>
#include <memory>

#include <SDL2/SDL.h>

#include "Handler.h"

namespace engine{


typedef Handler<SDL_Window> SDLWindowHnd;
typedef Handler<SDL_Renderer> SDLRendererHnd;


class Window
{
    public:
        Window(std::string title, int x, int y, int w, int h, Uint32 flags=0, Uint32 rflags=SDL_RENDERER_ACCELERATED);

        void drawTo(SDL_Texture *tex, SDL_Rect *dstRect, SDL_Rect *clip = NULL, float angle = 0.0, int xPivot = 0, int yPivot = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);

        void setLogicalRendererSize(int w, int h);
        void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);

        void getDrawColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a);

        void clear();

        void present();

        SDLWindowHnd getSDLWindow();
        SDLRendererHnd getSDLRenderer();

        SDL_Texture* textureFromSurface(SDL_Surface* surf);

        Uint32 getPixelFormat();

        int getDisplayIndex();
        void getDisplayMode(SDL_DisplayMode *mode);
        void setDisplayMode(SDL_DisplayMode *mode);

        void setFullscreen();

        static int GetNumVideoDisplays();
        static int GetNumDisplayModes(int displayIndex);
        static void GetDisplayMode(int displayIndex, int modeIndex, SDL_DisplayMode *mode);


    protected:
        typedef std::shared_ptr<SDL_Window> SDL_WindowPtr;
        typedef std::shared_ptr<SDL_Renderer> SDL_RendererPtr;

        SDL_WindowPtr       mWindow;
        SDL_RendererPtr     mRenderer;
        SDL_Rect            mScreenRect;
    private:
};
typedef std::shared_ptr<Window> WindowPtr;
typedef Handler<Window> WindowHnd;

} // End namespace "engine"


#endif // WINDOW_H
