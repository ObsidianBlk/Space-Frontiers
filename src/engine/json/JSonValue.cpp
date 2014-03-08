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

    JSonValue::JSonValue(int value) : mType(JSonType_Number){
        mValue._number = static_cast<double>(value);
    }

    JSonValue::JSonValue(float value) : mType(JSonType_Number){
        mValue._number = static_cast<double>(value);
    }

    JSonValue::JSonValue(bool value) : mType(JSonType_Bool){
        mValue._boolean = value;
    }


    JSonValue::~JSonValue(){
        ClearObjectsOrArrays();
    }


        JSonType JSonValue::type(){return mType;}
    const JSonType JSonValue::type() const{return mType;}

    void JSonValue::set(const JSonValue& value){
        switch(value.type()){
        case JSonType_Object:
            if (mValue._object != value.mValue._object){
                ClearObjectsOrArrays();
                mValue._object = new JSonObject(*(value.mValue._object));
            }
            break;
        case JSonType_Array:
            if (mValue._array != value.mValue._array){
                ClearObjectsOrArrays();
                mValue._array = new JSonArray(*(value.mValue._array));
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
                serial += (*i)->serialize();
                past_first_element = true;
            }
            serial += "]";
            break;
        case JSonType_String:
            serial = Serialize_str(*mValue._string);
        }

        return serial;
    }


    bool JSonValue::hasKey(const std::string key){
        if (mType == JSonType_Object || mType == JSonType_Array){
            std::string head = "";
            std::string tail = "";
            SplitKey(key, head, tail);

            if (mType == JSonType_Object){
                JSonObjectIter i = mValue._object->find(head);
                if (i != mValue._object->end()){
                    if (tail != ""){
                        return i->second.hasKey(tail);
                    }
                    return true;
                }
            } else if (mType == JSonType_Array){
                if (head[0] == '#'){
                    int index = StrToNum(head.substr(1, head.length()), -1);
                    if (index >= 0 && index < mValue._array->size()){
                        if (tail != ""){
                            return mValue._array->at(index)->hasKey(tail);
                        }
                        return true;
                    }
                }
            }
        }

        return false;
    }

    int JSonValue::StrToNum(std::string text, int defres){
        // NOTE: This method should NOT be needed. std::stoi() should do the job, but it seems
        // to be missing in MinGW. Until I come up with a more elegant way to deal with that issue,
        // this method will exist.
        std::istringstream ss(text);
        int res;
        return ss >> res ? res : defres;
    }

    void JSonValue::SplitKey(const std::string key, std::string &head, std::string &tail){
        std::string delimiter(":");
        size_t pos = key.find(delimiter);
        if (pos != std::string::npos){
            head = key.substr(0, pos);
            tail = key.substr(pos+delimiter.length(), key.length()-delimiter.length());
        } else {
            head = key;
            tail = "";
        }
    }

    bool JSonValue::hasKey(const char* key){
        return hasKey(std::string(key));
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


    JSonValue& JSonValue::operator[](const std::string& key){
        if (mType == JSonType_Object){
            return (*mValue._object)[key];
            /*JSonObjectIter i = mValue._object->find(index);
            if (i != mValue._object->end())
                return i->second;
            throw std::out_of_range(std::string("Index \"") + index + std::string("\" not found in JSonObject."));*/
        }
        throw std::runtime_error("JSonValue is not a JSonType_Object.");
    }

    JSonValue& JSonValue::operator[](const char* key){
        try{
            return operator[](std::string(key));
        } catch (std::out_of_range e){
            throw e;
        } catch (std::runtime_error e){
            throw e;
        }
    }

    JSonValue& JSonValue::operator[](const int index){
        if (mType == JSonType_Array){
            if (index >= 0 && index < mValue._array->size()){
                return *(mValue._array->at(index));
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
            for (int i = 0; i < mValue._array->size(); i++){
                delete mValue._array->at(i);
            }
            delete mValue._array;
            break;
        case JSonType_String:
            delete mValue._string;
        default:
            break;
        }
    }

} /* End of json namespace*/ } /* End of engine namespace */
