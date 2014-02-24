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

        try{
            LoadTexture();
        } catch (std::runtime_error e){
            throw e;
        }

        SDL_QueryTexture(mTexture.get(), &mFormat, &mAccess, &mWidth, &mHeight);
    }

    Texture::Texture(WindowHnd win, int w, int h, Uint32 format, int access) : Resource(){
        mTexture = SDL_TexturePtr();

        if (!win.IsValid()){
            throw std::runtime_error("Cannot Create Texture: Window pointer invalid.");
        }
        mTexWindow = win;
        try{
            CreateBlankTexture(format, access, w, h);
        } catch (std::runtime_error e){
            throw e;
        }
    }


    bool Texture::prepare(){
        if (mTexture.get() == 0){
            try{
                if (mURI != ""){
                    LoadTexture();
                } else {
                    CreateBlankTexture(mFormat, mAccess, mWidth, mHeight);
                }
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

    void Texture::queryInfo(Uint32 *fmt, int *access, int *width, int *height){
        if (mTexture.get() != 0){
            SDL_QueryTexture(mTexture.get(), fmt, access, width, height);
        }
    }


    void Texture::setAsRenderTarget(){
        if (prepare()){
            int access = 0;
            SDL_Texture* tex = mTexture.get();
            if (tex != 0){
                SDL_QueryTexture(tex, nullptr, &access, nullptr, nullptr);
                if (access & SDL_RENDERER_TARGETTEXTURE == 0){
                    throw std::runtime_error("Texture not configured to be a render target.");
                }

                SDL_Texture* curTargTex = mTexWindow->getRenderTarget();
                if (curTargTex == nullptr){
                    // Only set this texure as a render target if no other texture is in use.
                    mTexWindow->setRenderTarget(tex);
                } else if (curTargTex != tex){
                    throw std::runtime_error("Window already using a target texture.");
                }
            }
        } else {
            throw std::runtime_error("Failed to prepare Texture.");
        }
    }

    void Texture::clearRenderTarget(){
        if (mTexWindow.get() != 0 && mTexture.get() != 0){
            SDL_Texture* tex = mTexture.get();
            if (mTexWindow->getRenderTarget() == tex){
                // Only clear the target texture from the window renderer if it's THIS Texture that's currently in use.
                mTexWindow->setRenderTarget(nullptr);
            }
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


    void Texture::CreateBlankTexture(Uint32 format, int access, int width, int height){
        if (mTexWindow.get() != 0){
            SDL_Renderer* r = mTexWindow->getSDLRenderer().get();
            if (r != 0){
                SDL_Texture* tex = SDL_CreateTexture(r, format, access, width, height);
                if (tex != nullptr){
                    // I know we passed these in, but just in case SDL changed anything up on us...
                    SDL_QueryTexture(tex, &mFormat, &mAccess, &mWidth, &mHeight);
                    mTexture.reset(tex, SDL_DestroyTexture);
                } else {
                    throw std::runtime_error(SDL_GetError());
                }
            } else {
                throw std::runtime_error("Given Window object did not contain a valid renderer.");
            }
        } else {
            throw std::runtime_error("Texture Window object is invalid.");
        }
    }



} // End namespace "engine"





