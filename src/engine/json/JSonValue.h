#ifndef JSONVALUE_H
#define JSONVALUE_H

#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <map>
#include <vector>

namespace engine{ namespace json {

    class JSonValue;
    typedef std::pair<std::string, JSonValue> JSonObjectPair;
    typedef std::map<std::string, JSonValue> JSonObject;
    typedef std::map<std::string, JSonValue>::iterator JSonObjectIter;
    typedef std::vector<JSonValue*> JSonArray;
    typedef std::vector<JSonValue*>::iterator JSonArrayIter;

    enum JSonType {JSonType_Object, JSonType_Array, JSonType_Number, JSonType_String, JSonType_Bool, JSonType_Null};

    class JSonValue
    {
        public:
            JSonValue();
            explicit JSonValue(const JSonObject& value);
            explicit JSonValue(const JSonArray& value);
            explicit JSonValue(const std::string& value);
            explicit JSonValue(const char* value);
            explicit JSonValue(double value);
            explicit JSonValue(int value);
            explicit JSonValue(float value);
            explicit JSonValue(bool value);
            ~JSonValue();

            JSonType type();
            const JSonType type() const;

            template<typename T> bool is() const;
            template<typename T> const T* getPtr() const;
            template<typename T> T* getPtr();
            template<typename T> const T& get() const;
            template<typename T> T& get();
            void set(const JSonValue& value);

            std::string to_str();
            std::string serialize();

            bool hasKey(const std::string key);
            bool hasKey(const char* key);

            JSonValue& operator=(const JSonValue &rhs);
            JSonValue& operator=(const JSonObject &rhs);
            JSonValue& operator=(const JSonArray &rhs);
            JSonValue& operator=(const std::string &rhs);
            JSonValue& operator=(const char* rhs);
            JSonValue& operator=(double rhs);
            JSonValue& operator=(int rhs);
            JSonValue& operator=(float rhs);
            JSonValue& operator=(bool rhs);

            JSonValue& operator[](const std::string& key);
            JSonValue& operator[](const char* key);
            JSonValue& operator[](const int index);

        protected:
            union JSonVar{
                JSonObject*     _object;
                JSonArray*      _array;
                std::string*    _string;
                double          _number;
                bool            _boolean;
            };

            JSonType mType;
            JSonVar mValue;

        private:
            std::string Serialize_str(const std::string& s);
            int StrToNum(std::string text, int defres=0);
            void SplitKey(const std::string key, std::string &head, std::string &tail);
            void ClearObjectsOrArrays();
    };


    template<> inline bool JSonValue::is<JSonObject>() const{return mType == JSonType_Object;}
    template<> inline bool JSonValue::is<JSonArray>() const{return mType == JSonType_Array;}
    template<> inline bool JSonValue::is<std::string>() const{return mType == JSonType_String;}
    template<> inline bool JSonValue::is<double>() const{return mType == JSonType_Number;}
    template<> inline bool JSonValue::is<bool>() const{return mType == JSonType_Bool;}


    template<> inline const JSonObject* JSonValue::getPtr<JSonObject>() const{
        if (is<JSonObject>()){return mValue._object;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline const JSonArray* JSonValue::getPtr<JSonArray>() const{
        if (is<JSonArray>()){return mValue._array;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline JSonObject* JSonValue::getPtr<JSonObject>(){
        if (is<JSonObject>()){return mValue._object;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline JSonArray* JSonValue::getPtr<JSonArray>(){
        if (is<JSonArray>()){return mValue._array;}
        throw std::runtime_error("Type Mismatch!");
    }




    template<> inline const std::string& JSonValue::get<std::string>() const{
        if (is<std::string>()){return *mValue._string;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline const double& JSonValue::get<double>() const{
        if (is<double>()){return mValue._number;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline const bool& JSonValue::get<bool>() const{
        if (is<bool>()){return mValue._boolean;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline std::string& JSonValue::get<std::string>(){
        if (is<std::string>()){return *mValue._string;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline double& JSonValue::get<double>(){
        if (is<double>()){return mValue._number;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline bool& JSonValue::get<bool>(){
        if (is<bool>()){return mValue._boolean;}
        throw std::runtime_error("Type Mismatch!");
    }

} /* End of json namespace*/ } /* End of engine namespace */
#endif // JSONVALUE_H
