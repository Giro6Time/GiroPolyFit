#include "common.h"
#include "map_cell.h"
#include <list>
#include <memory>
class MapMutator;

class GIROPOLYFIT_API Map
{
public:
    typedef MapTypes::Vertex Vertex;
    typedef MapTypes::Halfedge Halfedge;
    typedef MapTypes::Facet Facet;

private:
    // elements
    std::list<Vertex> vertices_;
    std::list<Halfedge> halfedges_;
    std::list<Facet> facets_;

    // VertexAttributeManager vertex_attribute_manager_;
    // HalfedgeAttributeManager halfedge_attribute_manager_;
    // FacetAttributeManager facet_attribute_manager_;

    // std::vector<MapCombelObserver<Vertex> *> vertex_observers_;
    // std::vector<MapCombelObserver<Halfedge> *> halfedge_observers_;
    // std::vector<MapCombelObserver<Facet> *> facet_observers_;

    mutable bool bbox_is_valid_;
    // mutable Box3d bbox_;
};

class GIROPOLYFIT_API MapMutator{

};