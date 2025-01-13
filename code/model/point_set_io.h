#include "common.h"

#include <string>
#include "pointset.h"
class PointSet;

class GIROPOLYFIT_API PointSetIO
{
    static PointSet *read(const std::string &file_name);

    static bool save(const std::string &file_name, const PointSet *point_set);
};