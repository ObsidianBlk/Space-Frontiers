#ifndef JSONVALUE_H
#define JSONVALUE_H

#include <iostream>
#include <cstdio>
#include <string>
#include <map>
#include <vector>

namespace engine{ namespace json {

    class JSonValue;
    typedef std::pair<std::string, JSonValue> JSonObjectPair;
    typedef std::map<std::string, JSonValue> JSonObject;
    typedef std::map<std::string, JSonValue>::iterator JSonObjectIter;
    typedef std::vector<JSonValue> JSonArray;
    typedef std::vector<JSonValue>::iterator JSonArrayIter;

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
            explicit JSonValue(bool value);
            ~JSonValue();

            JSonType type();
            const JSonType type() const;

            template<typename T> bool is() const;
            template<typename T> const T& get() const;
            template<typename T> T& get();
            void set(const JSonValue& value);

            std::string to_str();
            std::string serialize();

            JSonValue& operator=(const JSonValue &rhs);
            JSonValue& operator=(const JSonObject &rhs);
            JSonValue& operator=(const JSonArray &rhs);
            JSonValue& operator=(const std::string &rhs);
            JSonValue& operator=(const char* rhs);
            JSonValue& operator=(double rhs);
            JSonValue& operator=(int rhs);
            JSonValue& operator=(float rhs);
            JSonValue& operator=(bool rhs);

            JSonValue& operator[](const std::string& index);
            JSonValue& operator[](const char* index);
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
            void ClearObjectsOrArrays();
    };

} /* End of json namespace*/ } /* End of engine namespace */
#endif // JSONVALUE_H
