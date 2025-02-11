#include "utils.h"
namespace FileUtils
{

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
}