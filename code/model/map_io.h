#pragma once

#include "common.h"
#include "basic_types.h"
#include <memory>
#include <string>
class Map;
class MapSerializer;

class GIROPOLYFIT_API MapIO
{
public:
    static std::shared_ptr<Map> read(std::string &filename);
    static bool save(const std::string &filename, std::shared_ptr<Map> map);
};