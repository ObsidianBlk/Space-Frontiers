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


#include "Writer.h"

namespace engine{

    WriterPtr Writer::mInstance = WriterPtr();

    WriterHnd Writer::getHandle(){
        if (mInstance.get() == 0){
            try{
                mInstance = WriterPtr(new Writer());
            } catch (std::runtime_error e){
                throw e;
            }
        }
        return WriterHnd(mInstance);
    }

    Writer::Writer()
    {
        if (TTF_Init() != 0){
            throw std::runtime_error("Failed to initilize Truetype Font writer.");
        }
    }

    Writer::~Writer()
    {
        while (!mFonts.empty()){
            TTF_CloseFont(mFonts.begin()->second);
            mFonts.erase(mFonts.begin());
        }
    }

    void Writer::defineFont(std::string fontName, std::string fontSrc, int fontSize){
        if (getFontPtr(fontName) == nullptr){
            TTF_Font *font = TTF_OpenFont(fontSrc.c_str(), fontSize);
            if (font == nullptr){
                throw std::runtime_error(std::string("Failed to open font \"") + fontSrc + std::string("\"."));
            }
            mFonts.push_back(FontPair(fontName, font));
        }
    }

    int Writer::getFontPixelHeight(std::string fontName){
        TTF_Font* font = getFontPtr(fontName);
        if (font != nullptr){
            return TTF_FontHeight(font);
        }
    }

    void Writer::setPenColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
        mPen.r = r;
        mPen.g = g;
        mPen.b = b;
        mPen.a = a;
    }

    void Writer::setPenColor(const SDL_Color *c){
        mPen.r = c->r;
        mPen.g = c->g;
        mPen.b = c->b;
        mPen.a = c->a;
    }

    void Writer::getPenColor(Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a){
        if (r != nullptr){*r = mPen.r;}
        if (g != nullptr){*g = mPen.g;}
        if (b != nullptr){*b = mPen.b;}
        if (a != nullptr){*a = mPen.a;}
    }

    void Writer::getPenColor(SDL_Color* c){
        c->r = mPen.r;
        c->g = mPen.g;
        c->b = mPen.b;
        c->a = mPen.a;
    }

    SDL_Surface* Writer::textToSurface(std::string fontName, std::string message){
        TTF_Font* font = getFontPtr(fontName);
        if (font != nullptr){
            return TTF_RenderText_Blended(font, message.c_str(), mPen);
        }
        return nullptr;
    }

    SDL_Texture* Writer::textToTexture(WindowHnd win, std::string fontName, std::string message){
        if (win.IsValid()){
            SDL_Surface* surf = textToSurface(fontName, message);
            if (surf != nullptr){
                SDL_Texture* t = win->textureFromSurface(surf);
                SDL_FreeSurface(surf);
                return t;
            }
        }
        return nullptr;
    }

    void Writer::presentToWindow(WindowHnd win, std::string fontName, std::string message, int x, int y){
        SDL_Texture* t = textToTexture(win, fontName, message);
        // Since textToTexture checks the validity of the given WindowHnd object itself, we don't need to
        // check it's validity here... we only need to check that we have a texture.
        if (t != nullptr){
            SDL_Rect src;
            SDL_Rect pos;
            src.x = 0;
            src.y = 0;
            SDL_QueryTexture(t, NULL, NULL, &src.w, &src.h);
            pos.x = x;
            pos.y = y;
            pos.w = src.w;
            pos.h = src.h;
            win->render(t, nullptr, &pos);
            SDL_DestroyTexture(t);
        }
    }


    TTF_Font* Writer::getFontPtr(std::string fontName){
        if (!mFonts.empty()){
            for (FontListIter item = mFonts.begin(); item != mFonts.end(); item++){
                if (item->first == fontName){
                    return item->second;
                }
            }
        }
        return nullptr;
    }


}
