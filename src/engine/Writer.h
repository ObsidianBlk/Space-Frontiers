#ifndef WRITER_H
#define WRITER_H


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

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Handler.h"
#include "Window.h"


namespace engine{

    class Writer;
    typedef std::shared_ptr<Writer> WriterPtr;
    typedef Handler<Writer> WriterHnd;

    class Writer
    {
        public:
            ~Writer();

            /**
            * Adds a font to the Writer font map. After definition, the font is accessed by the given name.
            */
            void defineFont(std::string fontName, std::string fontSrc, int fontSize);
            bool hasFont(std::string fontName);
            int getFontPixelHeight(std::string fontName);
            void getFontStringTextSize(std::string fontName, std::string str, int *w, int *h);

            void setPenColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
            void setPenColor(const SDL_Color *c);
            void getPenColor(Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a);
            void getPenColor(SDL_Color* c);

            /**
            * Generates an SDL_Surface object with the given message rendered using the requested font, and returns a pointer to the SDL_Surface object.
            * NOTE: The Writer class does not take ownership of the generated SDL_Surface. The SDL_Surface will need to be freed manually.
            */
            SDL_Surface* textToSurface(std::string fontName, std::string message);

            /**
            * Generates an SDL_Texture object with the given message rendered using the requested font, and returns a pointer to the SDL_Texture object.
            * NOTE: The Writer class does not take ownership of the generated SDL_Texture. The SDL_Texture will need to be freed manually.
            */
            SDL_Texture* textToTexture(WindowHnd win, std::string fontName, std::string message);

            /**
            * Renders out the given message to the given window using the given font at the given x,y coordinates.
            * Once the text has been pushed to the window, all generated surfaces or textures are destroyed.
            */
            void presentToWindow(WindowHnd win, std::string fontName, std::string message, int x, int y);

            static WriterHnd getHandle();

        private:
            typedef std::pair<std::string, TTF_Font*> FontPair;
            typedef std::vector<FontPair> FontList;
            typedef std::vector<FontPair>::iterator FontListIter;
            FontList mFonts;

            SDL_Color mPen;

            static WriterPtr mInstance;
            Writer();

            TTF_Font* getFontPtr(std::string fontName);
    };

}
#endif // WRITER_H
