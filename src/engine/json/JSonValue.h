#ifndef JSONVALUE_H
#define JSONVALUE_H


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


#include <algorithm>
#include <memory>

#include <sstream>

#include <string>
#include <map>
#include <vector>

namespace engine{ namespace json {

    class JSonValue;
    typedef std::shared_ptr<JSonValue> JSonValuePtr;
    typedef std::pair<std::string, JSonValue> JSonObjectPair;
    typedef std::map<std::string, JSonValue> JSonObject;
    typedef std::map<std::string, JSonValue>::iterator JSonObjectIter;
    typedef std::shared_ptr<JSonObject> JSonObjectPtr;
    typedef std::vector<JSonValuePtr> JSonArray;
    typedef std::vector<JSonValuePtr>::iterator JSonArrayIter;
    typedef std::shared_ptr<JSonArray> JSonArrayPtr;

    enum JSonType {JSonType_Object, JSonType_Array, JSonType_Number, JSonType_String, JSonType_Bool, JSonType_Null};

    // TODO: Use these in JSonValue.cpp
    static const char OBJECT_PAIR_SEPARATOR = ':';
    static const char VALUE_SEPARATOR = ',';
    static const char OBJECT_SYM_HEAD = '{';
    static const char OBJECT_SYM_TAIL = '}';
    static const char ARRAY_SYM_HEAD = '[';
    static const char ARRAY_SYM_TAIL = ']';

    class JSonValue
    {
        public:
            static std::string Key_Separator;

            JSonValue();
            explicit JSonValue(JSonObjectPtr value);
            explicit JSonValue(JSonArrayPtr value);
            explicit JSonValue(const std::string& value);
            explicit JSonValue(const char* value);
            explicit JSonValue(double value);
            explicit JSonValue(int value);
            explicit JSonValue(float value);
            explicit JSonValue(bool value);
            ~JSonValue();

            bool is(JSonType t);
            JSonType type();
            const JSonType type() const;
            std::string type_str();
            const std::string type_str() const;

            template<typename T> const T getPtr() const;
            template<typename T> T getPtr();
            template<typename T> const T get() const;
            template<typename T> T get();
            void set(const JSonValue& value);

            std::string to_str();
            std::string serialize();
            std::string pretty_serial(std::string indentSym=" ", size_t depth=0);

            JSonValue& getKey(const std::string key, bool createMissingKey=false);
            JSonValue& getKey(const char* key, bool createMissingKey=false);

            bool hasKey(const std::string key);
            bool hasKey(const char* key);

            /**
            * Returns the data size or length in elements or bytes.
            * For JSonType_Object and JSonType_Array, the return value is the number of elements.
            * For JSonType_String the return value is the length of the string.
            * For JSonType_Number and JSonType_Bool the return value is the number of bytes those data values use.
            * For JSonType_Null the return value is always 0 (zero).
            */
            size_t size();
            size_t size() const;

            /**
            * Returns true if the JSonValue is an empty container.
            * JSonValue is only considered a container if it's a JSonType_Object or JSonType_Array.
            */
            bool empty();

            template<typename T> const T begin() const;
            template<typename T> T begin();
            template<typename T> const T end() const;
            template<typename T> T end();

            JSonValue& operator[](const std::string& key);
            JSonValue& operator[](const char* key);
            JSonValue& operator[](const size_t index);

            JSonValue& operator=(const JSonValue &rhs);
            JSonValue& operator=(JSonObjectPtr rhs);
            JSonValue& operator=(JSonArrayPtr rhs);
            JSonValue& operator=(const std::string &rhs);
            JSonValue& operator=(const char* rhs);
            JSonValue& operator=(double rhs);
            JSonValue& operator=(int rhs);
            JSonValue& operator=(float rhs);
            JSonValue& operator=(bool rhs);

            /*
            For all following comparison operators, their meaning is most clear when used against a variable of the
            same type as this, or another JSonValue holding the same type as this.

            NOTE: If comparing against non-matching types, the operator will always return false, except the
            != operator which will always return true.
            */
            bool operator==(const JSonValue& rhs) const;
            bool operator==(const JSonObjectPtr rhs) const;
            bool operator==(const JSonArrayPtr rhs) const;
            bool operator==(const std::string rhs) const;
            bool operator==(const double rhs) const;
            bool operator==(const int rhs) const;
            bool operator==(const float rhs) const;
            bool operator==(const bool rhs) const;

            bool operator!=(const JSonValue& rhs) const;
            bool operator!=(const JSonObjectPtr rhs) const;
            bool operator!=(const JSonArrayPtr rhs) const;
            bool operator!=(const std::string rhs) const;
            bool operator!=(const double rhs) const;
            bool operator!=(const int rhs) const;
            bool operator!=(const float rhs) const;
            bool operator!=(const bool rhs) const;

            bool operator<(const JSonValue& rhs) const;
            bool operator<(const std::string rhs) const;
            bool operator<(const double rhs) const;
            bool operator<(const int rhs) const;
            bool operator<(const float rhs) const;

            bool operator>(const JSonValue& rhs) const;
            bool operator>(const std::string rhs) const;
            bool operator>(const double rhs) const;
            bool operator>(const int rhs) const;
            bool operator>(const float rhs) const;

            bool operator<=(const JSonValue& rhs) const;
            bool operator<=(const std::string rhs) const;
            bool operator<=(const double rhs) const;
            bool operator<=(const int rhs) const;
            bool operator<=(const float rhs) const;

            bool operator>=(const JSonValue& rhs) const;
            bool operator>=(const std::string rhs) const;
            bool operator>=(const double rhs) const;
            bool operator>=(const int rhs) const;
            bool operator>=(const float rhs) const;


            /**
            Create a JSonValue object containing an empty JSonObject.
            This method is a shorthand for the following code...
            JSonValue v(JSonObjectPtr(new JSonObject()));
            */
            static JSonValue Object();

            /**
            Create a JSonValue object containing an empty JSonArray.
            This method is a shorthand for the following code...
            JSonValue v(JSonArrayPtr(new JSonArray()));
            */
            static JSonValue Array();

            static JSonValue ParseFromString(const std::string &jsonstr);
            static JSonValue ParseFromFile(const std::string &src);

        protected:
            union JSonVar{
                std::string*    _string;
                double          _number;
                bool            _boolean;
            };

            JSonType        mType;
            JSonVar         mValue;
            JSonObjectPtr   mObject;
            JSonArrayPtr    mArray;

        private:
            std::string Serialize_str(const std::string& s);
            void SplitKey(const std::string key, std::string &head, std::string &tail);
            void ClearObjectsOrArrays();
    };

    template<> inline const JSonObjectPtr JSonValue::getPtr<JSonObjectPtr>() const{
        if (mType == JSonType_Object){return mObject;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline const JSonArrayPtr JSonValue::getPtr<JSonArrayPtr>() const{
        if (mType == JSonType_Array){return mArray;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline JSonObjectPtr JSonValue::getPtr<JSonObjectPtr>(){
        if (mType == JSonType_Object){return mObject;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline JSonArrayPtr JSonValue::getPtr<JSonArrayPtr>(){
        if (mType == JSonType_Array){return mArray;}
        throw std::runtime_error("Type Mismatch!");
    }




    template<> inline const std::string JSonValue::get<std::string>() const{
        if (mType == JSonType_String){return *mValue._string;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline const double JSonValue::get<double>() const{
        if (mType == JSonType_Number){return mValue._number;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline const int JSonValue::get<int>() const{
        if (mType == JSonType_Number){return static_cast<int>(mValue._number);}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline const float JSonValue::get<float>() const{
        if (mType == JSonType_Number){return static_cast<float>(mValue._number);}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline const bool JSonValue::get<bool>() const{
        if (mType == JSonType_Bool){return mValue._boolean;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline std::string JSonValue::get<std::string>(){
        if (mType == JSonType_String){return *mValue._string;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline double JSonValue::get<double>(){
        if (mType == JSonType_Number){return mValue._number;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline int JSonValue::get<int>(){
        if (mType == JSonType_Number){
            int num = static_cast<int>(mValue._number);
            return num;
        }
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline float JSonValue::get<float>(){
        if (mType == JSonType_Number){
            float num = static_cast<float>(mValue._number);
            return num;
        }
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline bool JSonValue::get<bool>(){
        if (mType == JSonType_Bool){return mValue._boolean;}
        throw std::runtime_error("Type Mismatch!");
    }


    template<> inline const JSonObjectIter JSonValue::begin() const{
        if (mType == JSonType_Object){return mObject->begin();}
        throw std::runtime_error("JSonValue is not an Object type.");
    }

    template<> inline JSonObjectIter JSonValue::begin(){
        if (mType == JSonType_Object){return mObject->begin();}
        throw std::runtime_error("JSonValue is not an Object type.");
    }

    template<> inline const JSonArrayIter JSonValue::begin() const{
        if (mType == JSonType_Array){return mArray->begin();}
        throw std::runtime_error("JSonValue is not an Array type.");
    }

    template<> inline JSonArrayIter JSonValue::begin(){
        if (mType == JSonType_Array){return mArray->begin();}
        throw std::runtime_error("JSonValue is not an Array type.");
    }

    template<> inline const JSonObjectIter JSonValue::end() const{
        if (mType == JSonType_Object){return mObject->end();}
        throw std::runtime_error("JSonValue is not an Object type.");
    }

    template<> inline JSonObjectIter JSonValue::end(){
        if (mType == JSonType_Object){return mObject->end();}
        throw std::runtime_error("JSonValue is not an Object type.");
    }

    template<> inline const JSonArrayIter JSonValue::end() const{
        if (mType == JSonType_Array){return mArray->end();}
        throw std::runtime_error("JSonValue is not an Array type.");
    }

    template<> inline JSonArrayIter JSonValue::end(){
        if (mType == JSonType_Array){return mArray->end();}
        throw std::runtime_error("JSonValue is not an Array type.");
    }

} /* End of json namespace*/ } /* End of engine namespace */
#endif // JSONVALUE_H
