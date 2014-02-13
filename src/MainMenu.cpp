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

#include "MainMenu.h"

const std::string MainMenu::WINDOW_RESOURCE_NAME = "MainWindow";
const std::string MainMenu::WINDOW_RESOURCE_TITLE = "Space Frontiers";
const std::string MainMenu::TEXTURE_BACKGROUND_NAME = "tBackground";


MainMenu::MainMenu(engine::GameStateManagerHnd gsm){
    mHasFocus = false;
    mWindow = engine::WindowHnd();
    mTexBackground = engine::TextureHnd();

    if (gsm.IsValid()){
        gsm->addState(engine::StatePtr(this));
    } else {
        throw std::runtime_error("Failed to obtain GameStateManager object.");
    }
    mGameStateManager = gsm;
}

MainMenu::~MainMenu(){}



void MainMenu::start(){
    // First either, get or create the main window.
    engine::WindowManager* wm = engine::WindowManager::getInstance();
    if (wm->has(MainMenu::WINDOW_RESOURCE_NAME)){
        mWindow = wm->get(MainMenu::WINDOW_RESOURCE_NAME);
    } else {
        try{
            mWindow = wm->createWindow(MainMenu::WINDOW_RESOURCE_NAME, MainMenu::WINDOW_RESOURCE_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600);
            mWindow->setLogicalRendererSize(1680, 1050);
        } catch (std::runtime_error e){
            throw e;
        }
    }

    // Make sure we really do have a window.
    if (!mWindow.IsValid()){
        throw std::runtime_error("Failed to obtain window.");
    }


    // Second, let's get or add the textures we need.
    engine::TextureManager* tm = engine::TextureManager::getInstance();
    if (tm->has(MainMenu::TEXTURE_BACKGROUND_NAME)){
        mTexBackground = tm->get(MainMenu::TEXTURE_BACKGROUND_NAME);
    } else {
        mTexBackground = tm->addTexture(MainMenu::TEXTURE_BACKGROUND_NAME, "assets/textures/mmbackground.jpg", mWindow);
    }
    if (!mTexBackground.IsValid()){
        throw std::runtime_error("Failed to obtain texture resource.");
    }

    mHasFocus = true;
}

void MainMenu::stop(){}

void MainMenu::getFocus(){
        mHasFocus = true;
}

void MainMenu::looseFocus(){
        mHasFocus = false;
}

void MainMenu::update(){
    if (mHasFocus){
        poll();
    }
}

void MainMenu::render(){
    if (mHasFocus){
        if (mWindow.IsValid() && mTexBackground.IsValid()){
            mWindow->clear();
            mTexBackground->draw(0, 0);
            mWindow->present();
        }
    }
}



// This is soon to DIE!
void MainMenu::poll(){
    // message processing loop
    SDL_Event event;
         while (SDL_PollEvent(&event))
    {
        // check for messages
        switch (event.type)
        {
            // exit if the window is closed
        case SDL_QUIT:
            if (mGameStateManager.IsValid()){
                // THIS state should have focus when this occures, we we're dropping ourselved here!
                mGameStateManager->dropState();
            }
            break;

            // check for keypresses
        case SDL_KEYDOWN:
            {
                // exit if ESCAPE is pressed
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    if (mGameStateManager.IsValid()){
                        // THIS state should have focus when this occures, we we're dropping ourselved here!
                        mGameStateManager->dropState();
                    }
                break;
            }
        } // end switch
    } // end of message processing
}