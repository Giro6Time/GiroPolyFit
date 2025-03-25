#pragma once
#include <string>

namespace FileUtils
{
    std::string to_lower_case(const std::string &str);
    std::string extension(const std::string &file_name);

    std::string dir_name(const std::string &file_name);
}