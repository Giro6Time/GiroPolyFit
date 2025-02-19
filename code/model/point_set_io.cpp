#include "point_set_io.h"

std::shared_ptr<PointSet> PointSetIO::read(const std::string &file_name)
{
    std::ifstream in(file_name.c_str());

    Logger::out("PointSetIO") << "try reading file: \"" << file_name << "\"" << std::endl;
    std::string extension = FileUtils::extension(file_name);

    std::shared_ptr<PointSet> ps = std::make_shared<PointSet>();
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

bool PointSetIO::save(const std::string &file_name, std::shared_ptr<PointSet> point_set)
{
    // 打开输出文件流
    std::ofstream out(file_name.c_str());

    if (out.fail())
    {
        Logger::error("PointSetIO") << "Fail to open file for writing: " << file_name << std::endl;
        return false;
    }

    // 记录日志
    Logger::out("PointSetIO") << "writing file..." << std::endl;

    // 获取文件扩展名
    std::string extension = FileUtils::extension(file_name);

    try
    {
        if (extension == "vg")
        {
            PointSetSerializer::save_vg(point_set, file_name);
        }
        else if (extension == "bvg")
        {
            PointSetSerializer::save_bvg(point_set, file_name);
        }
        else
        {
            Logger::error("PointSetIO") << "Unsupported file extension: " << extension << std::endl;
            return false;
        }

        // 成功保存文件
        out.close();
        Logger::out("PointSetIO") << "File saved successfully." << std::endl;
        return true;
    }
    catch (const std::exception &e)
    {
        Logger::error("PointSetIO") << "Exception occurred while saving file: " << e.what() << std::endl;
        out.close();
        return false;
    }
}