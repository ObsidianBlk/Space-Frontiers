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

#include "Resource.h"

#include <stdexcept>
#include <cstdio>


namespace engine{


Resource::Resource(){mURI = "";}

Resource::Resource(std::string uri){
    if (!resourceExists(uri)){
        throw std::runtime_error("Resource doesn't exist.");
    }
    mURI = uri;
}

std::string Resource::getURI(){
    return mURI;
}

bool Resource::resourceExists(std::string uri){
    if (uri != ""){
        // TODO: (MAYBE) Extend this to allow for a virtual file system at a later date.
        if (FILE* file = fopen(uri.c_str(), "r")){
            fclose(file);
            return true;
        }
    }
    return false;
}


} // End namespace "engine"
