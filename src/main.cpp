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

/*class SDLApp : public Application{
public:
    SDLApp(){
        mTextureManager = TextureManager::getInstance();
        mWindowManager = WindowManager::getInstance();
    }

    ~SDLApp(){
        ;
    }

    void run(){
        mRunning = true;
        WindowPtr win = mWindowManager->createWindow("Window1", "Demo Application", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480).lock();
        if (win.get() == 0){
            std::cout << "Failed to obtain window.";
            mRunning = false;
        }

        TexturePtr t = mTextureManager->addTexture("background", "cb.bmp", win).lock();
        if (t.get() == 0){
            std::cout << "Failed to load background texture.";
            mRunning = false;
        }

        win->setDrawColor(255, 128, 64);
        while (this->isRunning()){
            poll();
            win->clear();
            t->draw(0, 0);
            win->present();
        }
    }


protected:
    TextureManager* mTextureManager;
    WindowManager* mWindowManager;
};*/


int main ( int argc, char** argv )
{
    int success = 0;
    /*SDLApp* app = new SDLApp();
    app->run();*/

    Application *app = new Application();
    // Need to add a game state here, or this next line will simply exit the appplication for all the fun that'll do me!
    app->run();

    // all is well ;)
    printf("Exited cleanly\n");

    delete app;
    return success;
}
