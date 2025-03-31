#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
namespace FileUtils
{

    static const char *const PATH_SEPARATORS = "/\\";

    std::string to_lower_case(const std::string &str)
    {
        std::string lowcase_str(str);
        for (std::string::iterator itr = lowcase_str.begin();
             itr != lowcase_str.end();
             ++itr)
        {
            *itr = tolower(*itr);
        }
        return lowcase_str;
    }
    std::string extension(const std::string &file_name)
    {
        size_t dot = file_name.find_last_of('.');
        if (dot == std::string::npos)
        {
            return "";
        }

        std::string ext = file_name.substr(dot + 1);
        return to_lower_case(ext);
    }

    std::string dir_name(const std::string &file_name)
    {
        std::string::size_type slash = file_name.find_last_of(PATH_SEPARATORS);
        if (slash == std::string::npos)
            return std::string();
        else
            return std::string(file_name, 0, slash);
    }
    std::string load_text_source(const std::string &filepath)
    {
        std::ifstream file(filepath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}