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


class Texture : public Resource
{
    public:
        Texture(WindowHnd win, int w, int h, int depth, Uint32 flags=0, Uint32 rmask=0, Uint32 gmask=0, Uint32 bmask=0, Uint32 amask=0);
        Texture(std::string uri, WindowHnd win);
        ~Texture();

        bool prepare();
        void release();
        bool isPrepared();

        WindowHnd getWindow();
        void setWindow(WindowHnd win);

        void getTextureBounds(int *width, int *height);

        void draw(int x, int y, SDL_Rect* clip=nullptr);

    protected:
        SDL_TexturePtr mTexture;
        SDL_SurfacePtr mSurface;
        WindowHnd mTexWindow;

        void LoadTexture();
    private:
};


} // End namespace "engine"

#endif // TEXTURE_H


