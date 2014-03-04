#include "JSonValue.h"



namespace engine{ namespace json {


    JSonValue::JSonValue(){
        mType = JSonType_Null;
    }

    JSonValue::JSonValue(JSonObject& value){
        try{
            set(value);
        } catch (std::runtime_error e){
            throw e;
        }
    }

    JSonValue::JSonValue(JSonArray& value){
        try{
            set(value);
        } catch (std::runtime_error e){
            throw e;
        }
    }

    JSonValue::JSonValue(int value){
        try{
            set(value);
        } catch (std::runtime_error e){
            throw e;
        }
    }

    JSonValue::JSonValue(float value){
        try{
            set(value);
        } catch (std::runtime_error e){
            throw e;
        }
    }

    JSonValue::JSonValue(double value){
        try{
            set(value);
        } catch (std::runtime_error e){
            throw e;
        }
    }

    JSonValue::JSonValue(std::string value){
        try{
            set(value);
        } catch (std::runtime_error e){
            throw e;
        }
    }

    JSonValue::JSonValue(bool value){
        try{
            set(value);
        } catch (std::runtime_error e){
            throw e;
        }
    }


    JSonValue::~JSonValue(){
        ClearObjectsOrArrays();
    }


    JSonType JSonValue::type(){
        return mType;
    }


    void JSonValue::set(JSonObject& value){
        if (mType == JSonType_Object){
            JSonObject obj = boost::get<JSonObject>(mValue);
            if (&obj == &value){return;} // Attempting to point this JSonValue to the object it already contains.
        }
        ClearObjectsOrArrays();
        mValue = value;
        mType = JSonType_Object;
    }


    void JSonValue::set(JSonArray& value){
        if (mType == JSonType_Array){
            JSonArray arr = boost::get<JSonArray>(mValue);
            if (&arr == &value){return;} // Attempting to point this JSonValue to the object it already contains.
        }
        ClearObjectsOrArrays();
        mValue = value;
        mType = JSonType_Array;
    }

    void JSonValue::set(int value){
        ClearObjectsOrArrays();
        mValue = value;
        mType = JSonType_Number;
    }

    void JSonValue::set(float value){
        ClearObjectsOrArrays();
        mValue = value;
        mType = JSonType_Number;
    }

    void JSonValue::set(double value){
        ClearObjectsOrArrays();
        mValue = value;
        mType = JSonType_Number;
    }

    void JSonValue::set(std::string value){
        ClearObjectsOrArrays();
        mValue = value;
        mType = JSonType_String;
    }

    void JSonValue::set(bool value){
        ClearObjectsOrArrays();
        mValue = value;
        mType = JSonType_Bool;
    }


    JSonObject& JSonValue::getObject(){
        if (mType == JSonType_Object)
            return boost::get<JSonObject>(mValue);
        throw std::runtime_error("Value is not a JSonObject type.");
    }

    JSonArray& JSonValue::getArray(){
        if (mType == JSonType_Array)
            return boost::get<JSonArray>(mValue);
        throw std::runtime_error("Value is not a JSonArray type.");
    }

    int JSonValue::getInt(){
        if (mType == JSonType_Number){
            switch (mValue.which()){
            case 2: // Int Type
                return boost::get<int>(mValue);
            case 3: // Float Type
                return static_cast<int>(boost::get<float>(mValue));
            case 4: // Double Type
                return static_cast<int>(boost::get<double>(mValue));
            }
        }
        throw std::runtime_error("Value is not a Number type.");
    }

    float JSonValue::getFloat(){
        if (mType == JSonType_Number){
            switch (mValue.which()){
            case 2: // Int Type
                return static_cast<float>(boost::get<int>(mValue));
            case 3: // Float Type
                return boost::get<float>(mValue);
            case 4: // Double Type
                return static_cast<float>(boost::get<double>(mValue));
            }
        }
        throw std::runtime_error("Value is not a Number type.");
    }

    double JSonValue::getDouble(){
        if (mType == JSonType_Number){
            switch (mValue.which()){
            case 2: // Int Type
                return static_cast<double>(boost::get<int>(mValue));
            case 3: // Float Type
                return static_cast<double>(boost::get<float>(mValue));
            case 4: // Double Type
                return boost::get<double>(mValue);
            }
        }
        throw std::runtime_error("Value is not a Number type.");
    }

    std::string JSonValue::getString(){
        if (mType == JSonType_String){
            return boost::get<std::string>(mValue);
        }
        throw std::runtime_error("Value is not a String type.");
    }

    bool JSonValue::getBool(){
        if (mType == JSonType_Bool){
            return boost::get<bool>(mValue);
        }
        throw std::runtime_error("Value is not a Boolean type.");
    }


    void JSonValue::ClearObjectsOrArrays(){
        switch (mType){
        case JSonType_Object:
            {
                JSonObject* obj = boost::get<JSonObject*>(mValue);
                delete obj;
            }
            break;
        case JSonType_Array:
            {
                JSonArray* arr = boost::get<JSonArray*>(mValue);
                delete arr;
            }
            break;
        default:
            break;
        }
    }


    /*JSonValue &JSonValue::operator[](const int index);
    JSonValue &JSonValue::operator[](const std::string key);
    JSonValue &JSonValue::operator[](const char* key);*/

} /* End of json namespace*/ } /* End of engine namespace */
