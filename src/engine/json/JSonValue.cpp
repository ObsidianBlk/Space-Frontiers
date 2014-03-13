#include "JSonValue.h"



namespace engine{ namespace json {

    std::string JSonValue::Key_Separator=".";

    JSonValue::JSonValue() : mType(JSonType_Null){}

    JSonValue::JSonValue(JSonObjectPtr value) : mType(JSonType_Object){
        mObject = value;
    }

    JSonValue::JSonValue(JSonArrayPtr value) : mType(JSonType_Array){
        mArray = value;
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


    bool JSonValue::is(JSonType t){
        return mType == t;
    }

    JSonType JSonValue::type(){return mType;}
    const JSonType JSonValue::type() const{return mType;}

    std::string JSonValue::type_str(){
        switch(mType){
        case JSonType_Object:
            return std::string("JSonType_Object");
        case JSonType_Array:
            return std::string("JSonType_Array");
        case JSonType_String:
            return std::string("JSonType_String");
        case JSonType_Number:
            return std::string("JSonType_Number");
        case JSonType_Bool:
            return std::string("JSonType_Bool");
        default: break;
        }
        return std::string("JSonType_Null");
    }

    const std::string JSonValue::type_str() const{
        switch(mType){
        case JSonType_Object:
            return std::string("JSonType_Object");
        case JSonType_Array:
            return std::string("JSonType_Array");
        case JSonType_String:
            return std::string("JSonType_String");
        case JSonType_Number:
            return std::string("JSonType_Number");
        case JSonType_Bool:
            return std::string("JSonType_Bool");
        default: break;
        }
        return std::string("JSonType_Null");
    }

    void JSonValue::set(const JSonValue& value){
        switch(value.type()){
        case JSonType_Object:
            if (mObject != value.mObject){
                ClearObjectsOrArrays();
                mObject = value.mObject;
            }
            break;
        case JSonType_Array:
            if (mArray != value.mArray){
                ClearObjectsOrArrays();
                mArray = value.mArray;
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
            for (JSonObjectIter i = mObject->begin(); i != mObject->end(); i++){
                if (past_first_element){serial += ",";}
                serial += std::string("\"") + i->first + std::string("\":") + i->second.serialize();
                past_first_element = true;
            }
            serial += "}";
            break;
        case JSonType_Array:
            serial = "[";
            for (JSonArrayIter i = mArray->begin(); i != mArray->end(); i++){
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

    std::string JSonValue::pretty_serial(std::string indentSym, size_t depth){
        std::string serial;
        bool past_first_element = false;

        auto indent = [=](size_t dpth){
            std::string i = "";
            for (size_t d = 0; d < dpth; d++)
                i += indentSym;
            return i;
        };

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
            serial = "{\n";
            for (JSonObjectIter i = mObject->begin(); i != mObject->end(); i++){
                if (past_first_element){serial += ",\n";}
                std::stringstream ss;
                ss << indent(depth+1) << "\"" << i->first << "\" : " << i->second.pretty_serial(indentSym, depth + 1);
                serial += ss.str();
                past_first_element = true;
            }
            serial += "\n";
            serial += indent(depth) + "}";
            break;
        case JSonType_Array:
            serial = "[\n";
            for (JSonArrayIter i = mArray->begin(); i != mArray->end(); i++){
                if (past_first_element){serial += ",\n";}
                serial += indent(depth+1) + (*i)->pretty_serial(indentSym, depth + 1);
                past_first_element = true;
            }
            serial += "\n";
            serial += indent(depth) + "]";
            break;
        case JSonType_String:
            serial = Serialize_str(*mValue._string);
        }

        return serial;
    }


    JSonValue& JSonValue::getKey(const std::string key, bool createMissingKey){
        if (mType == JSonType_Object || mType == JSonType_Array){
            std::string head = "";
            std::string tail = "";

            auto _stoui = [](std::string s){
                // NOTE: This lambda should NOT be needed. std::stoi() should do the job, but it seems
                // to be missing in MinGW. Until I come up with a more elegant way to deal with that issue,
                // this lambda will exist.
                std::istringstream ss(s);
                size_t res;
                return ss >> res ? res : throw std::invalid_argument("String is not a number.");
            };
            if (JSonValue::Key_Separator != "")
                SplitKey(key, head, tail);
            else head = key;

            if (mType == JSonType_Object){
                JSonObjectIter i = mObject->find(head);
                if (i != mObject->end()){
                    if (tail != ""){
                        try{
                            return i->second.getKey(tail, createMissingKey);
                        } catch (std::out_of_range e){
                            throw e;
                        } catch (std::runtime_error e){
                            throw e;
                        }
                    }
                    return (*mObject)[head];
                } else if (createMissingKey){
                    auto _headkeyindex = [this](std::string &k){
                        std::string h = "";
                        std::string t = "";
                        SplitKey(k, h, t);
                        if (h[0] == '#')
                            return true;
                        k = t;
                        return false;
                    };
                    auto _keyhasindex = [_headkeyindex](std::string k){
                        //std::string t = "x"; // We need something to start.
                        while (k != ""){
                            if (_headkeyindex(k))
                                return true;
                        }
                        return false;
                    };

                    if (_keyhasindex(key))
                        throw std::runtime_error(std::string("Key segment \"") + key + std::string("\" contains array access. Arrays must be handled manually."));

                    (*mObject)[head] = JSonObjectPtr(new JSonObject());
                    if (tail != "")
                        return (*mObject)[head].getKey(tail, createMissingKey); // In theory, I shouldn't have to worry about any errors beyond this point.
                    return (*mObject)[head];
                }
                throw std::out_of_range(std::string("Unable to locate resource from key segment \"") + key + std::string("\"."));

            } else if (mType == JSonType_Array){
                if (head[0] == '#'){
                    size_t index = 0;
                    try {index = _stoui(head.substr(1, head.length()));} catch (std::invalid_argument e){
                        throw std::invalid_argument("Key segment expected to be an array index.");
                    }

                    if (index < mArray->size()){
                        if (tail != ""){
                            return mArray->at(index)->getKey(tail, createMissingKey);
                        }
                        return *(mArray->at(index));
                    }
                    std::out_of_range("Key to index value out of range of the JSonArray object.");
                } else if (head == "+" && tail == ""){ // A special case for growing an array without having to dig into the DOM for it.
                    mArray->push_back(JSonValuePtr(new JSonValue()));
                    return *(mArray->at(mArray->size()-1));
                }
                std::runtime_error(std::string("Key segment \"") + key + std::string("\" does not begin with array access key format."));
            }
        }
        throw std::runtime_error("JSonValue must be a JSonType_Object or JSonType_Array.");
    }

    JSonValue& JSonValue::getKey(const char* key, bool createMissingKey){
        try{
            return getKey(std::string(key, createMissingKey));
        } catch (std::out_of_range e){
            throw e;
        } catch (std::runtime_error e){
            throw e;
        }
    }

    bool JSonValue::hasKey(const std::string key){
        try{
            // If it doesn't throw an exception, the key exists!
            getKey(key, false);
        } catch (std::out_of_range e){
            return false;
        } catch (std::invalid_argument e){
            return false;
        } catch (std::runtime_error e){
            return false;
        }
        return true;
    }


    bool JSonValue::hasKey(const char* key){
        return hasKey(std::string(key));
    }


    size_t JSonValue::size(){
        switch (mType){
        case JSonType_Object:
            return mObject->size();
        case JSonType_Array:
            return mArray->size();
        case JSonType_String:
            return mValue._string->size();
        case JSonType_Number:
            return sizeof(mValue._number);
        case JSonType_Bool:
            return sizeof(mValue._boolean);
        default:
            break;
        }
        return 0;
    }

    size_t JSonValue::size() const{
        switch (mType){
        case JSonType_Object:
            return mObject->size();
        case JSonType_Array:
            return mArray->size();
        case JSonType_String:
            return mValue._string->size();
        case JSonType_Number:
            return sizeof(mValue._number);
        case JSonType_Bool:
            return sizeof(mValue._boolean);
        default:
            break;
        }
        return 0;
    }

    bool JSonValue::empty(){
        switch (mType){
        case JSonType_Array:
            return mArray->empty();
        case JSonType_Object:
            return mObject->empty();
        default: break;
        }
        return false;
    }



    JSonValue& JSonValue::operator[](const std::string& key){
        if (mType == JSonType_Object || mType == JSonType_Array){
            try {
                return getKey(key, mType == JSonType_Object ? true : false);
            } catch (std::out_of_range e){
                throw e;
            } catch (std::runtime_error e){
                throw e;
            }
        }
        throw std::runtime_error("JSonValue must be a JSonType_Object or JSonType_Array.");
    }

    JSonValue& JSonValue::operator[](const char* key){
        if (mType == JSonType_Object || mType == JSonType_Array){
            try{
                return getKey(std::string(key), mType == JSonType_Object ? true : false);
            } catch (std::out_of_range e){
                throw e;
            } catch (std::runtime_error e){
                throw e;
            }
        }
        throw std::runtime_error("JSonValue must be a JSonType_Object or JSonType_Array.");
    }

    JSonValue& JSonValue::operator[](const size_t index){
        if (mType == JSonType_Array){
            if (index >= 0 && index < mArray->size()){
                return *(mArray->at(index));
            }
            throw std::out_of_range("Index exceeds size of JSonArray.");
        }
        throw std::runtime_error("JSonValue is not a JSonType_Array.");
    }


    JSonValue& JSonValue::operator=(const JSonValue &rhs){
        set(rhs);
        return (*this);
    }

    JSonValue& JSonValue::operator=(JSonObjectPtr rhs){
        if (mType != JSonType_Object || mObject != rhs){
            ClearObjectsOrArrays();
            mObject = rhs;
            mType = JSonType_Object;
        }
        return (*this);
    }

    JSonValue& JSonValue::operator=(JSonArrayPtr rhs){
        if (mType != JSonType_Array || mArray != rhs){
            ClearObjectsOrArrays();
            mArray = rhs;
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


    bool JSonValue::operator==(const JSonValue& rhs) const{
        if (mType == rhs.mType){
            switch(mType){
            case JSonType_Object:
                return mObject == rhs.mObject;
            case JSonType_Array:
                return mArray == rhs.mArray;
            case JSonType_String:
                return *(mValue._string) == *(rhs.mValue._string);
            case JSonType_Bool:
                return mValue._boolean == rhs.mValue._boolean;
            case JSonType_Number:
                return mValue._number == rhs.mValue._number;
            default:
                break;
            }
        }
        return false;
    }

    bool JSonValue::operator==(const JSonObjectPtr rhs) const{
        if (mType == JSonType_Object){return mObject == rhs;}
        return false;
    }

    bool JSonValue::operator==(const JSonArrayPtr rhs) const{
        if (mType == JSonType_Array){return mArray == rhs;}
        return false;
    }

    bool JSonValue::operator==(const std::string rhs) const{
        if (mType == JSonType_String){return *(mValue._string) == rhs;}
        return false;
    }

    bool JSonValue::operator==(const double rhs) const{
        if (mType == JSonType_Number){return mValue._number == rhs;}
        return false;
    }

    bool JSonValue::operator==(const int rhs) const{
        if (mType == JSonType_Number){return mValue._number == static_cast<double>(rhs);}
        return false;
    }

    bool JSonValue::operator==(const float rhs) const{
        if (mType == JSonType_Number){return mValue._number == static_cast<double>(rhs);}
        return false;
    }

    bool JSonValue::operator==(const bool rhs) const{
        if (mType == JSonType_Bool){return mValue._boolean == rhs;}
        return false;
    }


    bool JSonValue::operator!=(const JSonValue& rhs) const{return !operator==(rhs);}
    bool JSonValue::operator!=(const JSonObjectPtr rhs) const{return !operator==(rhs);}
    bool JSonValue::operator!=(const JSonArrayPtr rhs) const{return !operator==(rhs);}
    bool JSonValue::operator!=(const std::string rhs) const{return !operator==(rhs);}
    bool JSonValue::operator!=(const double rhs) const{return !operator==(rhs);}
    bool JSonValue::operator!=(const int rhs) const{return !operator==(rhs);}
    bool JSonValue::operator!=(const float rhs) const{return !operator==(rhs);}
    bool JSonValue::operator!=(const bool rhs) const{return !operator==(rhs);}

    bool JSonValue::operator<(const JSonValue& rhs) const{
        if (mType == rhs.mType){
            switch(mType){
            case JSonType_Number:
                return mValue._number < rhs.mValue._number;
            case JSonType_String:
                return mValue._string->size() < rhs.mValue._string->size();
            default: break;
            }
        }
        throw std::invalid_argument(type_str() + " and " + rhs.type_str() + " types cannot be compared in this manner.");
    }

    bool JSonValue::operator<(const std::string rhs) const{
        if (mType == JSonType_String){return mValue._string->size() < rhs.size();}
        throw std::invalid_argument(type_str() + " and String types cannot be compared in this manner.");
    }

    bool JSonValue::operator<(const double rhs) const{
        if (mType == JSonType_Number){return mValue._number < rhs;}
        throw std::invalid_argument(type_str() + " and double types cannot be compared in this manner.");
    }

    bool JSonValue::operator<(const int rhs) const{
        if (mType == JSonType_Number){return mValue._number < static_cast<double>(rhs);}
        throw std::invalid_argument(type_str() + " and int types cannot be compared in this manner.");
    }

    bool JSonValue::operator<(const float rhs) const{
        if (mType == JSonType_Number){return mValue._number < static_cast<double>(rhs);}
        throw std::invalid_argument(type_str() + " and float types cannot be compared in this manner.");
    }

    bool JSonValue::operator>(const JSonValue& rhs) const{
        try{return (!operator<(rhs) && !operator==(rhs));}catch(std::invalid_argument e){throw e;}
    }

    bool JSonValue::operator>(const std::string rhs) const{
        try{return (!operator<(rhs) && !operator==(rhs));}catch(std::invalid_argument e){throw e;}
    }

    bool JSonValue::operator>(const double rhs) const{
        try{return (!operator<(rhs) && !operator==(rhs));}catch(std::invalid_argument e){throw e;}
    }

    bool JSonValue::operator>(const int rhs) const{
        try{return (!operator<(rhs) && !operator==(rhs));}catch(std::invalid_argument e){throw e;}
    }

    bool JSonValue::operator>(const float rhs) const{
        try{return (!operator<(rhs) && !operator==(rhs));}catch(std::invalid_argument e){throw e;}
    }

    bool JSonValue::operator<=(const JSonValue& rhs) const{
        try{return (operator<(rhs) || operator==(rhs));}catch(std::invalid_argument e){throw e;}
    }

    bool JSonValue::operator<=(const std::string rhs) const{
        try{return (operator<(rhs) || operator==(rhs));}catch(std::invalid_argument e){throw e;}
    }

    bool JSonValue::operator<=(const double rhs) const{
        try{return (operator<(rhs) || operator==(rhs));}catch(std::invalid_argument e){throw e;}
    }

    bool JSonValue::operator<=(const int rhs) const{
        try{return (operator<(rhs) || operator==(rhs));}catch(std::invalid_argument e){throw e;}
    }

    bool JSonValue::operator<=(const float rhs) const{
        try{return (operator<(rhs) || operator==(rhs));}catch(std::invalid_argument e){throw e;}
    }

    bool JSonValue::operator>=(const JSonValue& rhs) const{
        try{return (!operator<(rhs) || operator==(rhs));}catch(std::invalid_argument e){throw e;}
    }

    bool JSonValue::operator>=(const std::string rhs) const{
        try{return (!operator<(rhs) || operator==(rhs));}catch(std::invalid_argument e){throw e;}
    }

    bool JSonValue::operator>=(const double rhs) const{
        try{return (!operator<(rhs) || operator==(rhs));}catch(std::invalid_argument e){throw e;}
    }

    bool JSonValue::operator>=(const int rhs) const{
        try{return (!operator<(rhs) || operator==(rhs));}catch(std::invalid_argument e){throw e;}
    }

    bool JSonValue::operator>=(const float rhs) const{
        try{return (!operator<(rhs) || operator==(rhs));}catch(std::invalid_argument e){throw e;}
    }


/* ------------------------------------------------------------------------------------------------------
STATIC CLASS METHODS BELOW THIS POINT
------------------------------------------------------------------------------------------------------ */
    JSonValue JSonValue::Object(){
        JSonValue v;
        v = JSonObjectPtr(new JSonObject());
        return v;
    }

    JSonValue JSonValue::Array(){
        JSonValue v;
        v = JSonArrayPtr(new JSonArray());
        return v;
    }

/* ------------------------------------------------------------------------------------------------------
PRIVATE METHODS BELOW THIS POINT
------------------------------------------------------------------------------------------------------ */

    std::string JSonValue::Serialize_str(const std::string& s){
        std::string serial = "";
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
        serial.insert(0, "\"");
        serial.insert(serial.size(), "\"");
        return serial;
    }

    void JSonValue::SplitKey(const std::string key, std::string &head, std::string &tail){
        //std::string delimiter(":");
        size_t pos = key.find(JSonValue::Key_Separator);
        if (pos != std::string::npos){
            head = key.substr(0, pos);
            tail = key.substr(pos+JSonValue::Key_Separator.length(), key.length()-JSonValue::Key_Separator.length());
        } else {
            head = key;
            tail = "";
        }
    }

    void JSonValue::ClearObjectsOrArrays(){
        switch (mType){
        case JSonType_String:
            delete mValue._string;
        default:
            break;
        }
    }

} /* End of json namespace*/ } /* End of engine namespace */
