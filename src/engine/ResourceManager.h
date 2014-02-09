#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

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

#include <string>
#include <map>

namespace engine{


template <typename T, typename H>
/**
* The base resource management class for all future resource managers.
*/
class ResourceManager
{
    public:
        ResourceManager(){}

        bool has(std::string name){
            return mResources.find(name) != mResources.end();
        }

        bool hasByURI(std::string uri){
            for(ResourceMapIter item = mResources.begin(); item != mResources.end(); item++){
                if (item->second->getURI() == uri){
                    return true;
                }
            }
            return false;
        }

        H get(std::string name){
            ResourceMapIter item = mResources.find(name);
            if (item != mResources.end()){
                return H(item->second);
            }
            return H();
        }

        H getByURI(std::string uri){
            for(ResourceMapIter item = mResources.begin(); item != mResources.end(); item++){
                if (item->second->getURI() == uri){
                    return H(item->second);
                }
            }
            return H();
        }

    protected:
        typedef std::map<std::string, T> ResourceMap;
        typedef typename ResourceMap::iterator ResourceMapIter;
        ResourceMap mResources;
};


} // End namespace "engine"

#endif // RESOURCEMANAGER_H
