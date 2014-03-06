#include "JSonValue.h"



namespace engine{ namespace json {


    JSonValue::JSonValue() : mType(JSonType_Null){}

    JSonValue::JSonValue(const JSonObject& value) : mType(JSonType_Object){
        mValue._object = new JSonObject(value);
    }

    JSonValue::JSonValue(const JSonArray& value) : mType(JSonType_Array){
        mValue._array = new JSonArray(value);
    }

    JSonValue::JSonValue(const std::string& value) : mType(JSonType_String){
        mValue._string = new std::string(value);
    }

    JSonValue::JSonValue(const char* value) : mType(JSonType_String){
        mValue._string = new std::string(value);
    }

    JSonValue::JSonValue(double value) : mType(JSonType_Number){
        mValue._number = value;
    }

    JSonValue::JSonValue(bool value) : mType(JSonType_Bool){
        mValue._boolean = value;
    }


    JSonValue::~JSonValue(){
        ClearObjectsOrArrays();
    }


        JSonType JSonValue::type(){return mType;}
    const JSonType JSonValue::type() const{return mType;}

    template<> inline bool JSonValue::is<JSonObject>() const{return mType == JSonType_Object;}
    template<> inline bool JSonValue::is<JSonArray>() const{return mType == JSonType_Object;}
    template<> inline bool JSonValue::is<std::string>() const{return mType == JSonType_String;}
    template<> inline bool JSonValue::is<double>() const{return mType == JSonType_Number;}
    template<> inline bool JSonValue::is<bool>() const{return mType == JSonType_Bool;}

    template<> inline const JSonObject& JSonValue::get() const{
        if (is<JSonObject>()){return *mValue._object;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline const JSonArray& JSonValue::get() const{
        if (is<JSonArray>()){return *mValue._array;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline const std::string& JSonValue::get() const{
        if (is<std::string>()){return *mValue._string;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline const double& JSonValue::get() const{
        if (is<double>()){return mValue._number;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline const bool& JSonValue::get() const{
        if (is<bool>()){return mValue._boolean;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline JSonObject& JSonValue::get(){
        if (is<JSonObject>()){return *mValue._object;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline JSonArray& JSonValue::get(){
        if (is<JSonArray>()){return *mValue._array;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline std::string& JSonValue::get(){
        if (is<std::string>()){return *mValue._string;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline double& JSonValue::get(){
        if (is<double>()){return mValue._number;}
        throw std::runtime_error("Type Mismatch!");
    }

    template<> inline bool& JSonValue::get(){
        if (is<bool>()){return mValue._boolean;}
        throw std::runtime_error("Type Mismatch!");
    }

    void JSonValue::set(const JSonValue& value){
        switch(value.type()){
        case JSonType_Object:
            if (mValue._object != value.mValue._object){
                ClearObjectsOrArrays();
                mValue._object = new JSonObject(value.get<JSonObject>());
            }
            break;
        case JSonType_Array:
            if (mValue._array != value.mValue._array){
                ClearObjectsOrArrays();
                mValue._array = new JSonArray(value.get<JSonArray>());
            }
            break;
        case JSonType_String:
            if (mValue._string != value.mValue._string){
                ClearObjectsOrArrays();
                mValue._string = new std::string(value.get<std::string>());
            }
            break;
        case JSonType_Number:
            mValue._number = value.get<double>();
            break;
        case JSonType_Bool:
            mValue._boolean = value.get<bool>();
            break;
        default:
            break;
        }

        mType = value.mType;
    }


    std::string JSonValue::to_str(){
        switch(mType){
        case JSonType_Null:
            return "null";
        case JSonType_Object:
            return "Object";
        case JSonType_Array:
            return "Array";
        case JSonType_String:
            return *mValue._string;
        case JSonType_Number:
            return serialize(); // Serialize returns the number as a string already.
        case JSonType_Bool:
            return mValue._boolean ? "true" : "false";
        }

        return std::string();
    }

    std::string JSonValue::serialize(){
        std::string serial;
        bool past_first_element = false;
        switch(mType){
        case JSonType_Null:
        case JSonType_Bool:
            serial = to_str();
            break;
        case JSonType_Number:
            {
                char buf[256];
                snprintf(buf, sizeof(buf), "%g", mValue._number);
                serial = buf;
            }
            break;
        case JSonType_Object:
            serial = "{";
            for (JSonObjectIter i = mValue._object->begin(); i != mValue._object->end(); i++){
                if (past_first_element){serial += ",";}
                serial += i->first + std::string(":") + i->second.serialize();
                past_first_element = true;
            }
            serial += "}";
            break;
        case JSonType_Array:
            serial = "[";
            for (JSonArrayIter i = mValue._array->begin(); i != mValue._array->end(); i++){
                if (past_first_element){serial += ",";}
                serial += i->serialize();
                past_first_element = true;
            }
            serial += "]";
            break;
        case JSonType_String:
            serial = Serialize_str(*mValue._string);
        }

        return serial;
    }


    JSonValue& JSonValue::operator=(const JSonValue &rhs){
        set(rhs);
        return (*this);
    }

    JSonValue& JSonValue::operator=(const JSonObject &rhs){
        if (mType != JSonType_Object || mValue._object != &rhs){
            ClearObjectsOrArrays();
            mValue._object = new JSonObject(rhs);
            mType = JSonType_Object;
        }
        return (*this);
    }

    JSonValue& JSonValue::operator=(const JSonArray &rhs){
        if (mType != JSonType_Array || mValue._array != &rhs){
            ClearObjectsOrArrays();
            mValue._array = new JSonArray(rhs);
            mType = JSonType_Array;
        }
        return (*this);
    }

    JSonValue& JSonValue::operator=(const std::string &rhs){
        if (mType != JSonType_String || mValue._string != &rhs){
            ClearObjectsOrArrays();
            mValue._string = new std::string(rhs);
            mType = JSonType_String;
        }
        return (*this);
    }

    JSonValue& JSonValue::operator=(const char* rhs){
        ClearObjectsOrArrays();
        mValue._string = new std::string(rhs);
        mType = JSonType_String;
        return (*this);
    }

    JSonValue& JSonValue::operator=(double rhs){
        ClearObjectsOrArrays();
        mValue._number = rhs;
        mType = JSonType_Number;
        return (*this);
    }

    JSonValue& JSonValue::operator=(int rhs){
        return operator=(static_cast<double>(rhs));
    }

    JSonValue& JSonValue::operator=(float rhs){
        return operator=(static_cast<double>(rhs));
    }

    JSonValue& JSonValue::operator=(bool rhs){
        ClearObjectsOrArrays();
        mValue._boolean = rhs;
        mType = JSonType_Bool;
        return (*this);
    }


    JSonValue& JSonValue::operator[](const std::string& index){
        if (mType == JSonType_Object){
            JSonObjectIter i = mValue._object->find(index);
            if (i != mValue._object->end())
                return i->second;
            throw std::out_of_range(std::string("Index \"") + index + std::string("\" not found in JSonObject."));
        }
        throw std::runtime_error("JSonValue is not a JSonType_Object.");
    }

    JSonValue& JSonValue::operator[](const char* index){
        try{
            return operator[](std::string(index));
        } catch (std::out_of_range e){
            throw e;
        } catch (std::runtime_error e){
            throw e;
        }
    }

    JSonValue& JSonValue::operator[](const int index){
        if (mType == JSonType_Array){
            if (index >= 0 && index < mValue._array->size()){
                return mValue._array->at(index);
            }
            throw std::out_of_range("Index exceeds size of JSonArray.");
        }
        throw std::runtime_error("JSonValue is not a JSonType_Array.");
    }






    std::string JSonValue::Serialize_str(const std::string& s){
        std::string serial = "\"";
        for (std::string::const_iterator i = s.begin(); i != s.end(); i++){
            switch (*i){
            case '"':
                serial += "\\\""; break;
            case '\\':
                serial += "\\\\"; break;
            case '/':
                serial += "\\/"; break;
            case '\b':
                serial += "\\b"; break;
            case '\f':
                serial += "\\f"; break;
            case '\n':
                serial += "\\n"; break;
            case '\r':
                serial += "\\r"; break;
            case '\t':
                serial += "\\t"; break;
            default:
                serial += *i;
            }
        }
        serial += '"';
        return serial;
    }


    void JSonValue::ClearObjectsOrArrays(){
        switch (mType){
        case JSonType_Object:
            delete mValue._object;
            break;
        case JSonType_Array:
            delete mValue._array;
            break;
        case JSonType_String:
            delete mValue._string;
        default:
            break;
        }
    }


    /*JSonValue &JSonValue::operator[](const int index);
    JSonValue &JSonValue::operator[](const std::string key);
    JSonValue &JSonValue::operator[](const char* key);*/

} /* End of json namespace*/ } /* End of engine namespace */
