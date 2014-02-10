#ifndef HANDLER_H
#define HANDLER_H

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


#include <memory>


namespace engine {

    template<typename T>
    class Handler{
    public:
        Handler(){mRef = std::weak_ptr<T>();}
        Handler(std::weak_ptr<T> ptr){mRef = ptr;}
        Handler(std::shared_ptr<T> ptr){mRef = std::weak_ptr<T>(ptr);}

        bool IsValid(){return mRef.lock().get() != 0;}

        std::shared_ptr<T> operator->(){return mRef.lock();}
        const std::shared_ptr<T> operator->(){return mRef.lock();}

        inline bool operator==(const Handler<T> lhs, const Handler<T> rhs){
            return lhs.getReference().lock().get() == rhs.getReference().lock().get();
        }
        inline bool operator!=(const Handler<T> lhs, const Handler<T> rhs){return !operator==(lhs, rhs);}


    protected:

        std::weak_ptr<T> GetReference(){return mRef;}
        void SetReference(std::weak_ptr<T> ptr){mRef = ptr;}
        void SetReference(std::shared_ptr<T> ptr){mRef = std::weak_ptr<T>(ptr);}


    private:
        std::weak_ptr<T> mRef;
    };


}

#endif // HANDLER_H
