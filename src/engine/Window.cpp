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

//SDL_WindowPtr Window::mWindow = SDL_WindowPtr(nullptr, SDL_DestroyWindow);
//SDL_RendererPtr Window::mRenderer = SDL_RendererPtr(nullptr, SDL_DestroyRenderer);
//SDL_Rect Window::mScreenRect;


namespace engine{



Window::Window(std::string title, int x, int y, int w, int h, Uint32 wflags, Uint32 rflags){
    //mWindow = SDL_WindowPtr(nullptr, SDL_DestroyWindow);
    //mRenderer = SDL_RendererPtr(nullptr, SDL_DestroyRenderer);
    // Create the window.
    mWindow = SDL_WindowPtr(SDL_CreateWindow(title.c_str(), x, y, w, h, wflags), SDL_DestroyWindow);
    if (mWindow == nullptr)
        throw std::runtime_error("SDL failed to create window.");

    //Create the renderer
    mRenderer.reset(SDL_CreateRenderer(mWindow.get(), -1, rflags), SDL_DestroyRenderer);
    if (mRenderer == nullptr)
        throw std::runtime_error("SDL failed to create window renderer.");
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

void Window::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
    SDL_SetRenderDrawColor(mRenderer.get(), r, g, b, a);
}

void Window::getDrawColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a){
    SDL_GetRenderDrawColor(mRenderer.get(), r, g, b, a);
}

void Window::clear(){
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


