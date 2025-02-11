#include "point_set_io.h"

PointSet *PointSetIO::read(const std::string &file_name)
{
    std::ifstream in(file_name.c_str());

    if (in.fail())
    {
        Logger::error("PointSetIO") << "Fail to open file: " << file_name << std::endl;
        return nil;
    }
    in.close();

    Logger::out("PointSetIO") << "reading file..." << std::endl;
    std::string extension = FileUtils::extension(file_name);

    PointSet *ps = new PointSet;
    if (extension == "vg")
    {
        PointSetSerializer::load_vg(ps, file_name);
    }
    else if (extension == "bvg")
    {
        PointSetSerializer::load_bvg(ps, file_name);
    }

    return ps;
}