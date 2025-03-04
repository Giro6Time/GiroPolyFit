#pragma once
#include "common.h"
#include "basic_types.h"
#include <memory>

class Map;
class MapMutator;
namespace MapTypes
{
    class Vertex;
    class Halfedge;
    class Facet;
    class MapMutator;

    /// @brief Combinatorial Element
    class GIROPOLYFIT_API Combel
    {
    public:
        Combel();
        ~Combel();
    };

    class GIROPOLYFIT_API Vertex : public Combel, public std::enable_shared_from_this<Vertex>
    {
    public:
        Vertex() : halfedge_(nil) {}
        Vertex(const Math::vec3 &p) : halfedge_(nil), point_(p) {}
        ~Vertex() { halfedge_ = nil; }

        const Math::vec3 &point() const { return point_; }
        Math::vec3 &point() { return point_; }
        void set_point(const Math::vec3 &p) { point_ = p; }

        /// @return 指向当前顶点的halfedge
        std::shared_ptr<Halfedge> halfedge() const { return halfedge_; }

        bool is_valid() const;
        void assert_is_valid() const;

        /// @brief 计算顶点的度数
        /// @note 度数实际为入度或出度，一组对立的halfedge提供一个度
        unsigned int degree() const;
        bool is_on_border() const;
        /// @brief 检查两个是否与顶点v连通
        bool is_connected(const std::shared_ptr<Vertex> v) const;

    protected:
        void set_halfedge(std::shared_ptr<Halfedge> h) { halfedge_ = h; }
        friend class ::Map;
        friend class ::MapMutator;

    private:
        std::shared_ptr<Halfedge> halfedge_;
        Math::vec3 point_;
    };

    class GIROPOLYFIT_API Halfedge : public Combel, std::enable_shared_from_this<Halfedge>
    {
    public:
        Halfedge() : opposite_(nil), next_(nil),
                     prev_(nil), facet_(nil), vertex_(nil)
        {
        }
        ~Halfedge()
        {
            opposite_ = nil;
            next_ = nil;
            prev_ = nil;
            facet_ = nil;
            vertex_ = nil;
        }

        std::shared_ptr<Halfedge> opposite() const { return opposite_; }
        std::shared_ptr<Halfedge> next() const { return next_; }
        std::shared_ptr<Halfedge> prev() const { return prev_; }

        std::shared_ptr<Halfedge> next_around_vertex() const
        {
            return opposite()->prev();
        }
        std::shared_ptr<Halfedge> prev_around_vertex() const
        {
            return next()->opposite();
        }

        std::shared_ptr<Facet> facet() const { return facet_; }
        std::shared_ptr<Vertex> vertex() const { return vertex_; }

        bool is_border() const { return facet_ == nil; }
        bool is_border_edge() const
        {
            return is_border() || opposite()->is_border();
        }

        /// @brief 每个面有一个半边为key
        bool is_facet_key() const;

        /// @brief 每个顶点有一个半边为key
        bool is_vertex_key() const;

        /// @brief 每个半边有一个半边为key
        /// @note 此方法可用于环路处理，以便逐个遍历每条边的一半（例如，用于网格绘制）。
        bool is_edge_key() const;
        std::shared_ptr<Halfedge> edge_key()
        {
            return is_edge_key() ? shared_from_this() : opposite();
        }

        bool is_valid() const;
        void assert_is_valid() const;

    protected:
        void set_opposite(std::shared_ptr<Halfedge> h) { opposite_ = h; }
        void set_next(std::shared_ptr<Halfedge> h) { next_ = h; }
        void set_prev(std::shared_ptr<Halfedge> h) { prev_ = h; }
        void set_facet(std::shared_ptr<Facet> f) { facet_ = f; }
        void set_vertex(std::shared_ptr<Vertex> v) { vertex_ = v; }

        friend class ::Map;
        friend class ::MapMutator;

    private:
        std::shared_ptr<Halfedge> opposite_;
        std::shared_ptr<Halfedge> next_;
        std::shared_ptr<Halfedge> prev_;
        std::shared_ptr<Facet> facet_;
        std::shared_ptr<Vertex> vertex_;
    };

    class GIROPOLYFIT_API Facet : public Combel, public std::enable_shared_from_this<Facet>
    {
    public:
        Facet() : halfedge_(nil) {}
        ~Facet() { halfedge_ = nil; }

        std::shared_ptr<Halfedge> halfedge() const { return halfedge_; }

        int degree() const;
        int nb_edges() const { return degree(); }
        int nb_vertices() const { return degree(); }

        bool is_on_border() const;
        bool is_triangle() const;
        bool is_valid() const;

        void assert_is_valid() const;

    protected:
        void set_halfedge(std::shared_ptr<Halfedge> h) { halfedge_ = h; }
        friend class ::Map;
        friend class ::MapMutator;

    private:
        std::shared_ptr<Halfedge> halfedge_;
    };

    inline bool Halfedge::is_facet_key() const
    {
        return (facet_->halfedge() == shared_from_this());
    }
    inline bool Halfedge::is_vertex_key() const
    {
        return (vertex_->halfedge() == shared_from_this());
    }

    inline bool Halfedge::is_edge_key() const
    {
        return (shared_from_this() < opposite_);
    }
}