#pragma once

#include <vector>
#include <memory>
#include <string>
#include <set>
#include "color.h"
#include "common.h"
#include "point_set.h"
#include "plane.h"
class PointSet;

class GIROPOLYFIT_API VertexGroup : public std::vector<unsigned int>
{
public:
    VertexGroup(std::shared_ptr<PointSet> pset = nullptr)
        : label_("unknown"), point_set_(pset), visible_(true), highlighted_(false)
    {
    }
    ~VertexGroup()
    {
        clear();
    }

    const std::string label() { return label_; }
    void set_label(const std::string label) { label_ = label; }
    size_t num_vertex() { return size(); }

    const Color &color() const { return color_; }
    void set_color(const Color &color) { color_ = color; }

    Plane &plane() { return plane_; }
    void set_plane(const Plane &plane) { plane_ = plane; }

    std::shared_ptr<PointSet> point_set() { return point_set_; }
    void set_point_set(std::shared_ptr<PointSet> pset) { point_set_ = pset; }

    std::shared_ptr<VertexGroup> parent;

    std::vector<std::shared_ptr<VertexGroup>> children()
    {
        std::vector<std::shared_ptr<VertexGroup>> tmp(children_.begin(), children_.end());
        return tmp;
    }
    void add_child(std::shared_ptr<VertexGroup> g);
    void remove_child(std::shared_ptr<VertexGroup> g);
    inline void remove_all_children();

    bool is_visible() const { return visible_; }
    void set_visible(bool b) { visible_ = b; }

    bool is_highlighted() const { return highlighted_; }
    virtual void set_highlighted(bool b) { highlighted_ = b; }

private:
    Color color_;
    Plane plane_;
    std::set<std::shared_ptr<VertexGroup>> children_;
    std::string label_;
    std::shared_ptr<PointSet> point_set_;
    bool visible_;
    bool highlighted_;
};

class VertexGroupCmpIncreasing
{
public:
    VertexGroupCmpIncreasing() {}

    bool operator()(const std::shared_ptr<VertexGroup> g0, const std::shared_ptr<VertexGroup> g1) const
    {
        return g0->size() < g1->size();
    }
};

class VertexGroupCmpDecreasing
{
public:
    VertexGroupCmpDecreasing() {}

    bool operator()(const std::shared_ptr<VertexGroup> g0, const std::shared_ptr<VertexGroup> g1) const
    {
        return g0->size() > g1->size();
    }
};

inline void VertexGroup::add_child(std::shared_ptr<VertexGroup> g)
{
    g->set_point_set(this->point_set());
    children_.insert(g);

    // g->set_color(color());
}

inline void VertexGroup::remove_child(std::shared_ptr<VertexGroup> g)
{
    children_.erase(g);
}

inline void VertexGroup::remove_all_children()
{
    children_.clear();
}