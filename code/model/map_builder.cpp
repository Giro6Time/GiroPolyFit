#include "map_builder.h"
#include <algorithm>

struct FacetKey
{
    FacetKey(std::shared_ptr<Map::Vertex> a1, std::shared_ptr<Map::Vertex> a2, std::shared_ptr<Map::Vertex> a3)
    {
        v1 = a1;
        v2 = a2;
        v3 = a3;
    }
    bool operator<(const FacetKey &rhs) const
    {
        if (v1 < rhs.v1)
        {
            return true;
        }
        if (v1 > rhs.v1)
        {
            return false;
        }
        if (v2 < rhs.v2)
        {
            return true;
        }
        if (v2 > rhs.v2)
        {
            return false;
        }
        return (v3 < rhs.v3);
    }
    std::shared_ptr<Map::Vertex> v1;
    std::shared_ptr<Map::Vertex> v2;
    std::shared_ptr<Map::Vertex> v3;
};

static std::shared_ptr<std::set<FacetKey>> all_facet_keys;

// construct
MapBuilder::MapBuilder(std::shared_ptr<Map> map) : target_(map)
{
}

// state
void MapBuilder::transition(state from, state to)
{
    check_state(from);
    state_ = to;
}

void MapBuilder::check_state(state s)
{
    if (state_ != s)
    {
        Logger::error("MapBuilder") << "invalid state: \'"
                                    << state_to_string(state_)
                                    << "\', expected \'"
                                    << state_to_string(s)
                                    << "\'"
                                    << std::endl;
        my_assert_not_reached();
    }
}

std::string MapBuilder::state_to_string(state s)
{
    switch (s)
    {
    case initial:
        return "initial";
    case surface:
        return "surface";
    case facet:
        return "facet";
    case final:
        return "final";
    default:
        return "unknown";
    }
    return "unknown";
}

// while state surface
void MapBuilder::begin_surface()
{
    transition(initial, surface);
    star_ = target_->vertex_attribute_store().create_handle<Star>("star");
    is_locked_ = target_->vertex_attribute_store().create_handle<bool>("lock");
    // is_locked_.bind(target());
    num_non_manifold_v_ = 0;
    num_duplicate_e_ = 0;
    num_isolated_v_ = 0;
    all_facet_keys = std::make_shared<std::set<FacetKey>>();
}

void MapBuilder::end_surface()
{
    all_facet_keys.reset();

    transition(surface, final);
    terminate_surface();
    vertex_.clear();
    // star_.unbind();属性句柄在MapBuilder失效后自动解绑

    if (!quiet_ && (num_non_manifold_v_ != 0))
    {
        Logger::warn("MapBuilder")
            << "Encountered " << num_non_manifold_v_
            << " non-manifold vertices (fixed)" << std::endl;
    }
    if (!quiet_ && (num_duplicate_e_ != 0))
    {
        Logger::warn("MapBuilder")
            << "Encountered " << num_duplicate_e_
            << " duplicated edges (fixed)" << std::endl;
    }
    if (!quiet_ && (num_isolated_v_ != 0))
    {
        Logger::warn("MapBuilder")
            << "Encountered " << num_isolated_v_
            << " isolated vertices (removed)" << std::endl;
    }
    if (!quiet_ && target_->num_facets() == 0)
    {
        Logger::warn("MapBuilder")
            << "0 facets. Is it a point cloud?" << std::endl;
    }
}

void MapBuilder::terminate_surface()
{

    // Step 1 : create the border halfedges, and setup the 'opposite'
    //   and 'vertex' pointers.
    auto hbegin = target_->halfedges_begin();
    auto hend = target_->halfedges_end();
    for (auto it = hbegin; it != hend; it++)
    {
        if (!(*it)->opposite())
        {
            auto h = std::make_shared<Map::Halfedge>();
            Map::make_opposite(h, *it);
            Map::set_halfedge_vertex(h, (*it)->prev()->vertex());

            // Used later to fix non-manifold vertices.
            star_->get((*it)->vertex()->id).push_back(h);
        }
    }

    // Step 2 : setup the 'next' and 'prev' pointers of the border.
    //
    for (auto it = hbegin; it != hend; it++)
    {
        if (!(*it)->facet())
        {

            auto next = (*it)->opposite();
            while (!next->facet())
            {
                next = next->prev()->opposite();
            }
            Map::set_halfedge_next(*it, next);

            auto prev = (*it)->opposite();
            while (!prev->facet())
            {
                prev = prev->next()->opposite();
            }
            Map::set_halfedge_prev(*it, prev);
        }
    }

    // Step 3 : fix non-manifold vertices

    for (unsigned int i = 0; i < vertex_.size(); i++)
    {
        if (split_non_manifold_vertex(vertex_[i]))
        {
            num_non_manifold_v_++;
        }
    }

    // Step 4 : check for isolated vertices
    for (unsigned int i = 0; i < vertex_.size(); i++)
    {
        if (star_->get(vertex_[i]->id).size() == 0)
        {
            num_isolated_v_++;
            target_->delete_vertex(vertex_[i]);
        }
    }

    target_->assert_is_valid();
}

bool MapBuilder::split_non_manifold_vertex(const std::shared_ptr<Vertex> v)
{
    if (vertex_is_manifold(v))
    {
        return false;
    }

    std::set<std::shared_ptr<Map::Halfedge>> star;
    {
        auto &star_of_v = star_->get(v->id);
        for (unsigned int i = 0; i < star_of_v.size(); i++)
        {
            star.insert(star_of_v[i]);
        }
    }

    // For the first wedge, reuse the vertex
    disconnect_vertex(
        v->halfedge()->opposite(), v, star);

    // There should be other wedges (else the vertex
    // would have been manifold)
    my_assert(!star.empty());

    {
        is_locked_->set(v->id, true);
        // Create the vertices for the remaining wedges.
        while (!star.empty())
        {
            auto new_v = copy_vertex(v);
            auto h = *(star.begin());
            disconnect_vertex(h, new_v, star);
        }
    }

    return true;
}

bool MapBuilder::vertex_is_manifold(std::shared_ptr<Map::Vertex> v)
{
    if (v->halfedge() == nil)
    {
        // Isolated vertex, removed in subsequent steps
        return true;
    }

    // A vertex is manifold if the stored and the
    // computed stars match (the number of halfedges
    // are tested).
    // Note: this test is valid only if the borders
    // have been constructed.

    return (int(star_->get(v->id).size()) == v->degree());
}
void MapBuilder::disconnect_vertex(
    std::shared_ptr<Map::Halfedge> start_in, std::shared_ptr<Map::Vertex> v, std::set<std::shared_ptr<Map::Halfedge>> &star)
{
    auto start = start_in;

    auto &star_of_v = star_->get(v->id);
    star_of_v.clear();

    //   Important note: in this class, all the Stars correspond to the
    // set of halfedges radiating FROM a vertex (i.e. h->vertex() != v
    // if h belongs to Star(v) ).

    my_assert(star.find(start) != star.end());

    /*为什么要求 start 一定要是边界？
        为了确保在处理非流形顶点时，能够正确地处理边界情况。
        边界半边需要特殊处理，因为它们不与其他半边正确连接。
        通过确保 start 是边界半边，可以确保在处理非流形顶点
        时，能够正确地处理边界情况。
    */
    //   Note: the following code needs a special handling
    // of borders, since borders are not correctly connected
    // in a non-manifold vertex (the standard next_around_vertex
    // iteration does not traverse the whole star)

    while (!start->is_border())
    {
        start = start->prev()->opposite();
        if (start == start_in)
        {
            break;
        }
    }
    Map::set_vertex_halfedge(v, start->opposite());

    auto cur = start;
    Map::set_halfedge_vertex(cur->opposite(), v);
    star_of_v.push_back(cur);
    auto it = star.find(cur);
    my_assert(it != star.end());
    star.erase(it);

    while (!cur->opposite()->is_border())
    {
        cur = cur->opposite()->next();
        if (cur == start)
        {
            break;
        }
        Map::set_halfedge_vertex(cur->opposite(), v);
        auto it = star.find(cur);
        my_assert(it != star.end());
        star.erase(it);
        star_of_v.push_back(cur);
    }

    if (start->is_border())
    {
        Map::make_sequence(cur->opposite(), start);
    }
}
void MapBuilder::add_vertex(const Math::vec3 &v)
{
    transition(surface, surface);
    add_vertex_internal(v);
}

void MapBuilder::add_vertex_internal(const Math::vec3 &v)
{
    auto new_v = target_->new_vertex();
    new_v->set_point(v);
}

// #region state facet

void MapBuilder::begin_facet()
{
    facet_vertex_.clear();
}

void MapBuilder::end_facet()
{
    int num_vertices = facet_vertex_.size();

    if (num_vertices < 3)
    {
        if (!quiet_)
        {
            Logger::error("MapBuilder")
                << "Facet with less than 3 vertices, ignored"
                << std::endl;
        }
        return;
    }

    if (false && num_vertices == 3)
    {
        auto W = facet_vertex_;
        std::sort(W.begin(), W.end());
        FacetKey k(W[0], W[1], W[2]);
        if (all_facet_keys->find(k) != all_facet_keys->end())
        {
            std::cerr << '.' << std::flush;
            return;
        }
        all_facet_keys->insert(k);
    }

    // Detect duplicated vertices
    {
        for (int i = 0; i < num_vertices; i++)
        {
            for (int j = i + 1; j < num_vertices; j++)
            {
                if (facet_vertex_[i] == facet_vertex_[j])
                {
                    if (!quiet_)
                    {
                        Logger::error("MapBuilder")
                            << "Facet with duplicated vertices, ignored"
                            << std::endl;
                    }
                    return;
                }
            }
        }
    }

    // Detect and remove non-manifold edges by duplicating
    //   the corresponding vertices.
    {
        for (int from = 0; from < num_vertices; from++)
        {
            int to = ((from + 1) % num_vertices);
            if (find_halfedge_between(facet_vertex_[from], facet_vertex_[to]))
            {
                num_duplicate_e_++;
                facet_vertex_[from] = copy_vertex(facet_vertex_[from]);
                facet_vertex_[to] = copy_vertex(facet_vertex_[to]);
            }
        }
    }

    begin_facet_internal();
    for (int i = 0; i < num_vertices; i++)
    {
        add_vertex_to_facet_internal(facet_vertex_[i]);
    }
    end_facet_internal();
}

void MapBuilder::begin_facet_internal()
{
    transition(surface, facet);
    current_f_ = target_->new_facet();
    first_vertex_in_facet_.reset();
    current_v_.reset();
    first_halfedge_in_facet_ = nil;
    current_h_.reset();
}

void MapBuilder::end_facet_internal()
{
    transition(facet, surface);
    auto h =
        new_halfedge_between(current_v_, first_vertex_in_facet_);

    Map::make_sequence(current_h_, h);
    Map::make_sequence(h, first_halfedge_in_facet_);
}

void MapBuilder::add_vertex_to_facet_internal(std::shared_ptr<Vertex> v)
{
    transition(facet, facet);

    if (first_vertex_in_facet_ == nil)
    {
        first_vertex_in_facet_ = v;
    }
    else
    {
        auto new_halfedge =
            new_halfedge_between(current_v_, v);

        if (first_halfedge_in_facet_ == nil)
        {
            first_halfedge_in_facet_ = new_halfedge;
            Map::make_facet_key(first_halfedge_in_facet_);
        }
        else
        {
            Map::make_sequence(current_h_, new_halfedge);
        }
        current_h_ = new_halfedge;
    }
    current_v_ = v;
}

std::shared_ptr<Map::Halfedge> MapBuilder::find_halfedge_between(
    std::shared_ptr<Map::Vertex> from,
    std::shared_ptr<Map::Vertex> to)
{
    Star &star = star_->get(from->id);
    for (auto it = star.begin(); it != star.end(); it++)
    {
        auto cur = *it;
        if (cur->vertex() == to)
        {
            return cur;
        }
    }
    return nullptr;
}

void MapBuilder::add_vertex_to_facet(int i)
{
    if (i < 0 || i >= int(vertex_.size()))
    {
        if (!quiet_)
        {
            Logger::error("MapBuilder") << "vertex index "
                                        << i << " out of range"
                                        << std::endl;
        }
        return;
    }
    facet_vertex_.push_back(vertex_[i]);
    // 由于vertex的实际顺序就是按行读取的顺序，所以vertex_[i]的含义与obj文件定义的含义一致。
}

std::shared_ptr<Map::Vertex> MapBuilder ::copy_vertex(std::shared_ptr<Map::Vertex> v)
{
    auto new_v = target_->new_vertex();
    new_v->set_point(v->point());
    if (is_locked_->get(v->id))
    {
        is_locked_->set(new_v->id, true);
    }
    return new_v;
}

std::shared_ptr<Map::Halfedge> MapBuilder::new_halfedge_between(
    std::shared_ptr<Vertex> from,
    std::shared_ptr<Vertex> to)
{

    my_assert(from != to);

    // Non-manifold edges have been removed
    // by the higher level public functions.
    // Let us do a sanity check anyway ...
    my_assert(!find_halfedge_between(from, to));

    std::shared_ptr<Map::Halfedge> result = target_->new_halfedge();
    Map::set_halfedge_facet(result, current_f_);
    Map::set_halfedge_vertex(result, to);

    auto opposite = find_halfedge_between(to, from);
    if (opposite)
    {
        Map::make_opposite(result, opposite);
    }

    star_->get(from->id).push_back(result);
    Map::set_vertex_halfedge(to, result);

    return result;
}
