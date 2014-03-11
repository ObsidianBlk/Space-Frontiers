
#include <regex>
#include "JSonValue.h"


namespace engine{ namespace json {

    // TODO: Possibly move these functions into a utility function file.
    // NOTE: These functions are based on code found at:
    // http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
    inline std::string& _ltrim(std::string &s){
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    inline std::string& _rtrim(std::string &s){
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    inline std::string& _trim(std::string &s){
        return _ltrim(_rtrim(s));
    }

    std::string& _deserializeChars(std::string &s){
        bool escaped = false;
        //std::string::const_iterator i = s.begin();
        size_t i = 0;

        while (i != s.size()){
            switch (s[i]){
            case '\\':
                if (escaped){
                    s.replace(i-1, 2, "\\");
                    escaped = false;
                } else {
                    escaped = true;
                    i++;
                }
                break;
            case '"':
                if (escaped){
                    s.replace(i-1, 2, "\"");
                    escaped = false;
                } else {i++;}
                break;
            case '/':
                if (escaped){
                    s.replace(i-1, 2, "/");
                    escaped = false;
                } else {i++;}
                break;

            case 'b':
                if (escaped){
                    s.replace(i-1, 2, "\b");
                    escaped = false;
                } else {i++;}
                break;
            case 'f':
                if (escaped){
                    s.replace(i-1, 2, "\f");
                    escaped = false;
                } else {i++;}
                break;
            case 'n':
                if (escaped){
                    s.replace(i-1, 2, "\n");
                    escaped = false;
                } else {i++;}
                break;
            case 'r':
                if (escaped){
                    s.replace(i-1, 2, "\r");
                    escaped = false;
                } else {i++;}
                break;
            case 't':
                if (escaped){
                    s.replace(i-1, 2, "\t");
                    escaped = false;
                } else {i++;}
                break;
            default:
                if (escaped)
                    throw std::runtime_error("JSON String is malformed.");
                i++;
            }
        }

        return s;
    }


    JSonValue JSonValue::ParseFromString(std::string jsonstr){
        std::regex rx_nonescapedquote("[^\\]\"");
        JSonValue v;
        jsonstr = _deserializeChars(_trim(jsonstr));
        return v;
    }

} /* End of json namespace*/ } /* End of engine namespace */





