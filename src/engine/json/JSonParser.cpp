
//#include <regex>
#include <sstream>
#include <fstream>
#include "JSonValue.h"


namespace engine{ namespace json {

    // TODO: Possibly move these functions into a utility function file.
    // NOTE: These functions are based on code found at:
    // http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
    inline std::string _ltrim(std::string s){
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    inline std::string _rtrim(std::string s){
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    inline std::string _trim(std::string s){
        return _ltrim(_rtrim(s));
    }

    // A dead simple caseless equality check.
    bool _icaseeq(const std::string s1, const std::string s2){
        if (s1.size() != s2.size()){return false;}
        for (size_t i = 0; i < s1.size(); i++){
            if (tolower(s1[i]) != tolower(s2[i]))
                return false;
        }
        return true;
    }

    std::string _deserializeChars(std::string s){
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


    size_t _findClosingTailPos(const std::string &s, size_t pos, const char symhead, const char symtail){
        // NOTE: it is assumed that pos is the index of the character AFTER the opening symhead.
        bool instr = false;
        size_t depth = 0;
        for (size_t i = pos; i < s.size(); i++){
            switch(s[i]){
            case '"':
                if (i > 0){
                    if (s[i-1] != '\\'){
                        instr = !instr;
                    }
                } else {instr = !instr;}
                break;
            default:
                if (!instr){
                    if (s[i] == symhead){
                        depth++;
                    } else if (s[i] == symtail){
                        if (depth == 0)
                            return i;
                        depth--;
                    }
                }
            }
        }
        return std::string::npos;
    }

    size_t _findToSymbol(const std::string &s, size_t pos, const char sym){
        bool instr = false;
        bool inobj = false;
        bool inarr = false;
        for (size_t i = pos; i < s.size(); i++){
            switch(s[i]){
            case OBJECT_SYM_HEAD:
                if (!instr){
                    if (sym == OBJECT_SYM_HEAD && !inarr)
                        return i;
                    inobj = true;
                } break;
            case ARRAY_SYM_HEAD:
                if (!instr){
                    if (sym == ARRAY_SYM_HEAD && !inobj)
                        return i;
                    inarr = true;
                } break;
            case OBJECT_SYM_TAIL:
                if (!instr){
                    if (sym == OBJECT_SYM_TAIL && !inarr && !inobj)
                        return i;
                    inobj = false;
                } break;
            case ARRAY_SYM_TAIL:
                if (!instr){
                    if (sym == ARRAY_SYM_TAIL && !inarr && !inobj)
                        return i;
                    inarr = false;
                } break;
            case '"':
                if (!inarr && !inobj){
                    if (i > 0){
                        if (s[i-1] != '\\')
                            instr = !instr;
                    } else {instr = !instr;}
                }
                break;
            default:
                if (s[i] == sym && !instr && !inobj && !inarr)
                    return i;
                break;
            }
        }
        return std::string::npos;
    }

    JSonValue ParseDataValue(std::string data);
    JSonValue ParseObject(const std::string &s){
        size_t start_pos = _findToSymbol(s, 0, OBJECT_SYM_HEAD);
        size_t end_pos = _findClosingTailPos(s, start_pos+1, OBJECT_SYM_HEAD, OBJECT_SYM_TAIL);
        size_t str_pos = start_pos+1;


        if (start_pos == std::string::npos)
            throw std::runtime_error("JSON Parser Error: Given string is not in JSon Object format.");
        if (_trim(s.substr(0, start_pos)) != "") // JSon Object ({}) not found at the head of the string.
            throw std::runtime_error("JSON Parser Error: Given string is not in JSon Object format.");
        if (end_pos == std::string::npos)
            throw std::runtime_error("JSON Parser Error: JSon Object missing closing symbol.");


        JSonValue jobj = JSonValue::Object();
        std::string key = "";
        while (str_pos < end_pos){
            if (key == ""){
                size_t pos = _findToSymbol(s, str_pos, OBJECT_PAIR_SEPARATOR);
                if (pos != std::string::npos){
                    key = _trim(s.substr(str_pos, pos-str_pos));
                    if (key[0] != '"' || key[key.size()-1] != '"')
                        throw std::runtime_error("JSON Parser Error: Object keys must be strings.");
                    key = _deserializeChars(key.substr(1, key.size()-2));
                    str_pos = pos+1;
                } else {
                    // If all we have from the current position to the end of the string is the Object tail symbol,
                    // then we've either been given an empty Object ("{}") or the last item in the item list contains
                    // a trailing comma, which is perfectly legal.
                    //std::string tmp = _ltrim(s.substr(str_pos));
                    if (_ltrim(s.substr(str_pos, end_pos-str_pos)) == "")
                        str_pos = end_pos; // Jump to the end.
                    else{
                        // Of course... if there's more than just the Object tail symbol...
                        // we throw a fit!
                        throw std::runtime_error("JSON Parser Error: Malformed JSon Object Key:Value pairing.");
                    }
                }
            } else {
                size_t pos = _findToSymbol(s, str_pos, VALUE_SEPARATOR);
                if (pos == std::string::npos){
                    // If we've come to the end of the string, we assume the rest of the string is pure data!
                    pos = end_pos;
                }


                std::string data = _trim(s.substr(str_pos, pos-str_pos));
                try{
                    jobj[key] = ParseDataValue(data);
                } catch (std::runtime_error e){throw e;}

                str_pos = pos+1;
                key = "";
            }
        }

        return jobj;
    }


    JSonValue ParseArray(const std::string &s){
        size_t start_pos = _findToSymbol(s, 0, ARRAY_SYM_HEAD);
        size_t end_pos = _findClosingTailPos(s, start_pos+1, ARRAY_SYM_HEAD, ARRAY_SYM_TAIL);
        size_t str_pos = start_pos+1;


        if (start_pos == std::string::npos)
            throw std::runtime_error("JSON Parser Error: Given string is not in JSon Array format.");
        if (_trim(s.substr(0, start_pos)) != "") // JSon Object ({}) not found at the head of the string.
            throw std::runtime_error("JSON Parser Error: Given string is not in JSon Array format.");
        if (end_pos == std::string::npos)
            throw std::runtime_error("JSON Parser Error: JSon Array missing closing symbol.");


        JSonValue jarr = JSonValue::Array();
        while (str_pos < end_pos){
            size_t pos = _findToSymbol(s, str_pos, VALUE_SEPARATOR);
            if (pos == std::string::npos){
                // If we've come to the end of the string, we assume the rest of the string is pure data!
                pos = end_pos;
            }

            std::string data = _trim(s.substr(str_pos, pos-str_pos));
            if (data != ""){
                try{
                    jarr["+"] = ParseDataValue(data);
                } catch (std::runtime_error e){throw e;}

                str_pos = pos+1;
            } else {
                // Either we've been given an empty array, or the last item in the list
                // had a trailing comma, which is perfectly legal.
                str_pos = end_pos;
            }
        }

        return jarr;
    }


    JSonValue ParseDataValue(std::string data){
        if (_icaseeq(data, "true")){
            return JSonValue(true);
        } else if (_icaseeq(data, "false")){
            return JSonValue(false);
        } else if (_icaseeq(data, "null")){
            return JSonValue(); // This will create a "null" entry.
        } else if (data[0] == OBJECT_SYM_HEAD){
            try{
                return ParseObject(data);
            } catch (std::runtime_error e){throw e;}
        } else if (data[0] == ARRAY_SYM_HEAD){
            try{
                return ParseArray(data);
            } catch (std::runtime_error e){throw e;}
        } else if (data[0] == '"'){
            return  JSonValue(_deserializeChars(data.substr(1, data.size()-2)));
        } else {
            auto _stodbl = [](std::string s){
                // NOTE: This lambda should NOT be needed. std::stoi() should do the job, but it seems
                // to be missing in MinGW. Until I come up with a more elegant way to deal with that issue,
                // this lambda will exist.
                std::istringstream ss(s);
                double res;
                return ss >> res ? res : throw std::invalid_argument("String is not a number.");
            };

            //std::regex rx_numex("[-+]?((\\d+(\\.\\d+)?)|(\\.\\d+))");
            //if (std::regex_match(data, rx_numex)){
                try{
                    double d = _stodbl(data);
                    return JSonValue(d);
                } catch (std::invalid_argument e){;}
            //}
        }

        throw std::runtime_error("JSON Parser Error: Unknown value type.");
    }



    JSonValue JSonValue::ParseFromString(const std::string &jsonstr){
        /*jsonstr = _trim(jsonstr);
        if (jsonstr[0] == OBJECT_SYM_HEAD){
            return ParseObject(jsonstr);
        } else if (jsonstr[0] == ARRAY_SYM_HEAD){
            return ParseArray(jsonstr);
        }*/

        // The two try/catch blocks below may be hokey, but it's quick and should get to the point.
        try{
            return ParseObject(jsonstr);
        } catch (std::runtime_error e){;} // Ignore this.

        try{
            return ParseArray(jsonstr);
        } catch (std::runtime_error e){;} // Ignore this too.

        // And if both blocks fail...
        throw std::runtime_error("JSON Parser Error: JSON must start as either an Object or Array form.");
    }


    JSonValue JSonValue::ParseFromFile(const std::string &src){
        std::ifstream f(src.c_str());
        if (!f)
            throw std::runtime_error("File not found or cannot be read.");

        std::stringstream ss;
        ss << f.rdbuf();

        try{
            return JSonValue::ParseFromString(ss.str());
        } catch (std::runtime_error e){throw e;}
    }

} /* End of json namespace*/ } /* End of engine namespace */





