
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


#include "Timer.h"

Timer::Timer(): mPausedTicks(0), mStartTicks(0){}

void Timer::restart(){
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void Timer::start(){
    if (mStartTicks == 0){
        mStartTicks = SDL_GetTicks();
    }
}

int Timer::stop(){
    int t = ticks();
    mStartTicks = 0;
    mPausedTicks = 0;
    return t;
}

int Timer::pause(){
    if (mStartTicks > 0 && mPausedTicks == 0){
        mPausedTicks = SDL_GetTicks() - mStartTicks;
    }
    return ticks();
}

void Timer::resume(){
    if (mPausedTicks > 0){
        mStartTicks = SDL_GetTicks() - mPausedTicks;
        mPausedTicks = 0;
    }
}

int Timer::ticks(){
    if (mStartTicks > 0){
        if (mPausedTicks > 0){
            return mPausedTicks;
        }
        return SDL_GetTicks() - mStartTicks;
    }
    return 0;
}

bool Timer::started(){
    return mStartTicks > 0;
}

bool Timer::paused(){
    return mPausedTicks > 0;
}





