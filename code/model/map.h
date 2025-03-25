#pragma once
#include "common.h"
#include "map_cell.h"
#include "simple_attribute.h"
#include "box.h"
#include <list>
#include <memory>

class Map;

template <class CELL>
class MapCombelObserver
{
public:
    MapCombelObserver(std::shared_ptr<Map> m) : map_(m) {}
    virtual ~MapCombelObserver() {}
    virtual void add(std::shared_ptr<CELL> c) = 0;
    virtual void remove(std::shared_ptr<CELL> c) = 0;

protected:
    std::shared_ptr<Map> map() { return map_; }
    std::shared_ptr<Map> map_;
};

class VertexObserver;
class HalfedgeObserver;
class FacetObserver;

class GIROPOLYFIT_API Map : public std::enable_shared_from_this<Map>
{
public:
    typedef MapTypes::Vertex Vertex;
    typedef MapTypes::Halfedge Halfedge;
    typedef MapTypes::Facet Facet;

    typedef std::list<std::shared_ptr<Vertex>>::iterator VertexIterator;
    typedef std::list<std::shared_ptr<Halfedge>>::iterator HalfedgeIterator;
    typedef std::list<std::shared_ptr<Facet>>::iterator FacetIterator;

    typedef std::list<std::shared_ptr<Vertex>>::const_iterator ConstVertexIterator;
    typedef std::list<std::shared_ptr<Halfedge>>::const_iterator ConstHalfedgeIterator;
    typedef std::list<std::shared_ptr<Facet>>::const_iterator ConstFacetIterator;

public:
    Map() : bbox_is_valid_(false), bbox_() {}
    virtual ~Map();

    inline VertexIterator vertices_begin() { return vertices_.begin(); }
    inline VertexIterator vertices_end() { return vertices_.end(); }
    inline HalfedgeIterator halfedges_begin() { return halfedges_.begin(); }
    inline HalfedgeIterator halfedges_end() { return halfedges_.end(); }
    inline FacetIterator facets_begin() { return facets_.begin(); }
    inline FacetIterator facets_end() { return facets_.end(); }

    inline ConstVertexIterator vertices_begin() const { return vertices_.begin(); }
    inline ConstVertexIterator vertices_end() const { return vertices_.end(); }
    inline ConstHalfedgeIterator halfedges_begin() const { return halfedges_.begin(); }
    inline ConstHalfedgeIterator halfedges_end() const { return halfedges_.end(); }
    inline ConstFacetIterator facets_begin() const { return facets_.begin(); }
    inline ConstFacetIterator facets_end() const { return facets_.end(); }

    inline size_t num_vertices() const { return vertices_.size(); }
    inline size_t num_halfedges() const { return halfedges_.size(); }
    inline size_t num_facets() const { return facets_.size(); }

    const Box3d &bbox() const;
    void invalidate_bbox() { bbox_is_valid_ = false; }

    // 属性管理
    AttributeStore &vertex_attribute_store() { return vertex_attribute_store_; }
    AttributeStore &halfedge_attribute_store() { return halfedge_attribute_store_; }
    AttributeStore &facet_attribute_store() { return facet_attribute_store_; }

    // 观察者实现
    void add_vertex_observer(std::shared_ptr<VertexObserver> observer)
    {
        vertex_observers_.push_back(observer);
    }
    void remove_vertex_observer(std::shared_ptr<VertexObserver> observer)
    {
        auto it = std::find(vertex_observers_.begin(), vertex_observers_.end(), observer);
        if (it != vertex_observers_.end())
        {
            vertex_observers_.erase(it);
        }
        my_assert_not_reached();
    }

    void add_halfedge_observer(std::shared_ptr<HalfedgeObserver> observer)
    {
        halfedge_observers_.push_back(observer);
    }
    void remove_halfedge_observer(std::shared_ptr<HalfedgeObserver> observer)
    {
        auto it = std::find(halfedge_observers_.begin(), halfedge_observers_.end(), observer);
        if (it != halfedge_observers_.end())
        {
            halfedge_observers_.erase(it);
        }
    }

    void add_facet_observer(std::shared_ptr<FacetObserver> observer)
    {
        facet_observers_.push_back(observer);
    }
    void remove_facet_observer(std::shared_ptr<FacetObserver> observer)
    {
        auto it = std::find(facet_observers_.begin(), facet_observers_.end(), observer);
        if (it != facet_observers_.end())
        {
            facet_observers_.erase(it);
        }
    }

    //
    void clear();
    void clear_inactive_items();

    void compute_vertex_normals();
    void compute_facet_normals();

    bool is_triangulated() const;
    bool is_valid() const;
    void assert_is_valid() const;

public:
    /// @brief 初始化一组对立的半边，确保他们的对立关系正确。
    /// @return 其中一条半边。可通过调用 Halfedge::opposite() 方法获取对立半边。
    std::shared_ptr<Halfedge> new_edge();

    std::shared_ptr<Vertex> new_vertex();
    std::shared_ptr<Halfedge> new_halfedge();
    std::shared_ptr<Facet> new_facet();

    std::shared_ptr<Vertex> copy_vertex(std::shared_ptr<Vertex> v);
    std::shared_ptr<Halfedge> copy_halfedge(std::shared_ptr<Halfedge> h);
    std::shared_ptr<Facet> copy_facet(std::shared_ptr<Facet> f);

    void delete_vertex(std::shared_ptr<Vertex> v);
    void delete_halfedge(std::shared_ptr<Halfedge> h);
    void delete_facet(std::shared_ptr<Facet> f);

    void activate_vertex(std::shared_ptr<Vertex> v);
    void activate_halfedge(std::shared_ptr<Halfedge> h);
    void activate_facet(std::shared_ptr<Facet> f);

    void deactivate_vertex(std::shared_ptr<Vertex> v);
    void deactivate_halfedge(std::shared_ptr<Halfedge> h);
    void deactivate_facet(std::shared_ptr<Facet> f);

protected:
    void notify_add_vertex(std::shared_ptr<Vertex> v);
    void notify_add_halfedge(std::shared_ptr<Halfedge> h);
    void notify_add_facet(std::shared_ptr<Facet> f);

    void notify_remove_vertex(std::shared_ptr<Vertex> v);
    void notify_remove_halfedge(std::shared_ptr<Halfedge> h);
    void notify_remove_facet(std::shared_ptr<Facet> f);

private:
    // elements
    std::list<std::shared_ptr<Vertex>> vertices_;
    std::list<std::shared_ptr<Halfedge>> halfedges_;
    std::list<std::shared_ptr<Facet>> facets_;

    std::list<std::shared_ptr<VertexObserver>> vertex_observers_;
    std::list<std::shared_ptr<HalfedgeObserver>> halfedge_observers_;
    std::list<std::shared_ptr<FacetObserver>> facet_observers_;

    AttributeStore vertex_attribute_store_;
    AttributeStore halfedge_attribute_store_;
    AttributeStore facet_attribute_store_;

    mutable bool bbox_is_valid_;
    mutable Box3d bbox_;

public:
    // Utils
    static void make_opposite(std::shared_ptr<MapTypes::Halfedge> h1, std::shared_ptr<MapTypes::Halfedge> h2)
    {
        h1->set_opposite(h2);
        h2->set_opposite(h1);
    }

    static void set_halfedge_vertex(std::shared_ptr<Halfedge> h, std::shared_ptr<Vertex> v) { h->set_vertex(v); }
    static void set_halfedge_facet(std::shared_ptr<Halfedge> h, std::shared_ptr<Facet> f) { h->set_facet(f); }
    static void set_halfedge_next(std::shared_ptr<Halfedge> h1, std::shared_ptr<Halfedge> h2) { h1->set_next(h2); }
    static void set_halfedge_prev(std::shared_ptr<Halfedge> h1, std::shared_ptr<Halfedge> h2) { h1->set_prev(h2); }
    static void set_vertex_halfedge(std::shared_ptr<Vertex> v, std::shared_ptr<Halfedge> h) { v->set_halfedge(h); }
    static void make_sequence(std::shared_ptr<Halfedge> h1, std::shared_ptr<Halfedge> h2)
    {
        h1->set_next(h2);
        h2->set_prev(h1);
    }
    static void make_facet_key(std::shared_ptr<Halfedge> h)
    {
        h->facet()->set_halfedge(h);
    }
};

/// @brief MapCombelObserver 类的特化版本，用于观察 Map::Vertex 类型的变化。
/// 该类实现了观察者模式，当在 Map 对象中添加或移除顶点时通知观察者。
class GIROPOLYFIT_API VertexObserver : public MapCombelObserver<std::shared_ptr<Map::Vertex>>
{
public:
    VertexObserver(std::shared_ptr<Map> m);
    virtual ~VertexObserver();
    virtual void add(std::shared_ptr<Map::Vertex> c);
    virtual void remove(std::shared_ptr<Map::Vertex> c);

protected:
    std::shared_ptr<Map> map() { return map_; }

private:
    std::shared_ptr<Map> map_;
};

/// @brief MapCombelObserver 类的特化版本，用于观察 Map::Halfedge 类型的变化。
/// 该类实现了观察者模式，当在 Map 对象中添加或移除半边时通知观察者。
class GIROPOLYFIT_API HalfedgeObserver : public MapCombelObserver<std::shared_ptr<Map::Halfedge>>
{
public:
    HalfedgeObserver(std::shared_ptr<Map> m);
    virtual ~HalfedgeObserver();
    virtual void add(std::shared_ptr<Map::Halfedge> c);
    virtual void remove(std::shared_ptr<Map::Halfedge> c);

protected:
    std::shared_ptr<Map> map() { return map_; }

private:
    std::shared_ptr<Map> map_;
};

/// @brief MapCombelObserver 类的特化版本，用于观察 Map::Facet 类型的变化。
/// 该类实现了观察者模式，当在 Map 对象中添加或移除面时通知观察者。
class GIROPOLYFIT_API FacetObserver : public MapCombelObserver<std::shared_ptr<Map::Facet>>
{
public:
    FacetObserver(std::shared_ptr<Map> m);
    virtual ~FacetObserver();
    virtual void add(std::shared_ptr<Map::Facet> c);
    virtual void remove(std::shared_ptr<Map::Facet> c);

protected:
    std::shared_ptr<Map> map();

private:
    std::shared_ptr<Map> map_;
};