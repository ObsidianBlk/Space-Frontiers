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

#include "TextureManager.h"


namespace engine{



TextureManager* TextureManager::mInstance = nullptr;

TextureManager::TextureManager() : ResourceManager<TexturePtr, TextureHnd>(){
    //ResourceManager<TexturePtr, TextureWPtr>();
}

TextureManager* TextureManager::getInstance(){
    if (mInstance == nullptr){
        mInstance = new TextureManager();
    }
    return mInstance;
}

TextureHnd TextureManager::addTexture(std::string name, std::string uri, WindowHnd win, bool allowOverwrite){
    if (allowOverwrite || !has(name)){
        if (!hasByURI(uri)){
            try{
                TexturePtr t = TexturePtr(new Texture(uri, win));
                mResources.insert(std::pair<std::string, TexturePtr>(name, t));
                return TextureHnd(t);
            } catch (std::runtime_error e){
                throw e;
            }
        }
    }
    return TextureHnd();
}



} // End namespace "engine"
