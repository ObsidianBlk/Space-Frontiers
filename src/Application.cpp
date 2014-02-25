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
    // While we create the GameStateManager in the Application class, only decendants can access and push a state to it.
    mGameStateManager = engine::GameStateManagerPtr(new engine::GameStateManager());
    engine::WindowManager* wm = engine::WindowManager::getInstance();
    engine::WindowHnd w = engine::WindowHnd();
    try{
        w = wm->createWindow(MAINWINDOW_RESOURCE_NAME, MAINWINDOW_RESOURCE_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600);
    } catch (std::runtime_error e){
        throw e;
    }

    // Make sure we really do have a window.
    if (!w.IsValid()){
        throw std::runtime_error("Failed to create main application window.");
    }
    w->setLogicalRendererSize(1680, 1050);

    engine::WriterHnd writer = engine::Writer::getHandle();
    if (writer.get() != 0){
        writer->defineFont("default8", "assets/fonts/6809chargen.ttf", 8);
        writer->defineFont("default12", "assets/fonts/6809chargen.ttf", 12);
        writer->defineFont("default24", "assets/fonts/6809chargen.ttf", 24);
    }
}

Application::~Application()
{
    // make sure SDL cleans up before exit
    //atexit(SDL_Quit);
}

void Application::run(){
    while (not mGameStateManager->empty()){
        mGameStateManager->poll();
        mGameStateManager->update();
        mGameStateManager->render();
    }
}

engine::GameStateManagerHnd Application::getGameStateManager(){
    if (mGameStateManager.get() != 0){
        return engine::GameStateManagerHnd(mGameStateManager);
    }
    return engine::GameStateManagerHnd();
}



