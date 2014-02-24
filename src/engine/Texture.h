#ifndef TEXTURE_H
#define TEXTURE_H

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

#include <vector>
#include <memory>
//#include <boost/shared_ptr.hpp>
//#include <boost/weak_ptr.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Handler.h"
#include "Window.h"
#include "Resource.h"

namespace engine{


typedef std::shared_ptr<SDL_Surface> SDL_SurfacePtr;
typedef std::weak_ptr<SDL_Surface> SDL_SurfaceWPtr;
typedef std::shared_ptr<SDL_Texture> SDL_TexturePtr;
typedef std::weak_ptr<SDL_Texture> SDL_TextureWPtr;

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;
typedef Handler<Texture> TextureHnd;


struct TextureRenderState{
    SDL_Rect rect;
    double angle;
    SDL_Point center;
    SDL_RendererFlip flip;

    TextureRenderState(){
        rect.x = rect.y = rect.w = rect.h = 0;
        center.x = center.y = 0;
        angle = 0.0;
        flip = SDL_FLIP_NONE;
    }
    //TODO: Extend this with the operator=, operator==, and operator!= methods.
};


class Texture : public Resource
{
    public:
        Texture(WindowHnd win, int w, int h, Uint32 format=SDL_PIXELFORMAT_RGBA8888, int access=SDL_TEXTUREACCESS_STATIC);
        Texture(std::string uri, WindowHnd win);
        ~Texture();

        bool prepare();
        void release();
        bool isPrepared();

        WindowHnd getWindow();
        void setWindow(WindowHnd win);

        void queryInfo(Uint32 *fmt, int *access, int *width, int *height);

        void setAsRenderTarget();
        void clearRenderTarget();
        void render(int x, int y, SDL_Rect* clip=nullptr);
        void render(const SDL_Rect* src, const SDL_Rect* dst, const double& angle, const SDL_Point* center, const SDL_RendererFlip& flip);
        void render(const TextureRenderState *state, const SDL_Rect* dst);

    protected:
        // TOFO: Decide... Make this part of the Texture class or a child class.
        typedef std::vector<TextureRenderState> TexRenderStateList;
        typedef std::vector<TextureRenderState>::iterator TexRenderStateIter;
        TexRenderStateList mRenderStates;

        SDL_TexturePtr mTexture;
        WindowHnd mTexWindow;
        Uint32 mFormat;
        int mAccess;
        int mWidth;
        int mHeight;

        void LoadTexture();
        void CreateBlankTexture(Uint32 format, int access, int width, int height);
    private:
};


} // End namespace "engine"

#endif // TEXTURE_H


