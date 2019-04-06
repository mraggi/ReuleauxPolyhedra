#pragma once

#include "BipartiteGraph.hpp"
// #include "utility.hpp"
#include "utility.hpp"
#include <algorithm>
#include <cassert>
#include <set>
#include <sstream>

using Face = std::vector<Vertex>;

struct BlackEdge
{
    BlackEdge(Vertex a, Vertex b) : a(a), b(b) {}
    Vertex a;
    Vertex b;
};

bool operator<(const BlackEdge& A, const BlackEdge& B);

bool do_faces_intersect(const Face& A, const Face& B);

bool is_there_valid_assignment(Vertex x, const BlackEdge& e, const Graph& C, const BipartiteGraph& B);

index_t first_minus_one(const std::vector<int>& P);

struct CSPSolver
{
    CSPSolver(const Graph& v, const std::vector<Face>& f)
        : n(f.size()), V(v), F(f), C(f.size()), B(f.size(), f.size())
    {
        assert(n == V.num_vertices());
        ConstructFaceGraph();
        ConstructBipartiteGraph();
    }

    void ConstructFaceGraph()
    {
        for (index_t j = 1; j < n; ++j)
        {
            for (index_t i = 0; i < j; ++i)
            {
                if (do_faces_intersect(F[i], F[j]))
                {
                    C.add_edge(i, j);
                }
            }
        }
    }

    void ConstructBipartiteGraph()
    {
        for (Vertex v : V.vertices())
        {
            for (int i = 0; i < n; ++i)
            {
                if (V.degree(v) != F[i].size())
                    continue;

                // TODO: check list of neighbour degrees? Maybe its faster.

                if (!belongs_to(v, F[i]))
                {
                    B.add_edge(v, i);
                }
            }
        }
    }

    std::set<BlackEdge> SetOfAllEdges(const Graph& G)
    {
        std::set<BlackEdge> result;
        for (Vertex u : V.vertices())
        {
            for (Vertex v : V.neighbors(u))
            {
                result.emplace(u, v);
            }
        }
        return result;
    }

    BlackEdge pop_first(std::set<BlackEdge>& X)
    {
        BlackEdge result = *X.begin();
        X.erase(X.begin());
        return result;
    }

    void ArcConsistency()
    {
        std::set<BlackEdge> BlackEdgesToProcess = SetOfAllEdges(V);

        while (!BlackEdgesToProcess.empty())
        {
            BlackEdge e = pop_first(BlackEdgesToProcess);

            ProcessBlackEdge(e, BlackEdgesToProcess);
        }
    }

    void ProcessBlackEdge(const BlackEdge& e, std::set<BlackEdge>& BlackEdgesToProcess)
    {
        for (Vertex x : B.neighborsX(e.a))
        {
            if (!is_there_valid_assignment(x, e, C, B))
            {
                B.remove_edge(e.a, x);
                for (Vertex v : V.neighbors(e.a))
                {
                    BlackEdgesToProcess.emplace(v, e.a);
                }
            }
        }
    }

    using partial_sol = std::vector<int>;
    bool IsCompletionValid(const partial_sol& P, int v, int c)
    {
        for (int i = 0; i < n; ++i)
        {
            int j = P[i];
            if (j == -1)
                continue;

            if (V.are_neighbors(v, i) != C.are_neighbors(c, j))
                return false;
        }

        for (auto vi : F[c])
        {
            int w = P[vi];
            if (w != -1)
            {
                if (!belongs_to(v, F[w]))
                    return false;
            }
        }

        return true;
    }

    std::vector<partial_sol> PossibleFillings(const partial_sol& P, int v)
    {
        assert(P[v] == -1);
        std::vector<partial_sol> resultados;
        for (auto u : B.neighborsX(v))
        {
            if (IsCompletionValid(P, v, u))
            {
                auto Q = P;
                Q[v] = u;
                resultados.push_back(Q);
            }
        }

        return resultados;
    }

    std::vector<partial_sol> DFSSearch()
    {
        std::vector<partial_sol> solutions;

        partial_sol Q(n, -1);

        // set the ones we know their value for sure.
        for (Vertex v : V.vertices())
        {
            if (B.degreeX(v) == 1)
            {
                auto c = B.neighborsX(v)[0];
                if (IsCompletionValid(Q,v,c))
                    Q[v] = c;
                else
                    return solutions;
            }
        }

        // standard DFS search
        std::vector<partial_sol> frontier;
        frontier.push_back(Q);

        while (!frontier.empty())
        {
            auto P = frontier.back();
            frontier.pop_back();

            index_t t = first_minus_one(P);
            if (t == n)
            {
                solutions.push_back(P);
                continue; // TODO: return solutions
            }

            for (auto x : PossibleFillings(P, t))
                frontier.push_back(x);
        }

        return solutions;
    }

    // returns number of solutions
    int PrintSolutions(std::stringstream& ss)
    {
        int num = 0;
        ArcConsistency();
        auto solutions = DFSSearch();
        if (solutions.size() > 0)
        {
            for (auto& P : solutions)
            {
                ss << "# Original graph" << '\n';
                ss << V.num_vertices() << ' ' << V.num_edges() << '\n';
                
                for (Vertex u : V.vertices())
                {
                    for (Vertex v : V.neighbors(u))
                    {
                        if (u < v)
                            ss << u << ' ' << v << '\n';
                    }
                }
                
                ss << "# Faces (in order)\n";
                for (Vertex v : V.vertices())
                {
                    ss << F[P[v]] << '\n';
                }
                
//                 ss << "caras[" << num << "] = " << F << ";" << std::endl;
//                 ss << "sol[" << num << "] = " << P << ";" << std::endl;
//                 ss << "aristadual[" << num << "] = {";
                ss << "# Unit distance graph\n";

                for (Vertex u : V.vertices())
                {
                    for (Vertex v : F[P[u]])
                    {
                        if (u < v)
                        {
                            ss << u << ' ' << v << '\n';
                        }
                    }
                }
                ss << '\n';

                ++num;
            }
        }
        return num;
    }

    index_t n;

    Graph V; // original graph

    std::vector<Face> F; // faces of V.

    Graph C; // dual

    // neighbors of v in B are the possible values that v could take
    BipartiteGraph B;
};
