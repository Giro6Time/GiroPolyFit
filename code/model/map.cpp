#include "map.h"
#include "math_types.h"
#include "map_attribute.h"

Map::~Map() { clear(); }

const Box3d &Map::bbox() const
{
    if (!bbox_is_valid_)
    {
        bbox_ = GeometryTypes::bounding_box(shared_from_this());
        bbox_is_valid_ = true;
    }
    return bbox_;
}

void Map::compute_facet_normals()
{
    auto handle = facet_attribute_store_.create_handle<std::shared_ptr<Math::vec3>>("facet_normals");
    for (auto it = facets_begin(); it != facets_end(); it++)
    {
        auto sptr = std::make_shared<Math::vec3>(GeometryTypes::facet_normal(*it));
        (*handle).set((*it)->id, sptr);
    }
}

void Map::compute_vertex_normals()
{
    auto handle = vertex_attribute_store_.create_handle<std::shared_ptr<Math::vec3>>("vertex_normals");

    for (auto it = vertices_begin(); it != vertices_end(); it++)
    {
        auto sptr = std::make_shared<Math::vec3>(GeometryTypes::vertex_normal(*it));
        (*handle).set((*it)->id, sptr);
    }
}

void Map::clear()
{
    vertices_.clear();
    halfedges_.clear();
    facets_.clear();

    vertex_attribute_store_.clear();
    halfedge_attribute_store_.clear();
    facet_attribute_store_.clear();
    bbox_is_valid_ = false;
}

void Map::clear_inactive_items()
{
    vertices_.erase(
        std::remove_if(vertices_.begin(), vertices_.end(),
                       [](const std::shared_ptr<Vertex> &v)
                       { return !v->is_activated; }),
        vertices_.end());

    halfedges_.erase(
        std::remove_if(halfedges_.begin(), halfedges_.end(),
                       [](const std::shared_ptr<Halfedge> &h)
                       { return !h->is_activated; }),
        halfedges_.end());

    facets_.erase(
        std::remove_if(facets_.begin(), facets_.end(),
                       [](const std::shared_ptr<Facet> &f)
                       { return !f->is_activated; }),
        facets_.end());
}

bool Map::is_triangulated() const
{
    for (auto it = facets_begin(); it != facets_end(); it++)
    {
        if (!(*it)->is_triangle())
        {
            return false;
        }
    }
    return true;
}

bool Map::is_valid() const
{
    return false;
}

void Map::assert_is_valid() const
{
}

std::shared_ptr<Map::Halfedge> Map::new_edge()
{
    auto h1 = new_halfedge();
    auto h2 = new_halfedge();
    h1->set_opposite(h2);
    h2->set_opposite(h1);
    h1->set_next(h2);
    h2->set_next(h1);
    h1->set_prev(h2);
    h2->set_prev(h1);

    return h1;
}

std::shared_ptr<Map::Vertex> Map::new_vertex()
{
    auto v = std::make_shared<Vertex>();
    vertices_.push_back(v);
    notify_add_vertex(v);
    return v;
}

std::shared_ptr<Map::Halfedge> Map::new_halfedge()
{
    auto h = std::make_shared<Halfedge>();
    halfedges_.push_back(h);
    notify_add_halfedge(h);
    return h;
}

std::shared_ptr<Map::Facet> Map::new_facet()
{
    auto f = std::make_shared<Facet>();
    facets_.push_back(f);
    notify_add_facet(f);
    return f;
}

std::shared_ptr<Map::Vertex> Map::copy_vertex(std::shared_ptr<Map::Vertex> v)
{
    std::shared_ptr<Vertex> new_v = new_vertex();
    new_v->point() = v->point();
    vertex_attribute_store_.copy_attributes(v->id, new_v->id);
    notify_add_vertex(new_v);
    return new_v;
}

std::shared_ptr<Map::Halfedge> Map::copy_halfedge(std::shared_ptr<Map::Halfedge> h)
{
    std::shared_ptr<Halfedge> new_h = new_halfedge();
    halfedge_attribute_store_.copy_attributes(h->id, new_h->id);
    notify_add_halfedge(new_h);
    return new_h;
}

std::shared_ptr<Map::Facet> Map::copy_facet(std::shared_ptr<Map::Facet> f)
{
    std::shared_ptr<Facet> new_f = new_facet();
    facet_attribute_store_.copy_attributes(f->id, new_f->id);

    notify_add_facet(new_f);
    return new_f;
}

void Map::delete_vertex(std::shared_ptr<Map::Vertex> v)
{
    notify_remove_vertex(v);
    vertex_attribute_store_.delete_all_attributes(v->id);
    vertices_.remove(v);
}

void Map::delete_halfedge(std::shared_ptr<Map::Halfedge> h)
{
    notify_remove_halfedge(h);
    vertex_attribute_store_.delete_all_attributes(h->id);
    halfedges_.remove(h);
}

void Map::delete_facet(std::shared_ptr<Map::Facet> f)
{
    notify_add_facet(f);
    vertex_attribute_store_.delete_all_attributes(f->id);
    facets_.remove(f);
}

void Map::activate_vertex(std::shared_ptr<Map::Vertex> v)
{
    v->is_activated = true;
    notify_add_vertex(v);
}

void Map::activate_halfedge(std::shared_ptr<Map::Halfedge> h)
{
    h->is_activated = true;
    notify_add_halfedge(h);
}

void Map::activate_facet(std::shared_ptr<Map::Facet> f)
{
    f->is_activated = true;
    notify_add_facet(f);
}

void Map::deactivate_vertex(std::shared_ptr<Map::Vertex> v)
{
    v->is_activated = false;
    notify_remove_vertex(v);
}

void Map::deactivate_halfedge(std::shared_ptr<Map::Halfedge> h)
{
    h->is_activated = false;
    notify_remove_halfedge(h);
}

void Map::deactivate_facet(std::shared_ptr<Map::Facet> f)
{
    f->is_activated = false;
    notify_remove_facet(f);
}

void Map::notify_add_vertex(std::shared_ptr<Map::Vertex> v)
{
    for (auto it = vertex_observers_.begin(); it != vertex_observers_.end(); it++)
    {
        (*it)->add(v);
    }
}

void Map::notify_add_halfedge(std::shared_ptr<Map::Halfedge> h)
{
    for (auto it = halfedge_observers_.begin(); it != halfedge_observers_.end(); it++)
    {
        (*it)->add(h);
    }
}

void Map::notify_add_facet(std::shared_ptr<Map::Facet> f)
{
    for (auto it = facet_observers_.begin(); it != facet_observers_.end(); it++)
    {
        (*it)->add(f);
    }
}

void Map::notify_remove_vertex(std::shared_ptr<Map::Vertex> v)
{
    for (auto it = vertex_observers_.begin(); it != vertex_observers_.end(); it++)
    {
        (*it)->remove(v);
    }
}

void Map::notify_remove_halfedge(std::shared_ptr<Map::Halfedge> h)
{
    for (auto it = halfedge_observers_.begin(); it != halfedge_observers_.end(); it++)
    {
        (*it)->remove(h);
    }
}

void Map::notify_remove_facet(std::shared_ptr<Map::Facet> f)
{
    for (auto it = facet_observers_.begin(); it != facet_observers_.end(); it++)
    {
        (*it)->remove(f);
    }
}

VertexObserver::VertexObserver(std::shared_ptr<Map> m)
    : MapCombelObserver(m)
{
}

VertexObserver::~VertexObserver()
{
}

void VertexObserver::add(std::shared_ptr<Map::Vertex> v)
{
}

void VertexObserver::remove(std::shared_ptr<Map::Vertex> v)
{
}

HalfedgeObserver::HalfedgeObserver(std::shared_ptr<Map> m)
    : MapCombelObserver(m)
{
}

HalfedgeObserver::~HalfedgeObserver()
{
}

void HalfedgeObserver::add(std::shared_ptr<Map::Halfedge> h)
{
}

void HalfedgeObserver::remove(std::shared_ptr<Map::Halfedge> h)
{
}

FacetObserver::FacetObserver(std::shared_ptr<Map> m)
    : MapCombelObserver(m)
{
}

FacetObserver::~FacetObserver()
{
}

void FacetObserver::add(std::shared_ptr<Map::Facet> f)
{
}

void FacetObserver::remove(std::shared_ptr<Map::Facet> f)
{
}
