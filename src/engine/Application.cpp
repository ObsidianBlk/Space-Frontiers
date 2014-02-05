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

#include "Application.h"


Application::Application(){
    mRunning = false;
}

Application::~Application()
{
    // make sure SDL cleans up before exit
    //atexit(SDL_Quit);
}

void Application::run(){}

void Application::poll(){
    // message processing loop
    SDL_Event event;
         while (SDL_PollEvent(&event))
    {
        // check for messages
        switch (event.type)
        {
            // exit if the window is closed
        case SDL_QUIT:
            mRunning = false;
            break;

            // check for keypresses
        case SDL_KEYDOWN:
            {
                // exit if ESCAPE is pressed
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    mRunning = false;
                break;
            }
        } // end switch
    } // end of message processing
}

WindowWPtr Application::createWindow(std::string wname, std::string title, int x, int y, int w, int h, Uint32 wflags, Uint32 rflags){
    if (mWindows.find(wname) == mWindows.end()){
        WindowPtr win(new Window(title, x, y, w, h, wflags, rflags));
        mWindows.insert(std::pair<std::string, WindowPtr>(wname, win));
        return WindowWPtr(win);
    }
    return WindowWPtr();
}



