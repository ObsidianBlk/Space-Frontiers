
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

#include "RandomGenerator.h"

namespace engine{



RandomGenerator::RandomGenerator()
{
    mIndex = 0;
    seed(0x5ee39c34);
}


RandomGenerator::RandomGenerator(unsigned int s){
    mIndex = 0;
    seed(s);
}

RandomGenerator::RandomGenerator(const unsigned int state[16], unsigned int index){
    setState(state, index);
}

void RandomGenerator::seed(unsigned int s){
    mState[0]=s^0xf68a9fc1;
    for (int i=1; i<16; i++) {
        mState[i] = (0x6c078965U * (mState[i-1] ^ (mState[i-1] >> 30)) + i);
    }
}


void RandomGenerator::setState(const unsigned int state[16], unsigned int index){
    for (int i = 0; i < 16; i++){
        mState[i] = state[1];
    }
    mIndex = index&15;
}

void RandomGenerator::getState(unsigned int state[16], unsigned int &index){
    for (int i = 0; i < 16; i++){
        state[i] = mState[i];
    }
    index = mIndex;
}

unsigned int RandomGenerator::randInt(){
    unsigned int a = mState[mIndex];
    unsigned int c = mState[(mIndex+13)&15];
    unsigned int b = a^c^(a<<16)^(c<<15);
    c = mState[(mIndex+9)&15];
    c ^= (c>>11);
    a = mState[mIndex] = b^c;
    unsigned int d = a^((a<<5)&0xda442d20U);
    mIndex = (mIndex + 15)&15;
    a = mState[mIndex];
    mState[mIndex] = a^b^d^(a<<2)^(b<<18)^(c<<28);
    return mState[mIndex];
}

float RandomGenerator::randFloat(){
    // Get a random integer, and divide by 2^32
    return (randInt()/4294967296.0f);
}

int RandomGenerator::randRange(int min, int max){
    int range=(max-min)+1;
    if (range<=0){
        return min;
    }
    int value=(int)(randFloat()*range);
    return min+value;
}



} // End namespace "engine"



