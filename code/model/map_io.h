#pragma once

#include "common.h"
#include "basic_types.h"
#include "map_serializer.h"
#include <memory>
#include <string>
class Map;

class GIROPOLYFIT_API MapIO
{
public:
    static std::shared_ptr<Map> read(std::string &filename);
    static bool save(const std::string &filename, std::shared_ptr<Map> map);

private:
    static std::shared_ptr<MapSerializer_obj> MapIO::resolve_serializer(const std::string &file_name);
};