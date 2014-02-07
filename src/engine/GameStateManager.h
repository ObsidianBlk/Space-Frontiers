#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

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


#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "StateManager.h"
#include "Updateables.h"
#include "Renderables.h"
#include "States.h"

class GameStateManager;
typedef boost::shared_ptr<GameStateManager> GameStateManagerPtr;
typedef boost::weak_ptr<GameStateManager> GameStateManagerWPtr;

class GameStateManager : public IStateManager, public IUpdateable, public IRenderable
{
    public:
        GameStateManager();
        ~GameStateManager();

        /**
        * Adds the given state to the head of the state stack. If other states exist, the current state is notified it is loosing focus and the given state is notified it now has
        & primary focus.
        */
        void addState(StatePtr &s);

        /**
        * Replaces the current state with the given state, removing the current state from the stack.
        * If there are no states on the stack, this has the same effect as addState.
        */
        void swapState(StatePtr &s);

        /**
        * Removes the given state from the stack and readds it as the current state.
        * If the given state is not already on the stack, this has the same effect as addState
        */
        void elevateState(StatePtr &s);

        /**
        * Removes the current state from the stack and reports to the state just below (if there is one) that it now has primary focus.
        */
        void dropState();

        /**
        * Returns pointer to the current state on the state stack.
        */
        StatePtr currentState();

        /**
        * This removes ALL of the states from the State Manager.
        */
        void clear();

        /**
        * Returns true if there are no states on the stack and false otherwise.
        */
        bool empty();

        /**
        * Returns the number of states on the stack.
        */
        unsigned int size();

        void update();
        void render();

    private:
        typedef std::vector<StatePtr> StateVec;
        StateVec mStateStack;

        typedef std::vector<IUpdateable *> UpdateableVec;
        UpdateableVec mUpdateables;

        typedef std::vector<IRenderable *> RenderableVec;
        RenderableVec mRenderables;


        /**
        * If s is found on the existing stack, it will remove s from the stack and return true. If s is not on the stack, false is returned.
        */
        bool RemoveFromStack(StatePtr &s);

        void AddUpdateableOrRenderable(IState* state);
        void DropUpdateableOrRenderable(IState* state);
        void RebuildUpdateablesAndRenderables();
};

#endif // GAMESTATEMANAGER_H
