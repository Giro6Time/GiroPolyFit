#include "map_io.h"
#include "map.h"
#include "map_serializer.h"
#include "utils.h"
#include "basic_types.h"

std::shared_ptr<Map> MapIO::read(std::string &filename)
{
    auto serializer = resolve_serializer(filename);
    if (serializer)
    {
        auto mesh = std::make_shared<Map>();

        Logger::out("-") << "reading file ..." << std::endl;

        if (serializer->serialize_read(filename, mesh))
        {
            // Logger::out("-") << "done. " << w.elapsed() << " sec." << std::endl; 监听时间
            return mesh;
        }
        else
        {
            mesh.reset();
            Logger::error("-") << "reading file failed" << std::endl;
        }
    }
    return nullptr;
}

bool MapIO::save(const std::string &filename, std::shared_ptr<Map> map)
{
    // 空实现
    return false;
}

std::shared_ptr<MapSerializer_obj> MapIO::resolve_serializer(const std::string &file_name)
{
    std::string extension = FileUtils::extension(file_name);

    if (extension.length() == 0)
    {
        Logger::error("MapIO") << "No extension in file name" << std::endl;
        return nil;
    }

    std::shared_ptr<MapSerializer_obj> serializer;

    if (extension == "obj")
        serializer = std::make_shared<MapSerializer_obj>();
    else if (extension == "eobj")
        serializer = std::make_shared<MapSerializer_obj>(); // TODO 拓展更多可文件类型
    else
    {
        Logger::error("-") << "unknown file format" << std::endl;
        return nullptr;
    }

    return serializer;
}
