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

#include "Texture.h"


namespace engine{


Texture::~Texture(){}

Texture::Texture(std::string uri, WindowHnd win) : Resource(uri){
    mTexture = SDL_TexturePtr();
    mSurface = SDL_SurfacePtr();

    if (!win.IsValid()){
        throw std::runtime_error("Cannot Create Texture: Window pointer invalid.");
    }
    mTexWindow = win;

    LoadTexture();
}

Texture::Texture(WindowHnd win, int w, int h, int depth, Uint32 flags, Uint32 rmask, Uint32 gmask, Uint32 bmask, Uint32 amask) : Resource(){
    mTexture = SDL_TexturePtr();
    mSurface = SDL_SurfacePtr();

    if (!win.IsValid()){
        throw std::runtime_error("Cannot Create Texture: Window pointer invalid.");
    }
    mTexWindow = win;

    SDL_Surface *usurf = SDL_CreateRGBSurface(flags, w, h, depth, rmask, gmask, bmask, amask);
    if (usurf == nullptr){
        throw std::runtime_error("Failed to create texture pixel surface.");
    }
    SDL_PixelFormat *fmt = SDL_AllocFormat(mTexWindow->getPixelFormat());
    if (fmt == 0){
        SDL_FreeSurface(usurf);
        throw std::runtime_error("Failed to obtain window pixel format.");
    }

    SDL_Surface *osurf = SDL_ConvertSurface(usurf, fmt, 0);
    SDL_FreeSurface(usurf);
    SDL_FreeFormat(fmt);

    if (osurf == 0){
        throw std::runtime_error("Failed to create optimized pixel surface.");
    }

    mSurface = SDL_SurfacePtr(osurf);
}


bool Texture::prepare(){
    if (mTexture.get() == 0){
        if (mSurface.get() != 0){
            if (!mTexWindow.IsValid()){
                throw std::runtime_error("Cannot convert pixel surface to screen texture. Defined Window is invalid.");
            }
            SDLRendererHnd renderer = mTexWindow->getSDLRenderer();
            // TODO: I should test the renderer, but I'mma be lazy right now. Fix me please!
            SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer.get(), mSurface.get());
            if (tex == nullptr){
                throw std::runtime_error("Failed to convert pixel surface to screen texture.");
            }
            mTexture = SDL_TexturePtr(tex, SDL_DestroyTexture);
        } else if (mURI != ""){
            try{
                LoadTexture();
            } catch (std::runtime_error e){
                // Print error
                return false;
            }
        }
    } else if (!mTexWindow.IsValid()){
        return false;
    }
    return true;
}

void Texture::release(){
    if (mTexture.get() != 0){
        mTexture.reset();
    }
}

bool Texture::isPrepared(){
    return mTexture.get() != 0 && mTexWindow.IsValid();
}


WindowHnd Texture::getWindow(){
    return mTexWindow;
}

void Texture::setWindow(WindowHnd win){
    if (win.IsValid() && mTexWindow.IsValid() && win != mTexWindow){
        release();
        mTexWindow = win;
    }
}

void Texture::queryInfo(Uint32 *fmt, int *access, int *width, int *height){
    if (mTexture.get() != 0){
        SDL_QueryTexture(mTexture.get(), fmt, access, width, height);
    }
}


void Texture::render(int x, int y, SDL_Rect* clip){
    if (prepare()){
        if (mTexWindow.IsValid()){
            SDL_Rect pos;
            pos.x = x;
            pos.y = y;
            if (clip != nullptr){
                pos.w = clip->w;
                pos.h = clip->h;
            } else {
                SDL_QueryTexture(mTexture.get(), NULL, NULL, &pos.w, &pos.h);
            }
            mTexWindow->render(mTexture.get(), &pos, clip);
        }
    }
}

void Texture::render(const SDL_Rect* src, const SDL_Rect* dst, const double& angle, const SDL_Point* center, const SDL_RendererFlip& flip){
    if (prepare() && mTexWindow.IsValid()){
        mTexWindow->render(mTexture.get(), src, dst, angle, center, flip);
    }
}

void Texture::render(const TextureRenderState *state, const SDL_Rect* dst){
    this->render(&state->rect, dst, state->angle, &state->center, state->flip);
}


void Texture::LoadTexture(){
    if (mTexWindow.IsValid() && mURI != ""){
        if (mTexture.get() == 0){
            SDL_Texture* tex = nullptr;
            SDLRendererHnd renderer = mTexWindow->getSDLRenderer();
            if (!renderer.IsValid()){
                throw std::runtime_error("Failed to Load Texture: Could not obtain window renderer instance.");
            }

            tex = IMG_LoadTexture(renderer.get(), mURI.c_str());
            if (tex == nullptr){
                throw std::runtime_error("Failed to Load Texture:"); //\"" + IMG_GetError() + "\"");
            }
            mTexture.reset(tex, SDL_DestroyTexture);
        }
    }
}



} // End namespace "engine"





