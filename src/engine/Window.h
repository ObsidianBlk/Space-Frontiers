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

        /* -- Basic Drawing Operations -- */
        void setPenColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
        void setBucketColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
        void setClearColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);

        void setPenColor(const SDL_Color *color);
        void setBucketColor(const SDL_Color *color);
        void setClearColor(const SDL_Color *color);

        void getPenColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a);
        void getBucketColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a);
        void getClearColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a);

        void getPenColor(SDL_Color *color);
        void getBucketColor(SDL_Color *color);
        void getClearColor(SDL_Color *color);

        void setPenThickness(Uint16 thickness);
        int getPenThickness();

        void drawLine(int x1, int y1, int x2, int y2);
        void drawLines(const SDL_Point* points, int count);
        void drawRect(int x, int y, int w, int h, bool hollow=false);
        void drawRect(const SDL_Rect* rect, bool hollow=false);
        void drawRects(const SDL_Rect* rects, int count, bool hollow=false);
        void drawPoint(int x, int y);
        void drawPoint(const SDL_Point* point);
        void drawPoints(const SDL_Point* points, int count);

        /* -- Core Operations -- */
        void clear();
        void present();
        void render(SDL_Texture *tex, const SDL_Rect *src, const SDL_Rect *dst);
        void render(SDL_Texture *tex, const SDL_Rect* src, const SDL_Rect* dst, const double& angle, const SDL_Point* center, const SDL_RendererFlip& flip);
        void setRenderTarget(SDL_Texture *target);
        SDL_Texture* getRenderTarget();

        /* -- States -- */
        void setLogicalRendererSize(int w, int h);
        void setFullscreen();

        /* -- Utilities -- */
        SDL_Texture* textureFromSurface(SDL_Surface* surf);

        /* -- Information -- */
        SDLWindowHnd getSDLWindow();
        SDLRendererHnd getSDLRenderer();

        Uint32 getPixelFormat();
        void getLogicalRendererSize(int *w, int *h);
        void getRenderInfo(SDL_RendererInfo &info);

        int getDisplayIndex();
        void getDisplayMode(SDL_DisplayMode *mode);
        void setDisplayMode(SDL_DisplayMode *mode);

        /* -- Video Display Information -- */
        static int GetNumVideoDisplays();
        static int GetNumDisplayModes(int displayIndex);
        static void GetDisplayMode(int displayIndex, int modeIndex, SDL_DisplayMode *mode);


    protected:
        typedef std::shared_ptr<SDL_Window> SDL_WindowPtr;
        typedef std::shared_ptr<SDL_Renderer> SDL_RendererPtr;

        SDL_WindowPtr       mWindow;
        SDL_RendererPtr     mRenderer;
        SDL_Rect            mScreenRect;

        // Below are variables used in the drawing operations.
        int mPenThickness;
        SDL_Color mClearColor;
        SDL_Color mBucketColor;
        SDL_Color mPenColor;

        /** Simple color setting helper method.*/
        void setRenderColor(SDL_Color *c);
    private:
};
typedef std::shared_ptr<Window> WindowPtr;
typedef Handler<Window> WindowHnd;

} // End namespace "engine"


#endif // WINDOW_H
