#include "common.h"

#include <string>
#include <fstream>
#include <memory>
#include "point_set_serializer.h"
#include "point_set.h"
#include "logger.h"
#include "utils.h"
class PointSet;

class GIROPOLYFIT_API PointSetIO
{
public:
    static std::shared_ptr<PointSet> read(const std::string &file_name);

    static bool save(const std::string &file_name, std::shared_ptr<PointSet> point_set);
};