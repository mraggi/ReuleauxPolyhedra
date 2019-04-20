#pragma once

#include "utils/BipartiteGraph.hpp"
#include "utils/utility.hpp"
#include <algorithm>
#include <cassert>
#include <set>
#include <sstream>

using Face = std::vector<Vertex>;

// using Edge = Graph::Edge;

struct Edge
{
    Edge(Vertex a, Vertex b) : u(a), v(b) {}
    Vertex u;
    Vertex v;
};

bool operator<(const Edge& A, const Edge& B);

bool do_faces_intersect(const Face& A, const Face& B);

bool is_there_valid_assignment(Vertex x, const Edge& e, const Graph& DualG, const BipartiteGraph& B);

index_t first_minus_one(const std::vector<int>& P);

class CSPSolver
{
public:
    CSPSolver(const Graph& v, const std::vector<Face>& f)
        : n(f.size()), G(v), F(f), DualG(f.size()), B(f.size(), f.size())
    {
        assert(n == G.num_vertices());
        ConstructFaceGraph();
        ConstructBipartiteGraph();
    }

private:
    void ConstructFaceGraph()
    {
        for (index_t j = 1; j < n; ++j)
        {
            for (index_t i = 0; i < j; ++i)
            {
                if (do_faces_intersect(F[i], F[j]))
                {
                    DualG.add_edge(i, j);
                }
            }
        }
    }

    void ConstructBipartiteGraph()
    {
        for (Vertex v : G.vertices())
        {
            for (int i : indices(F))
            {
                if (G.degree(v) == F[i].size() && !belongs_to(v, F[i]))
                {
                    B.add_edge(v, i);
                }
            }
        }
    }

    std::set<Edge> SetOfAllEdges(const Graph& G)
    {
        std::set<Edge> result;
        for (Vertex u : G.vertices())
        {
            for (Vertex v : G.neighbors(u))
            {
                result.emplace(u, v);
            }
        }
        return result;
    }

    void ArcConsistency()
    {
        std::set<Edge> EdgesToProcess = SetOfAllEdges(G);

        while (!EdgesToProcess.empty())
        {
            Edge e = pop_first(EdgesToProcess);

            ProcessEdge(e, EdgesToProcess);
        }
    }

    void ProcessEdge(const Edge& e, std::set<Edge>& EdgesToProcess)
    {
        for (Vertex x : B.neighborsX(e.u))
        {
            if (!is_there_valid_assignment(x, e))
            {
                B.remove_edge(e.u, x);
                for (Vertex v : G.neighbors(e.u))
                {
                    EdgesToProcess.emplace(v, e.u);
                }
            }
        }
    }

    using partial_sol = std::vector<int>;
    bool IsAssignmentValid(const partial_sol& P, int v, int f)
    {
        for (int i = 0; i < n; ++i)
        {
            int j = P[i];
            if (j == -1)
                continue;

            if (G.are_neighbors(v, i) != DualG.are_neighbors(f, j))
                return false;
        }

        for (auto vi : F[f])
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
        std::vector<partial_sol> results;
        for (auto u : B.neighborsX(v))
        {
            if (IsAssignmentValid(P, v, u))
            {
                auto Q = P;
                Q[v] = u;
                results.push_back(Q);
            }
        }

        return results;
    }

    bool is_there_valid_assignment(Vertex x, const Edge& e)
    {
        for (auto w : B.neighborsX(e.v))
        {
            if (w != x && DualG.are_neighbors(x, w))
                return true;
        }
        return false;
    }

    std::vector<partial_sol> DFSSearch()
    {
        std::vector<partial_sol> solutions;

        partial_sol Q(n, -1);

        // set the ones we know their value for sure.
        for (Vertex v : G.vertices())
        {
            if (B.degreeX(v) == 1)
            {
                auto face = B.neighborsX(v)[0];
                if (IsAssignmentValid(Q, v, face))
                    Q[v] = face;
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
                // TODO: replace by return solutions, if claim that if there is a
                // solution it is unique, turns out to be true.
                continue;
            }

            for (auto x : PossibleFillings(P, t))
                frontier.push_back(x);
        }

        return solutions;
    }
    
public:
    // returns number of solutions
    int PrintSolutions(std::stringstream& ss)
    {
        int num = 0;
        ArcConsistency();
        auto solutions = DFSSearch();
        for (auto& tau : solutions)
        {
            ss << "# Original graph" << '\n';
            ss << G << '\n';

            ss << "# Faces (sorted by isomorphism)\n";
            for (Vertex v : G.vertices())
            {
                ss << F[tau[v]] << '\n';
            }

            ss << "\n# Diameter graph\n";
            ss << G.num_vertices() << ' ' << G.num_edges() << '\n';

            for (Vertex u : G.vertices())
            {
                for (Vertex v : F[tau[u]])
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
        return num;
    }

    index_t n;

    Graph G; // original graph
    std::vector<Face> F; // faces of G.
    Graph DualG; // dual of G

    // neighborsX of v in B are the possible values that v could take
    BipartiteGraph B;
};
