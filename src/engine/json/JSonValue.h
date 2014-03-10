#ifndef JSONVALUE_H
#define JSONVALUE_H

#include <memory>
#include <iostream>
#include <sstream>
#include <cstdio>
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

    class JSonValue
    {
        public:
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

            JSonType type();
            const JSonType type() const;
            std::string type_str();
            const std::string type_str() const;

            template<typename T> bool is() const;
            template<typename T> const T getPtr() const;
            template<typename T> T getPtr();
            template<typename T> const T& get() const;
            template<typename T> T& get();
            void set(const JSonValue& value);

            std::string to_str();
            std::string serialize();

            bool hasKey(const std::string key);
            bool hasKey(const char* key);

            JSonValue& getKey(const std::string key, bool createMissingKey=false);
            JSonValue& getKey(const char* key, bool createMissingKey=false);

            size_t size();
            size_t size() const;

            JSonValue& operator=(const JSonValue &rhs);
            JSonValue& operator=(JSonObjectPtr rhs);
            JSonValue& operator=(JSonArrayPtr rhs);
            JSonValue& operator=(const std::string &rhs);
            JSonValue& operator=(const char* rhs);
            JSonValue& operator=(double rhs);
            JSonValue& operator=(int rhs);
            JSonValue& operator=(float rhs);
            JSonValue& operator=(bool rhs);

            JSonValue& operator[](const std::string& key);
            JSonValue& operator[](const char* key);
            JSonValue& operator[](const int index);

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
            int StrToNum(std::string text, int defres=0);
            void SplitKey(const std::string key, std::string &head, std::string &tail);
            bool KeyContainsArrayAccess(const std::string key);
            void ClearObjectsOrArrays();
    };


    template<> inline bool JSonValue::is<JSonObject>() const{return mType == JSonType_Object;}
    template<> inline bool JSonValue::is<JSonArray>() const{return mType == JSonType_Array;}
    template<> inline bool JSonValue::is<std::string>() const{return mType == JSonType_String;}
    template<> inline bool JSonValue::is<double>() const{return mType == JSonType_Number;}
    template<> inline bool JSonValue::is<bool>() const{return mType == JSonType_Bool;}


    template<> inline const JSonObjectPtr JSonValue::getPtr<JSonObjectPtr>() const{
        if (is<JSonObject>()){return mObject;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline const JSonArrayPtr JSonValue::getPtr<JSonArrayPtr>() const{
        if (is<JSonArray>()){return mArray;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline JSonObjectPtr JSonValue::getPtr<JSonObjectPtr>(){
        if (is<JSonObject>()){return mObject;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline JSonArrayPtr JSonValue::getPtr<JSonArrayPtr>(){
        if (is<JSonArray>()){return mArray;}
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
