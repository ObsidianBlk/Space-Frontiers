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

const std::string MainMenu::TEXTURE_BACKGROUND_NAME = "tBackground";

/*
* NOTE: Defined below is a estetically modified and comment stripped form of a C++ Sha1 class source code.
* This code was written by Michael D. Leonhard and is made available at the website http://www.tamale.net/sha1/sha1-0.2/
* It's been made freely available, but is being used here in the context of a "steaming code" effect. No actual application in this software.
*/
const std::string MainMenu::CODE_STREAM_TEXT = "#ifndef SHA1_HEADER\n"
    "typedef unsigned int Uint32;\n"
    " class SHA1\n"
    " {\n"
    "  private:\n"
    "   Uint32 H0, H1, H2, H3, H4;\n"
    "   unsigned char bytes[64];\n"
    "   int unprocessedBytes;\n"
    "   Uint32 size;\n"
    "   void process();\n"
	"  public:\n"
    "   SHA1();\n"
    "   ~SHA1();\n"
    "   void addBytes( const char* data, int num );\n"
    "   unsigned char* getDigest();\n"
    "   static Uint32 SHA1::lrot( Uint32 x, int bits );\n"
    "   static void SHA1::storeBigEndianUint32( unsigned char* byte, Uint32 num );\n"
    "   static void SHA1::hexPrinter( unsigned char* c, int l );\n"
    " };\n"
    "#define SHA1_HEADER\n"
    "#endif\n"
    "void SHA1::hexPrinter( unsigned char* c, int l )\n"
    "{\n"
	" assert( c );\n"
	" assert( l > 0 );\n"
	" while( l > 0 )\n"
	" {\n"
    "  printf( \" %02x\", *c );\n"
    "  l--;\n"
    "  c++;\n"
	" }\n"
    "}\n"
    "Uint32 SHA1::lrot( Uint32 x, int bits )\n"
    "{\n"
	" return (x<<bits) | (x>>(32 - bits));\n"
    "};\n"
    "void SHA1::storeBigEndianUint32( unsigned char* byte, Uint32 num )\n"
    "{\n"
	" assert( byte );\n"
    " byte[0] = (unsigned char)(num>>24);\n"
	" byte[1] = (unsigned char)(num>>16);\n"
	" byte[2] = (unsigned char)(num>>8);\n"
	" byte[3] = (unsigned char)num;\n"
    "}\n"
    "SHA1::SHA1()\n"
    "{\n"
	" assert( sizeof( Uint32 ) * 5 == 20 );\n"
    " H0 = 0x67452301;\n"
	" H1 = 0xefcdab89;\n"
	" H2 = 0x98badcfe;\n"
	" H3 = 0x10325476;\n"
	" H4 = 0xc3d2e1f0;\n"
	" unprocessedBytes = 0;\n"
	" size = 0;\n"
    "}\n"
    "SHA1::~SHA1()\n"
    "{\n"
    " H0 = H1 = H2 = H3 = H4 = 0;\n"
	" for( int c = 0; c < 64; c++ ) bytes[c] = 0;\n"
	" unprocessedBytes = size = 0;\n"
    "}\n"
    "void SHA1::process()\n"
    "{\n"
	" assert( unprocessedBytes == 64 );\n"
    " int t;\n"
	" Uint32 a, b, c, d, e, K, f, W[80];\n"
    " a = H0;\n"
	" b = H1;\n"
	" c = H2;\n"
	" d = H3;\n"
	" e = H4;\n"
	" for( t = 0; t < 16; t++ ) W[t] = (bytes[t*4] << 24)\n"
    "      +(bytes[t*4 + 1] << 16)\n"
    "      +(bytes[t*4 + 2] << 8)\n"
    "      + bytes[t*4 + 3];\n"
	" for(; t< 80; t++ ) W[t] = lrot( W[t-3]^W[t-8]^W[t-14]^W[t-16], 1 );\n"
	" Uint32 temp;\n"
	" for( t = 0; t < 80; t++ )\n"
	" {\n"
    "  if( t < 20 ) {\n"
    "   K = 0x5a827999;\n"
    "   f = (b & c) | ((b ^ 0xFFFFFFFF) & d);\n"
    "  } else if( t < 40 ) {\n"
    "   K = 0x6ed9eba1;\n"
    "   f = b ^ c ^ d;\n"
    "  } else if( t < 60 ) {\n"
    "   K = 0x8f1bbcdc;\n"
    "   f = (b & c) | (b & d) | (c & d);\n"
    "  } else {\n"
    "   K = 0xca62c1d6;\n"
    "   f = b ^ c ^ d;\n"
    "  }\n"
    "  temp = lrot(a,5) + f + e + W[t] + K;\n"
    "  e = d;\n"
    "  d = c;\n"
    "  c = lrot(b,30);\n"
    "  b = a;\n"
    "  a = temp;\n"
	" }\n"
	" H0 += a;\n"
	" H1 += b;\n"
	" H2 += c;\n"
	" H3 += d;\n"
	" H4 += e;\n"
	" unprocessedBytes = 0;\n"
    "}\n"
    "void SHA1::addBytes( const char* data, int num )\n"
    "{\n"
	" assert( data );\n"
	" assert( num > 0 );\n"
	" size += num;\n"
	" while( num > 0 )\n"
	" {\n"
    "  int needed = 64 - unprocessedBytes;\n"
    "  assert( needed > 0 );\n"
    "  int toCopy = (num < needed) ? num : needed;\n"
    "  memcpy( bytes + unprocessedBytes, data, toCopy );\n"
    "  num -= toCopy;\n"
    "  data += toCopy;\n"
    "  unprocessedBytes += toCopy;\n"
    "  if( unprocessedBytes == 64 ) process();\n"
	" }\n"
    "}\n"
    "unsigned char* SHA1::getDigest()\n"
    "{\n"
	" Uint32 totalBitsL = size << 3;\n"
    " Uint32 totalBitsH = size >> 29;\n"
	" addBytes( \"\\x80\", 1 );\n"
	" unsigned char footer[64] = {\n"
    "   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n"
    "   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n"
    "   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n"
    "   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };\n"
	" if( unprocessedBytes > 56 )\n"
    "  addBytes( (char*)footer, 64 - unprocessedBytes);\n"
	" assert( unprocessedBytes <= 56 );\n"
	" int neededZeros = 56 - unprocessedBytes;\n"
	" storeBigEndianUint32( footer + neededZeros    , totalBitsH );\n"
	" storeBigEndianUint32( footer + neededZeros + 4, totalBitsL );\n"
	" addBytes( (char*)footer, neededZeros + 8 );\n"
	" unsigned char* digest = (unsigned char*)malloc( 20 );\n"
	" storeBigEndianUint32( digest, H0 );\n"
	" storeBigEndianUint32( digest + 4, H1 );\n"
	" storeBigEndianUint32( digest + 8, H2 );\n"
	" storeBigEndianUint32( digest + 12, H3 );\n"
	" storeBigEndianUint32( digest + 16, H4 );\n"
	" return digest;\n"
    "}\n";


MainMenu::MainMenu(engine::GameStateManagerHnd gsm){
    mHasFocus = false;
    mMenuItemID = 0;
    mLogicalRenderWidth = 0;
    mLogicalRenderHeight = 0;
    mCodeStreamIndex = 0;
    mWindow = engine::WindowHnd();
    mTexBackground = engine::TextureHnd();

    mMenuItems.resize(3);
    mMenuItems[0].itemName = "Start Game";
    mMenuItems[1].itemName = "Options";
    mMenuItems[2].itemName = "Quit";

    // Hehe... tripped myself up...
    // NOTHING AFTER THIS LINE!!
    if (gsm.IsValid()){
        gsm->addState(engine::StatePtr(this));
    } else {
        throw std::runtime_error("Failed to obtain GameStateManager object.");
    }
    mGameStateManager = gsm;
}

MainMenu::~MainMenu(){
    stop();
    clearCodeStreamTextures();
}



void MainMenu::start(){
    // First either, get or create the main window.
    engine::WindowManager* wm = engine::WindowManager::getInstance();
    mWindow = wm->get(MAINWINDOW_RESOURCE_NAME);

    // Make sure we really do have a window.
    if (!mWindow.IsValid()){
        throw std::runtime_error("Failed to obtain window.");
    }
    mWindow->getLogicalRendererSize(&mLogicalRenderWidth, &mLogicalRenderHeight);

    // Second, let's get or add the textures we need.
    /*engine::TextureManager* tm = engine::TextureManager::getInstance();
    if (tm->has(MainMenu::TEXTURE_BACKGROUND_NAME)){
        mTexBackground = tm->get(MainMenu::TEXTURE_BACKGROUND_NAME);
    } else {
        mTexBackground = tm->addTexture(MainMenu::TEXTURE_BACKGROUND_NAME, "assets/textures/mmbackground.jpg", mWindow);
    }
    if (!mTexBackground.IsValid()){
        throw std::runtime_error("Failed to obtain texture resource.");
    }*/

    mWriter = engine::Writer::getHandle();
    if (!mWriter.IsValid()){
        throw std::runtime_error("Failed to obtain Writer object.");
    }
    if (!mWriter->hasFont("default8") || !mWriter->hasFont("default12") || !mWriter->hasFont("default24")){
        throw std::runtime_error("Required font not defined.");
    }
    //mWriter->setPenColor(64, 255, 64);
    preRenderMenuItems();

    // Now split that really big const string defined at the top of this file into vector.
    splitString(CODE_STREAM_TEXT, "\n", &mCodeStreamList);
    mHasFocus = true;
}

void MainMenu::stop(){
    for (int i = 0; i < mMenuItems.size(); i++){
        SDL_DestroyTexture(mMenuItems.at(i).texIdle);
        SDL_DestroyTexture(mMenuItems.at(i).texSelected);
    }
    mMenuItems.clear();
}

void MainMenu::getFocus(){
        mHasFocus = true;
}

void MainMenu::looseFocus(){
        mHasFocus = false;
}

void MainMenu::update(){
    if (mHasFocus){
        if (!mCodeStreamTimer.started()){mCodeStreamTimer.start(50);}
        updateCodeStream(mCodeStreamTimer.steps(), 400);
    }
}

void MainMenu::render(){
    SDL_Rect src;
    SDL_Rect dst;

    if (mHasFocus && mWindow.IsValid()){
        mWindow->clear();

        mWindow->setPenColor(0, 255, 0);
        //mWindow->drawLine(0, 5, 200, 5);
        //mTexBackground->render(0, 0);
        //if (mWriter.IsValid()){
        //    mWriter->presentToWindow(mWindow, "default", "On a dark and stormy night in the middle of the universe", 10, 10);
        //}

        dst.x = 50;
        dst.y = 200;
        for (int i = 0; i < mMenuItems.size(); i++){
            SDL_Texture* tex = nullptr;
            if (mMenuItemID == i){
                tex = mMenuItems.at(i).texSelected;
            } else {
                tex = mMenuItems.at(i).texIdle;
            }

            SDL_QueryTexture(tex, nullptr, nullptr, &src.w, &src.h);
            src.x = src.y = 0;
            dst.w = src.w;
            dst.h = src.h;
            mWindow->render(tex, &src, &dst);
            dst.y += src.h+2;
        }

        renderCodeStream(1240, 400, 400, 400);
        mWindow->present();
    }
}


// PRIVATE
void MainMenu::updateCodeStream(int steps, int viewHeight){
    if (steps > 0 && mWindow.IsValid() && mWriter.IsValid()){
        int fontHeight = mWriter->getFontPixelHeight("default12");

        if (mCodeStreamList.size() > 0 && fontHeight > 0 && viewHeight > 0 && viewHeight > fontHeight){
            int maxViewableLines = viewHeight/fontHeight;

            mWriter->setPenColor(64, 255, 64);
            // Ok... now... I'm managing an EXTREAM case where there could be a HUGE delay between calls.
            if (steps > maxViewableLines){
                mCodeStreamIndex = maxViewableLines - ((mCodeStreamIndex+steps)%maxViewableLines);
                clearCodeStreamTextures();
                for (int i = 0; i < maxViewableLines; i++){
                    SDL_Texture* tex = mWriter->textToTexture(mWindow, "default12", mCodeStreamList.at(mCodeStreamIndex));
                    mCodeStreamIndex++;
                    if (mCodeStreamIndex >= mCodeStreamList.size())
                        mCodeStreamIndex = 0;

                    if (tex != nullptr){
                        mCodeStreamTextures.push_back(tex);
                    }
                }
            } else {
                for (int i = 0; i < steps; i++){
                    // Build the new string texture and store it.
                    SDL_Texture* tex = mWriter->textToTexture(mWindow, "default12", mCodeStreamList.at(mCodeStreamIndex));
                    mCodeStreamIndex++;
                    if (mCodeStreamIndex >= mCodeStreamList.size())
                        mCodeStreamIndex = 0;

                    if (tex != nullptr){
                        // If we already have maxViewableLines of textures, destroy and remove the oldest one.
                        while (mCodeStreamTextures.size() >= maxViewableLines){
                            SDL_DestroyTexture(mCodeStreamTextures.front());
                            mCodeStreamTextures.erase(mCodeStreamTextures.begin());
                        }
                        mCodeStreamTextures.push_back(tex);
                    }
                }
            }
        }
    }
}

// PRIVATE
void MainMenu::renderCodeStream(int x, int y, int viewWidth, int viewHeight){
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;

    if (mWindow.IsValid() && mWriter.IsValid() && mCodeStreamTextures.size() > 0){
        int fontHeight = mWriter->getFontPixelHeight("default12");

        // Render the string textures we have!
        for (int index = 0; index < mCodeStreamTextures.size(); index++){
            SDL_Texture* t = mCodeStreamTextures.at(index);
            SDL_Rect src;
            src.x = 0;
            src.y = 0;
            SDL_QueryTexture(t, nullptr, nullptr, &src.w, &src.h);
            if (src.w > viewWidth){
                src.w = viewWidth;
            }
            dst.w = src.w;
            dst.h = src.h;

            mWindow->render(t, &src, &dst);
            dst.y += fontHeight;
        }
    }
}


bool MainMenu::poll(SDL_Event event){
    // check for messages
    switch (event.type)
    {
        // exit if the window is closed
    case SDL_QUIT:
        if (mGameStateManager.IsValid()){
            // THIS state should have focus when this occures, we we're dropping ourselved here!
            mGameStateManager->dropState();
        }
        return true;

        // check for keypresses
    case SDL_KEYDOWN:
        {
            // exit if ESCAPE is pressed
            if (event.key.keysym.sym == SDLK_ESCAPE){
                if (mGameStateManager.IsValid()){
                    // THIS state should have focus when this occures, we we're dropping ourselved here!
                    mGameStateManager->dropState();
                }
            } else if (event.key.keysym.sym == SDLK_DOWN){
                if (mMenuItemID < mMenuItems.size()-1){
                    mMenuItemID++;
                }
            } else if (event.key.keysym.sym == SDLK_UP){
                if (mMenuItemID > 0){
                    mMenuItemID--;
                }
            } else if (event.key.keysym.sym == SDLK_RETURN){
                if (mMenuItems.at(mMenuItemID).itemName == std::string("Quit")){
                    if (mGameStateManager.IsValid()){
                        mGameStateManager->dropState();
                    }
                }
            }
            return true;
        }
    } // end switch
    return false;
}

// PRIVATE
void MainMenu::clearCodeStreamTextures(){
    while (!mCodeStreamTextures.empty()){
        SDL_DestroyTexture(mCodeStreamTextures.back());
        mCodeStreamTextures.pop_back();
    }
}


void MainMenu::splitString(std::string s, std::string delimiter, std::vector<std::string> *container){
    // Code modified from original source supplied by "Vincenzo Pii" at
    // http://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    size_t pos = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        container->push_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.length());
    }
    // This will either store the tailing remanents of the given string, or the whole string (if the delimiter was not found at all)
    container->push_back(s);
}


void MainMenu::preRenderMenuItems(){
    for (int n = 0; n < mMenuItems.size(); n++){
        sMenuItemInfo* i = &mMenuItems.at(n);
        mWriter->getFontStringTextSize("default24", i->itemName, &i->width, &i->height);

        mWriter->setPenColor(64, 128, 64);
        i->texIdle = mWriter->textToTexture(mWindow, "default24", i->itemName);
        mWriter->setPenColor(64, 255, 64);
        i->texSelected = mWriter->textToTexture(mWindow, "default24", i->itemName);
    }
}



