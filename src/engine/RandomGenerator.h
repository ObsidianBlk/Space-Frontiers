#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

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


#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif


/*
* Random number generation, using the WELL algorithm by F. Panneton, P. L'Ecuyer and M. Matsumoto.
*
* More information in the original paper: http://www.iro.umontreal.ca/~panneton/WELLRNG.html
*
* This code is originally based on WELL512 C/C++ code written by Chris Lomont (published in Game Programming Gems 7)
* and placed in the public domain. There's also an article about it on Lomont's site:
*
*      http://lomont.org/Math/Papers/2008/Lomont_PRNG_2008.pdf
*
* ----------------
* Code modified from posted source
*      http://forums.indiegamer.com/showthread.php?21510-help-me-with-weird-random-number-thing!
* Posted by Mattias Gustavsson  (Thank you!)
*/

class RandomGenerator{
public:
    RandomGenerator();
    RandomGenerator(unsigned int s);
    RandomGenerator(const unsigned int state[16], unsigned int index);

    void seed(unsigned int s);
    void setState(const unsigned int state[16], unsigned int index);
    void getState(unsigned int state[16], unsigned int &index);

    unsigned int randInt();
    float randFloat();
    int randRange(int min, int max);

private:
    unsigned int mState[16];
    unsigned int mIndex;
};

#endif // RANDOMGENERATOR_H
