#pragma once

#include "Graph.hpp"
#include "utility.hpp"

class BipartiteGraph
{
public:
    BipartiteGraph(Vertex x, Vertex y) : m_X(x), m_Y(y) {}

    [[nodiscard]] int degreeX(Vertex v) const { return m_X[v].size(); }

    [[nodiscard]] size_t num_verticesX() const { return m_X.size(); }

    [[nodiscard]] size_t num_verticesY() const { return m_Y.size(); }

    [[nodiscard]] size_t num_vertices() const { return num_verticesX() + num_verticesY(); }

    void remove_edge(Vertex u, Vertex v)
    {
        remove_from_vector(m_X[u], v);
        remove_from_vector(m_Y[v], u);
    }

    [[nodiscard]] const std::vector<std::vector<Vertex>>& X() const { return m_X; }

    [[nodiscard]] const std::vector<std::vector<Vertex>>& Y() const { return m_Y; }

    [[nodiscard]] const std::vector<Vertex>& neighborsX(Vertex a) const { return m_X[a]; }

    [[nodiscard]] const std::vector<Vertex>& neighborsY(Vertex a) const { return m_Y[a]; }

    void add_edge(Vertex x, Vertex y)
    {
        m_X[x].emplace_back(y);
        m_Y[y].emplace_back(x);
    }

    void sort_neighbors()
    {
        for (auto& x : m_X)
            std::sort(std::begin(x), std::end(x));

        for (auto& y : m_Y)
            std::sort(std::begin(y), std::end(y));

        m_neighbors_sorted = true;
    }

private:
    std::vector<std::vector<Vertex>> m_X;
    std::vector<std::vector<Vertex>> m_Y;
    bool m_neighbors_sorted{false};
};
