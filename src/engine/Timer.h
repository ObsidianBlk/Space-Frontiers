#ifndef TIMER_H
#define TIMER_H

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


#include <SDL2/SDL.h>


class Timer
{
    public:
        Timer();

        /**
        * Resets the timer's starting value. If the timer isn't already started, this has the same effect as calling the start() method.
        */
        void restart(int stepTime);
        void restart();
        void start(int stepTime);
        void start();
        int stop();
        int pause();
        void resume();

        int ticks();
        int steps();

        int getDefinedStepTime();
        bool started();
        bool paused();

    private:
        int mPausedTicks;
        int mStartTicks;

        int mStepTime;  // # milliseconds to consider to be 1 step.
        int mStepTicks; // Timestamp last time steps were checked.
        int mAccumulatedTicks; // A collections of ticks between steps, adjusted by passed steps.
};

#endif // TIMER_H
