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

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Window.h"
#include "Resource.h"


typedef boost::shared_ptr<SDL_Surface> SDL_SurfacePtr;
typedef boost::weak_ptr<SDL_Surface> SDL_SurfaceWPtr;
typedef boost::shared_ptr<SDL_Texture> SDL_TexturePtr;
typedef boost::weak_ptr<SDL_Texture> SDL_TextureWPtr;

class Texture;
typedef boost::shared_ptr<Texture> TexturePtr;
typedef boost::weak_ptr<Texture> TextureWPtr;


class Texture : public Resource
{
    public:
        Texture(std::string uri, WindowWPtr win);
        //Texture(TextureWPtr tex, WindowWPtr win);
        ~Texture();

        bool prepare();
        void release();
        bool isPrepared();

        WindowWPtr getWindow();
        void setWindow(WindowWPtr win);

        void getTextureBounds(int *width, int *height);

        void draw(int x, int y, SDL_Rect* clip=nullptr);

    protected:
        SDL_TexturePtr mTexture;
        WindowWPtr mTexWindow;

        void LoadTexture();
    private:
};



#endif // TEXTURE_H


