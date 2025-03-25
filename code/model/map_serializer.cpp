#include "map_serializer.h"
#include "utils.h"
#include <string>
#include <fstream>
#include <sstream>
MapSerializer_obj::MapSerializer_obj()
{
}

bool MapSerializer_obj::serialize_read(const std::string &file_name, std::shared_ptr<Map> mesh)
{
    // 检查文件读取，具体obj文件解析逻辑在do_read中执行
    std::string cur_dir = FileUtils::dir_name(file_name);

    if (!mesh)
    {
        Logger::error("MapSerializer") << "mesh is null" << std::endl;
    }

    std::fstream::openmode mode = std::fstream::in;

    std::ifstream input(cur_dir.c_str(), mode);
    if (input.fail())
    {
        Logger::error("MapSerializer") << "Could not open file\'" << file_name << "\'" << std::endl;
    }
    MapBuilder builder(mesh);
    return do_read(input, builder);
}

bool MapSerializer_obj::do_read(std::istream &input, MapBuilder &builder)
{
    auto color_ = builder.get_vertex_attribute_handle<Color>("color");
    auto lock_ = builder.get_vertex_attribute_handle<bool>("lock");
    std::string line;

    builder.begin_surface();
    while (!input.eof())
    {
        std::getline(input, line);
        std::istringstream stream(line);
        std::string token;

        stream >> token;
        if (token == "v")
        {
            Math::vec3 p;
            stream >> p;
            builder.add_vertex(p);
        }
        else if (token == "vt")
        {
            Math::vec2 q;
            stream >> q;
            // builder.add_tex_vertex(q);TODO: 实现纹理
        }
        else if (token == "f")
        {
            builder.begin_facet();
            while (!stream || !stream.eof())
            {
                std::string s;
                stream >> s;
                if (s.length())
                {
                    std::istringstream v_input(s);
                    int index;
                    v_input >> index;
                    builder.add_vertex_to_facet(index - 1);
                    char c;
                    v_input >> c;
                    if (c == '/')
                    {
                        v_input >> index;
                        // builder.set_corner_tex_vertex(index-1);
                    }
                }
            }
            builder.end_facet();
            color_->set(builder.current_facet()->id, current_material_);
        }
        else if (token == "#")
        {
            std::string second_keyword;
            stream >> second_keyword;
            if (second_keyword == "anchor")
            {
                int index;
                stream >> index;
                lock_->set(builder.vertex()[index]->id, true);
            }
        }
        else if (token == "mtllib")
        {
            std::string mtl_lib_filename;
            stream >> mtl_lib_filename;
            mtl_lib_filename = current_directory_ + "/" + mtl_lib_filename;
            std::ifstream mtl_lib_in(mtl_lib_filename.c_str());
            if (mtl_lib_in)
            {
                Logger::out("MapSerializer_obj") << "using material lib: " << mtl_lib_filename << std::endl;
                read_mtl_lib(mtl_lib_in);
            }
            else
            {
                Logger::error("MapSerializer_obj") << mtl_lib_filename << ": no such file" << std::endl;
            }
        }
        else if (token == "usemtl")
        {
            std::string material;
            stream >> material;
            MaterialLib::iterator it = material_lib_.find(material);
            if (it == material_lib_.end())
            {
                current_material_ = Color(0.7f, 0.7f, 0.7f, 1.0f); // default mat
            }
            else
            {
                current_material_ = it->second;
            }
        }
    }

    return false;
}

bool MapSerializer_obj::do_write(std::ostream &output, const Map *mesh) const
{
    // 空实现
    return false;
}

void MapSerializer_obj::read_mtl_lib(std::istream &input)
{
    std::string keyword;
    std::string cur_mtl = "default";
    while (!input.eof())
    {
        std::string line;
        std::getline(input, line);
        std::stringstream in(line);
        in >> keyword;
        if (keyword == "newmtl")
        {
            in >> cur_mtl;
        }
        else if (keyword == "Kd")
        {
            Color c;
            in >> c;
            material_lib_[cur_mtl] = c;
        }
    }
}
