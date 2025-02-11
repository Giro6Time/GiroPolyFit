#include "common.h"

#include <string>
#include <fstream>
#include "point_set_serializer.h"
#include "point_set.h"
#include "logger.h"
#include "utils.h"

class PointSet;

class GIROPOLYFIT_API PointSetIO
{
public:
    static PointSet *read(const std::string &file_name);

    static bool save(const std::string &file_name, const PointSet *point_set);
};