#pragma once
#define nil 0

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <string>
#include <vector>
namespace String
{
    void split(const std::string &in,
               char seperator,
               std::vector<std::string> &out,
               bool skip_empty_fields = true);

}