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

    if (!win.IsValid()){
        throw std::runtime_error("Cannot Create Texture: Window pointer invalid.");
    }
    mTexWindow = win;

    LoadTexture();
}

//Texture::Texture(TextureWPtr tex, WindowWPtr win);

bool Texture::prepare(){
    if (mTexture.get() == 0){
        try{
            LoadTexture();
        } catch (std::runtime_error e){
            // Print error
            return false;
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

void Texture::getTextureBounds(int *width, int *height){
    if (mTexture.get() != 0){
        SDL_QueryTexture(mTexture.get(), NULL, NULL, width, height);
    }
}


void Texture::draw(int x, int y, SDL_Rect* clip){
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
            mTexWindow->drawTo(mTexture.get(), &pos, clip);
        }
    }
}


void Texture::LoadTexture(){
    if (mTexWindow.IsValid()){
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




