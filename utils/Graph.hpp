#pragma once

#include "IntegerInterval.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using Vertex = int;
using size_type = std::ptrdiff_t;
using index_type = std::ptrdiff_t;
using Row = std::vector<char>;
using Matrix = std::vector<Row>;

// simple graph
class Graph
{
public:
    explicit Graph(int num_vertices)
        : num_vertices_(num_vertices), graph_(num_vertices), A(num_vertices, Row(num_vertices, 0))
    {}

    explicit Graph(const std::vector<std::string>& names)
        : num_vertices_(names.size()), graph_(names.size())
    {}

    [[nodiscard]] size_type degree(Vertex a) const { return graph_[a].size(); }

    // Graph modification functions
    void add_edge(Vertex from, Vertex to)
    {
        graph_[from].emplace_back(to);
        graph_[to].emplace_back(from);
        A[from][to] = A[to][from] = 1;
        ++num_edges_;
        neighbors_sorted_ = false;
    }

    [[nodiscard]] bool are_neighbors(Vertex a, Vertex b) const { return static_cast<bool>(A[a][b]); }

    // Get Graph Info
    [[nodiscard]] size_type num_vertices() const { return num_vertices_; }
    [[nodiscard]] auto vertices() const { return NN(num_vertices_); }

    [[nodiscard]] size_type num_edges() const
    {
        return num_edges_;
    }

    [[nodiscard]] const std::vector<Vertex>& neighbors(Vertex n) const { return graph_[n]; }

    void sort_neighbors()
    {
        if (neighbors_sorted_)
            return;
        for (Vertex v : vertices())
        {
            sort(graph_[v].begin(), graph_[v].end());
        }
        neighbors_sorted_ = true;
    }

private:
    size_type num_vertices_;
    size_type num_edges_ {0};
    std::vector<std::vector<Vertex>> graph_;
    bool neighbors_sorted_{false};

    Matrix A;
};

inline std::ostream& operator<<(std::ostream& os, const Graph& G)
{
    os << G.num_vertices() << ' ' << G.num_edges() << '\n';
    for (Vertex u : G.vertices())
    {
        for (auto v : G.neighbors(u))
        {
            if (u < v)
                os << u << ' ' << v << '\n';
        }
    }
    return os;
}
