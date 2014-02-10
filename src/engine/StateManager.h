#ifndef STATEMANAGER_H
#define STATEMANAGER_H

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

#include "States.h"


namespace engine{


class IStateManager
{
    public:
        virtual void addState(StatePtr s)=0;
        virtual void swapState(StatePtr s)=0;
        virtual void elevateState(StatePtr s)=0;
        virtual void dropState()=0;

        virtual StatePtr currentState()=0;

        virtual void clear()=0;
        virtual bool empty()=0;
        virtual unsigned int size()=0;

    protected:
        IStateManager(){}
    private:
};


} // End namespace "engine"

#endif // STATEMANAGER_H
