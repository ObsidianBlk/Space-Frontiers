#ifndef JSONVALUE_H
#define JSONVALUE_H


#include <string>
#include <map>
#include <vector>

#include <boost/variant.hpp>

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
            JSonValue(JSonObject& value);
            JSonValue(JSonArray& value);
            JSonValue(int value);
            JSonValue(float value);
            JSonValue(double value);
            JSonValue(std::string value);
            JSonValue(bool value);
            ~JSonValue();


            JSonType type();

            void set(JSonObject& value);
            void set(JSonArray& value);
            void set(int value);
            void set(float value);
            void set(double value);
            void set(std::string value);
            void set(bool value);

            JSonObject& getObject();
            JSonArray& getArray();
            int getInt();
            float getFloat();
            double getDouble();
            std::string getString();
            bool getBool();

            /*JSonValue &operator[](const int index);
            JSonObject &operator[](const int index);
            JSonArray &operator[](const int index);

            JSonValue &operator[](const std::string key);
            JSonValue &operator[](const char* key);*/

        protected:
            typedef boost::variant<JSonObject, JSonArray, int, float, double, std::string, bool> jvar;

        private:
            JSonType mType;
            jvar mValue;

            void ClearObjectsOrArrays();
    };

} /* End of json namespace*/ } /* End of engine namespace */
#endif // JSONVALUE_H
