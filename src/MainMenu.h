#ifndef MAINMENU_H
#define MAINMENU_H

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
#include <string>

#include <SDL2/SDL.h>

#include "Common.h"
#include "engine/States.h"
#include "engine/Updateables.h"
#include "engine/Renderables.h"
#include "engine/GameStateManager.h"
#include "engine/WindowManager.h"
#include "engine/TextureManager.h"
#include "engine/Writer.h"
#include "engine/Timer.h"


class MainMenu : public engine::IState, public engine::IUpdateable, public engine::IRenderable
{
    public:
        MainMenu(engine::GameStateManagerHnd gsm);
        ~MainMenu();

        void start();
        void stop();

        void getFocus();
        void looseFocus();

        void update();
        void render();

    private:
        static const std::string TEXTURE_BACKGROUND_NAME;
        static const std::string CODE_STREAM_TEXT;

        engine::GameStateManagerHnd mGameStateManager;
        engine::WriterHnd mWriter;
        engine::WindowHnd mWindow;
        engine::TextureHnd mTexBackground;
        bool mHasFocus;
        int mLogicalRenderWidth;
        int mLogicalRenderHeight;

        std::vector<std::string> mCodeStreamList;
        int mCodeStreamIndex;
        std::vector<SDL_Texture*> mCodeStreamTextures;
        Timer mCodeStreamTimer;


        // NOTE: This method will die doon!
        void poll();
        void splitString(std::string s, std::string delimiter, std::vector<std::string> *container);

        void updateCodeStream(int steps, int viewHeight);
        void renderCodeStream(int x, int y, int viewWidth, int viewHeight);
        void clearCodeStreamTextures();
};

#endif // MAINMENU_H
