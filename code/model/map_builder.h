#pragma once
#include <memory>
#include "map.h"
#include "logger.h"
#include "basic.h"
#include "simple_attribute.h"

class MapBuilder
{
    typedef Map::Vertex Vertex;
    typedef Map::Facet Facet;
    enum state
    {
        initial,
        surface,
        facet,
        final
    };

public:
    MapBuilder(std::shared_ptr<Map> map);

    // state surface
    void begin_surface();
    void end_surface();
    void add_vertex(const Math::vec3 &v);

    // state facet
    void begin_facet();
    void end_facet();
    void add_vertex_to_facet(int i);

    // implement
    template <typename T>
    std::shared_ptr<AttributeHandle<T>> get_vertex_attribute_handle(std::string attr_name)
    {
        return target_->vertex_attribute_store().create_handle<T>(attr_name);
    }
    template <typename T>
    std::shared_ptr<AttributeHandle<T>> get_halfedge_attribute_handle(std::string attr_name)
    {
        return target_->halfedge_attribute_store().create_handle<T>(attr_name);
    }
    template <typename T>
    std::shared_ptr<AttributeHandle<T>> get_facet_attribute_handle(std::string attr_name)
    {
        return target_->facet_attribute_store().create_handle<T>(attr_name);
    }

    const std::shared_ptr<Map::Facet> &current_facet() const
    {
        return current_f_;
    }

    const std::vector<std::shared_ptr<Vertex>> &vertex() const
    {
        return vertex_;
    }

protected:
    // state管理
    void
    transition(state from, state to);
    void check_state(state s);
    std::string state_to_string(state s);

    void add_vertex_internal(const Math::vec3 &v);
    std::shared_ptr<Map::Vertex> copy_vertex(std::shared_ptr<Vertex> v);

    void terminate_surface();

    void begin_facet_internal();
    void end_facet_internal();

    bool split_non_manifold_vertex(const std::shared_ptr<Vertex> v);
    bool vertex_is_manifold(std::shared_ptr<Map::Vertex> v);
    void disconnect_vertex(
        std::shared_ptr<Map::Halfedge> start_in,
        std::shared_ptr<Map::Vertex> v,
        std::set<std::shared_ptr<Map::Halfedge>> &star);

    std::shared_ptr<Map::Halfedge> MapBuilder::find_halfedge_between(
        std::shared_ptr<Map::Vertex> from,
        std::shared_ptr<Map::Vertex> to);

    std::shared_ptr<Map::Halfedge> MapBuilder::new_halfedge_between(
        std::shared_ptr<Vertex> from,
        std::shared_ptr<Vertex> to);
    void MapBuilder::add_vertex_to_facet_internal(std::shared_ptr<Vertex> v);

private:
    state state_;
    std::shared_ptr<Vertex> current_v_;
    std::shared_ptr<Facet> current_f_;
    std::shared_ptr<Map::Halfedge> current_h_;
    std::shared_ptr<Map> target_;
    std::shared_ptr<Vertex> first_vertex_in_facet_;
    std::shared_ptr<Map::Halfedge> first_halfedge_in_facet_;

    int num_non_manifold_v_;
    int num_duplicate_e_;
    int num_isolated_v_;

    typedef std::vector<std::shared_ptr<Map::Halfedge>> Star;
    std::shared_ptr<AttributeHandle<Star>> star_;
    typedef AttributeHandle<bool> MapVertexLock;
    std::shared_ptr<MapVertexLock> is_locked_;

    std::vector<std::shared_ptr<Vertex>> vertex_;
    std::vector<std::shared_ptr<Vertex>> facet_vertex_;

    bool quiet_;
};