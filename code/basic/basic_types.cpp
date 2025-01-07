#include "basic_types.h"

namespace String
{
    void split(const std::string &in,
               char seperator,
               std::vector<std::string> &out,
               bool skip_empty_fields)

    {
        size_t length = in.length();
        size_t start = 0;
        size_t end = 0;

        while (start < length)
        {
            end = in.find(seperator, start);
            if (end == std::string::npos)
            {
                end = length;
            }
            if (!skip_empty_fields || (end - start > 0))
            {
                out.push_back(in.substr(start, end - start));
            }
            start = end + 1;
        }
    }
}