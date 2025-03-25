#pragma once
#include <iostream>
#include <map>
#include "common.h"
#include "color.h"
#include "map.h"
#include "map_builder.h"

class GIROPOLYFIT_API MapSerializer_obj
{
public:
    MapSerializer_obj();

    virtual bool serialize_read(
        const std::string &file_name, std::shared_ptr<Map> mesh);

protected:
    virtual bool do_read(std::istream &input, MapBuilder &builder);
    virtual bool do_write(std::ostream &output, const Map *mesh) const;
    void read_mtl_lib(std::istream &input);

protected:
    std::string current_directory_;

    typedef Color Material;
    typedef std::map<std::string, Material> MaterialLib;

    MaterialLib material_lib_;
    Material current_material_;
};
