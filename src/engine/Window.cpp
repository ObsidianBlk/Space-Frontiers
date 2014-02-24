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

#include "Window.h"


namespace engine{



    Window::Window(std::string title, int x, int y, int w, int h, Uint32 wflags, Uint32 rflags){
        // Create the window.
        mWindow = SDL_WindowPtr(SDL_CreateWindow(title.c_str(), x, y, w, h, wflags), SDL_DestroyWindow);
        if (mWindow == nullptr)
            throw std::runtime_error("SDL failed to create window.");

        //Create the renderer
        mRenderer.reset(SDL_CreateRenderer(mWindow.get(), -1, rflags), SDL_DestroyRenderer);
        if (mRenderer == nullptr)
            throw std::runtime_error("SDL failed to create window renderer.");

        setClearColor(0, 0, 0, 255);
        setPenColor(0, 0, 0, 255);
        setBucketColor(0, 0, 0, 255);
        mPenThickness = 1;
    }

    void Window::render(SDL_Texture *tex, const SDL_Rect *src, const SDL_Rect *dst){
        SDL_Renderer *r = mRenderer.get();
        if (r != 0){
            SDL_RenderCopy(r, tex, src, dst);
        }
    }

    void Window::render(SDL_Texture *tex, const SDL_Rect* src, const SDL_Rect* dst, const double& angle, const SDL_Point* center, const SDL_RendererFlip& flip){
        SDL_Renderer *r = mRenderer.get();
        if (r != 0){
            SDL_RenderCopyEx(r, tex, src, dst, angle, center, flip);
        }
    }

    void Window::setLogicalRendererSize(int w, int h){
        SDL_RenderSetLogicalSize(mRenderer.get(), w, h);
    }

    void Window::setPenColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
        mPenColor.r = r;
        mPenColor.g = g;
        mPenColor.b = b;
        mPenColor.a = a;
    }

    void Window::setBucketColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
        mBucketColor.r = r;
        mBucketColor.g = g;
        mBucketColor.b = b;
        mBucketColor.a = a;
    }

    void Window::setClearColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
        mClearColor.r = r;
        mClearColor.g = g;
        mClearColor.b = b;
        mClearColor.a = a;
    }

    void Window::setPenColor(const SDL_Color *color){
        mPenColor.r = color->r;
        mPenColor.g = color->g;
        mPenColor.b = color->b;
        mPenColor.a = color->a;
    }

    void Window::setBucketColor(const SDL_Color *color){
        mBucketColor.r = color->r;
        mBucketColor.g = color->g;
        mBucketColor.b = color->b;
        mBucketColor.a = color->a;
    }

    void Window::setClearColor(const SDL_Color *color){
        mClearColor.r = color->r;
        mClearColor.g = color->g;
        mClearColor.b = color->b;
        mClearColor.a = color->a;
    }


    void Window::getPenColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a){
        if (r != nullptr){*r = mPenColor.r;}
        if (g != nullptr){*g = mPenColor.g;}
        if (b != nullptr){*b = mPenColor.b;}
        if (a != nullptr){*a = mPenColor.a;}
    }

    void Window::getBucketColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a){
        if (r != nullptr){*r = mBucketColor.r;}
        if (g != nullptr){*g = mBucketColor.g;}
        if (b != nullptr){*b = mBucketColor.b;}
        if (a != nullptr){*a = mBucketColor.a;}
    }

    void Window::getClearColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a){
        if (r != nullptr){*r = mClearColor.r;}
        if (g != nullptr){*g = mClearColor.g;}
        if (b != nullptr){*b = mClearColor.b;}
        if (a != nullptr){*a = mClearColor.a;}
    }

    void Window::getPenColor(SDL_Color *color){
        color->r = mPenColor.r;
        color->g = mPenColor.g;
        color->b = mPenColor.b;
        color->a = mPenColor.a;
    }

    void Window::getBucketColor(SDL_Color *color){
        color->r = mBucketColor.r;
        color->g = mBucketColor.g;
        color->b = mBucketColor.b;
        color->a = mBucketColor.a;
    }

    void Window::getClearColor(SDL_Color *color){
        color->r = mClearColor.r;
        color->g = mClearColor.g;
        color->b = mClearColor.b;
        color->a = mClearColor.a;
    }


    void Window::setPenThickness(Uint16 thickness){
        mPenThickness = thickness;
    }

    int Window::getPenThickness(){
        return mPenThickness;
    }

    void Window::setRenderColor(SDL_Color *c){
        SDL_Renderer* r = mRenderer.get();
        if (r != 0){
            SDL_SetRenderDrawColor(r, c->r, c->g, c->b, c->a);
        }
    }

    void Window::drawLine(int x1, int y1, int x2, int y2){
        SDL_Renderer* r = mRenderer.get();
        if (r != 0){
            setRenderColor(&mPenColor);
            SDL_RenderDrawLine(r, x1, y1, x2, y2);
        }
    }

    void Window::drawLines(const SDL_Point* points, int count){
        SDL_Renderer* r = mRenderer.get();
        if (r != 0){
            setRenderColor(&mPenColor);
            SDL_RenderDrawLines(r, points, count);
        }
    }

    void Window::drawRect(int x, int y, int w, int h, bool hollow){
        SDL_Renderer* r = mRenderer.get();
        if (r != 0){
            SDL_Rect rect;
            rect.x = x;
            rect.y = y;
            rect.w = w;
            rect.h = h;
            if (!hollow){
                SDL_RenderFillRect(r, &rect);
            }
            if (mPenThickness > 0){
                SDL_RenderDrawRect(r, &rect);
            }
        }
    }

    void Window::drawRect(const SDL_Rect* rect, bool hollow){
        SDL_Renderer* r = mRenderer.get();
        if (r != 0){
            if (!hollow){
                SDL_RenderFillRect(r, rect);
            }
            if (mPenThickness > 0){
                SDL_RenderDrawRect(r, rect);
            }
        }
    }

    void Window::drawRects(const SDL_Rect* rects, int count, bool hollow){
        SDL_Renderer* r = mRenderer.get();
        if (r != 0){
            if (!hollow){
                SDL_RenderFillRects(r, rects, count);
            }
            if (mPenThickness > 0){
                SDL_RenderDrawRects(r, rects, count);
            }
        }
    }

    void Window::drawPoint(int x, int y){
        SDL_Renderer* r = mRenderer.get();
        if (r != 0){
            setRenderColor(&mPenColor);
            SDL_RenderDrawPoint(r, x, y);
        }
    }

    void Window::drawPoint(const SDL_Point* point){
        SDL_Renderer* r = mRenderer.get();
        if (r != 0){
            setRenderColor(&mPenColor);
            SDL_RenderDrawPoint(r, point->x, point->y);
        }
    }

    void Window::drawPoints(const SDL_Point* points, int count){
        SDL_Renderer* r = mRenderer.get();
        if (r != 0){
            setRenderColor(&mPenColor);
            SDL_RenderDrawPoints(r, points, count);
        }
    }

    void Window::setRenderTarget(SDL_Texture *target){
        SDL_Renderer* r = mRenderer.get();
        if (r != 0){
            SDL_RendererInfo rinfo;
            SDL_GetRendererInfo(r, &rinfo);
            if (rinfo.flags & SDL_RENDERER_TARGETTEXTURE != 0){
                SDL_SetRenderTarget(r, target);
            } else {
                throw std::runtime_error("Render to Texture not supported.");
            }
        }
    }

    SDL_Texture* Window::getRenderTarget(){
        SDL_Renderer* r = mRenderer.get();
        if (r != 0){
            return SDL_GetRenderTarget(r);
        }
        return nullptr;
    }

    void Window::clear(){
        setRenderColor(&mClearColor);
        SDL_RenderClear(mRenderer.get());
    }

    void Window::present(){
        SDL_RenderPresent(mRenderer.get());
    }

    SDLWindowHnd Window::getSDLWindow(){
        if (mWindow.get() != 0){
            return SDLWindowHnd(mWindow);
        }
        return SDLWindowHnd();
    }

    SDLRendererHnd Window::getSDLRenderer(){
        if (mRenderer.get() != 0){
            return SDLRendererHnd(mRenderer);
        }
        return SDLRendererHnd();
    }

    SDL_Texture* Window::textureFromSurface(SDL_Surface* surf){
        if (mRenderer.get() != 0){
            return SDL_CreateTextureFromSurface(mRenderer.get(), surf);
        }
        return nullptr;
    }


    Uint32 Window::getPixelFormat(){
        SDL_Window *w = mWindow.get();
        if (w != 0){
            return SDL_GetWindowPixelFormat(w);
        }
        return 0;
    }

    void Window::getLogicalRendererSize(int *w, int *h){
        SDL_Renderer *r = mRenderer.get();
        if (r != 0){
            SDL_RenderGetLogicalSize(r, w, h);
        }
    }

    void Window::getRenderInfo(SDL_RendererInfo &info){
        SDL_Renderer *r = mRenderer.get();
        if (r != 0){
            SDL_GetRendererInfo(r, &info);
        }
    }

    int Window::getDisplayIndex(){
        if (mWindow.get() != 0){
            return SDL_GetWindowDisplayIndex(mWindow.get());
        }
        return 0;
    }

    void Window::getDisplayMode(SDL_DisplayMode *mode){
        if (mWindow.get() != 0){
            SDL_GetWindowDisplayMode(mWindow.get(), mode);
        } else {
            mode = nullptr;
        }
    }

    void Window::setDisplayMode(SDL_DisplayMode *mode){
        if (mWindow.get() != 0){
            SDL_SetWindowDisplayMode(mWindow.get(), mode);
        }
    }


    void Window::setFullscreen(){
        if (mWindow.get() != 0){
            // Truthfully... the way I understand SDL2's management of "fake" fullscreen, I prefer it
            // over "true" full screen. Seems more stable... but then, that's just me.
            // For now, this will only do "fake" full screen, but may give the choice later.
            SDL_SetWindowFullscreen(mWindow.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);

            // TODO: Ok... this is actually sort of lame. This should be a form of toggle, and, as such,
            // I should be storing the current "windowed" display mode in which to switch BACK to if the
            // user disabled fullscreen mode. Buuuut... I'd rather write this comment that do that right
            // this instant, soooo... some other time.
        }
    }


    // ---------------------


    int Window::GetNumVideoDisplays(){
        return SDL_GetNumVideoDisplays();
    }

    int Window::GetNumDisplayModes(int displayIndex){
        return SDL_GetNumDisplayModes(displayIndex);
    }

    void Window::GetDisplayMode(int displayIndex, int modeIndex, SDL_DisplayMode *mode){
        SDL_GetDisplayMode(displayIndex, modeIndex, mode);
    }


} // End namespace "engine"


