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

#include "GameStateManager.h"

GameStateManager::GameStateManager(){}

GameStateManager::~GameStateManager()
{
    clear();
}


void GameStateManager::addState(StatePtr &s){
    StatePtr cstate = currentState();
    if (cstate.get() != 0){
        cstate->looseFocus();
    }
    mStateStack.push_back(s);
    AddUpdateableOrRenderable(s.get());
    s->start();
}

void GameStateManager::swapState(StatePtr &s){
    if (!mStateStack.empty()){
        dropState();
    }
    addState(s);
}

void GameStateManager::elevateState(StatePtr &s){
    RemoveFromStack(s); // If s is not already on the stack, this does nothing.
    RebuildUpdateablesAndRenderables();
    addState(s);
}

void GameStateManager::dropState(){
    if (!mStateStack.empty()){
        mStateStack.at(mStateStack.size()-1)->stop();
        DropUpdateableOrRenderable(mStateStack.at(mStateStack.size()-1).get());
        mStateStack.pop_back();
    }
}

StatePtr GameStateManager::currentState(){
    if (!mStateStack.empty()){
        return mStateStack.at(mStateStack.size()-1);
    }
    return StatePtr();
}


void GameStateManager::clear(){
    while (!mStateStack.empty()){
        dropState();
    }
}


bool GameStateManager::empty(){
    return mStateStack.empty();
}

unsigned int GameStateManager::size(){
    return mStateStack.size();
}


void GameStateManager::update(){
    for (size_t index = 0; index < mUpdateables.size(); index ++){
        mUpdateables.at(index)->update();
    }
}

void GameStateManager::render(){
    for (size_t index = 0; index < mRenderables.size(); index++){
        mRenderables.at(index)->render();
    }
}

// -----------------------------------------------------------------------------

bool GameStateManager::RemoveFromStack(StatePtr &s){
    for (size_t index = 0; index < mStateStack.size(); index++){
        if (mStateStack.at(index).get() == s.get()){
            mStateStack.erase(mStateStack.begin()+index);
            return true;
        }
    }
    return false;
}

void GameStateManager::AddUpdateableOrRenderable(IState* state){
    IRenderable *r = dynamic_cast<IRenderable *>(state);
    if(r){
        mRenderables.push_back(r);
    }

    IUpdateable *u = dynamic_cast<IUpdateable *>(state);
    if(u){
        mUpdateables.push_back(u);
    }
}


void GameStateManager::DropUpdateableOrRenderable(IState* state){
    IRenderable *r = dynamic_cast<IRenderable *>(state);
    if(r){
        mRenderables.pop_back();
    }

    IUpdateable *u = dynamic_cast<IUpdateable *>(state);
    if(u){
        mUpdateables.pop_back();
    }
}

void GameStateManager::RebuildUpdateablesAndRenderables(){
    mRenderables.clear();
    mUpdateables.clear();

    if (!mStateStack.empty()){
        for (size_t index = 0; index < mStateStack.size(); index++){
            AddUpdateableOrRenderable(mStateStack.at(index).get());
        }
    }
}



