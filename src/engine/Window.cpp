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

void Window::drawTo(SDL_Texture *tex, SDL_Rect *dstRect, SDL_Rect *clip, float angle, int xPivot, int yPivot, SDL_RendererFlip flip){
    xPivot += dstRect->w/2;
    yPivot += dstRect->h/2;
    SDL_Point pivot = {xPivot, yPivot};

    //Draw the texture
    SDL_RenderCopyEx(mRenderer.get(), tex, clip, dstRect, angle, &pivot, flip);
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

SDL_WindowWPtr Window::getSDLWindow(){
    if (mWindow.get() != 0){
        return SDL_WindowWPtr(mWindow);
    }
    return SDL_WindowWPtr();
}

SDL_RendererWPtr Window::getSDLRenderer(){
    if (mRenderer.get() != 0){
        return SDL_RendererWPtr(mRenderer);
    }
    return SDL_RendererWPtr();
}







