#pragma once

#include "CSPSolver.hpp"
#include "utils/Graph.hpp"
#include "utils/Point3d.hpp"
#include "utils/Random.hpp"
#include "utils/utility.hpp"

struct GraphAndDiameterGraph
{
    GraphAndDiameterGraph(Graph g, std::vector<Face> f, Graph d)
        : G(std::move(g)), F(std::move(f)), D(std::move(d))
    {}

    Graph G;
    std::vector<Face> F;
    Graph D;
};

inline Graph read_graph(const std::vector<std::string>& data, int& curr_line, int n = -1, int m = -1)
{
    if (n == -1 || m == -1)
    {
        auto nm = split_line_into<int>(data[curr_line]);
        ++curr_line;
        n = nm[0];
        m = nm[1];
    }
    Graph G(n);
    for (int i = 0; i < m; ++i)
    {
        auto E = split_line_into<int>(data[curr_line]);
        G.add_edge(E[0], E[1]);
        ++curr_line;
    }

    return G;
}

inline std::vector<Face> read_faces(const std::vector<std::string>& data, int& curr_line, int n)
{
    std::vector<Face> F;
    for (int i = 0; i < n; ++i)
    {
        F.emplace_back(split_line_into<int>(data[curr_line]));
        ++curr_line;
    }

    return F;
}

inline std::vector<GraphAndDiameterGraph> ParseFile(const std::string& filename)
{
    std::vector<GraphAndDiameterGraph> result;

    std::vector<std::string> data = read_txt_file(filename, true);

    int total = split_line_into<int>(data[0])[0];

    int curr_line = 1;

    for (int t = 0; t < total; ++t)
    {
        auto G = read_graph(data, curr_line);
        auto F = read_faces(data, curr_line, G.num_vertices());
        auto D = read_graph(data, curr_line);
        result.emplace_back(G, F, D);
    }

    return result;
}

inline std::vector<Point3d> generate_random_points(int n, double t = 1.0)
{
    Random R;
    std::vector<Point3d> P(n);
    for (auto& p : P)
        p = Point3d(R.random_real(-t, t), R.random_real(-t, t), R.random_real(-t, t));

    return P;
}
