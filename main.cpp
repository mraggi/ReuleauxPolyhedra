#include "BuildBipartite.hpp"
#include "Graph.hpp"
#include "ReadFile.hpp"
#include "utility.hpp"
#include <iostream>
#include <sstream>

void tonto()
{
    Graph V(5);
    V.add_edge(0, 1);
    V.add_edge(0, 2);
    V.add_edge(0, 3);
    V.add_edge(0, 4);
    V.add_edge(1, 2);
    V.add_edge(2, 3);
    V.add_edge(3, 4);
    V.add_edge(4, 1);

    std::vector<Face> F = {{0, 1, 2}, {0, 1, 4}, {0, 2, 3}, {0, 3, 4}, {1, 2, 3, 4}};

    CSPSolver S(V, F);

    std::cout << S.F << std::endl;

    auto B = S.B;

    for (int v = 0; v < B.num_vertices()/2; ++v)
    {
        std::cout << v << ": ";
        for (auto u : B.neighborsX(v))
            std::cout << u << ", ";
        std::cout << std::endl;
    }

    S.ArcConsistency();
    std::cout << "sol = " << S.DFSSearch() << std::endl;
}

void tonto2()
{
    Graph V(7);
    V.add_edge(0, 1);
    V.add_edge(0, 2);
    V.add_edge(0, 4);
    V.add_edge(0, 5);
    V.add_edge(1, 2);
    V.add_edge(1, 4);
    V.add_edge(2, 3);
    V.add_edge(3, 4);
    V.add_edge(3, 6);
    V.add_edge(4, 5);
    V.add_edge(4, 6);
    V.add_edge(5, 6);

    std::vector<Face> F =
      {{0, 1, 2}, {0, 1, 4}, {0, 4, 5}, {4, 5, 6}, {3, 4, 6}, {0, 2, 3, 5, 6}, {1, 2, 3, 4}};

    CSPSolver S(V, F);

    auto B = S.B;

    for (int v = 0; v < B.num_vertices()/2; ++v)
    {
        std::cout << v << ": ";
        for (auto u : B.neighborsX(v))
            std::cout << u << ", ";
        std::cout << std::endl;
    }

    S.ArcConsistency();
    std::cout << "sol = " << S.DFSSearch() << std::endl;
}

void tonto3()
{
    Graph V(7);
    V.add_edge(0, 1);
    V.add_edge(0, 2);
    V.add_edge(0, 3);
    V.add_edge(1, 2);
    V.add_edge(1, 3);
    V.add_edge(1, 4);
    V.add_edge(2, 3);
    V.add_edge(2, 5);
    V.add_edge(3, 6);
    V.add_edge(4, 5);
    V.add_edge(4, 6);
    V.add_edge(5, 6);

    std::vector<Face> F =
      {{0, 1, 2}, {0, 1, 3}, {0, 2, 3}, {1, 2, 4, 5}, {1, 3, 4, 6}, {2, 3, 5, 6}, {4, 5, 6}};

    CSPSolver S(V, F);

    auto B = S.B;

    for (int v = 0; v < B.num_vertices()/2; ++v)
    {
        std::cout << v << ": ";
        for (auto u : B.neighborsX(v))
            std::cout << u << ", ";
        std::cout << std::endl;
    }

    S.ArcConsistency();
    
    B = S.B;

    for (int v = 0; v < B.num_vertices()/2; ++v)
    {
        std::cout << v << ": ";
        for (auto u : B.neighborsX(v))
            std::cout << u << ", ";
        std::cout << std::endl;
    }
    
    std::cout << "sol = " << S.DFSSearch() << std::endl;
}

void tonto4()
{
    Graph V(7);
    V.add_edge(0, 1);
    V.add_edge(0, 2);
    V.add_edge(0, 3);
    V.add_edge(0, 4);
    V.add_edge(0, 5);
    V.add_edge(0, 6);
    V.add_edge(1, 2);
    V.add_edge(2, 3);
    V.add_edge(3, 4);
    V.add_edge(4, 5);
    V.add_edge(5, 6);
    V.add_edge(6, 1);

    std::vector<Face> F =
      {{0, 1, 2}, {0, 1, 6}, {0, 2, 3}, {0, 3, 4}, {0, 4, 5}, {0, 5, 6}, {1, 2, 3, 4, 5, 6}};

    CSPSolver S(V, F);

    auto B = S.B;

    for (int v = 0; v < B.num_vertices()/2; ++v)
    {
        std::cout << v << ": ";
        for (auto u : B.neighborsX(v))
            std::cout << u << ", ";
        std::cout << std::endl;
    }

    S.ArcConsistency();
    std::cout << "sol = " << S.DFSSearch() << std::endl;
}

void tonto5()
{
    Graph V(8);
    V.add_edge(0, 1);
    V.add_edge(0, 2);
    V.add_edge(0, 7);
    V.add_edge(1, 3);
    V.add_edge(1, 5);
    V.add_edge(2, 4);
    V.add_edge(2, 6);
    V.add_edge(3, 4);
    V.add_edge(3, 5);
    V.add_edge(3, 7);
    V.add_edge(4, 6);
    V.add_edge(4, 7);
    V.add_edge(5, 7);
    V.add_edge(6, 7);

    std::vector<Face> F = {{0, 1, 2, 3, 4},
                           {0, 1, 5, 7},
                           {0, 2, 6, 7},
                           {1, 3, 5},
                           {2, 4, 6},
                           {3, 5, 7},
                           {4, 6, 7},
                           {3, 4, 7}};

    CSPSolver S(V, F);

    auto B = S.B;

    for (int v = 0; v < B.num_vertices()/2; ++v)
    {
        std::cout << v << ": ";
        for (auto u : B.neighborsX(v))
            std::cout << u << ", ";
        std::cout << std::endl;
    }

    S.ArcConsistency();
    std::cout << "sol = " << S.DFSSearch() << std::endl;
}

int siguiente(const std::vector<int>& vecinosDeB, int a)
{
    auto it = find(vecinosDeB.begin(), vecinosDeB.end(), a + 1);
    ++it;
    if (it == vecinosDeB.end())
        it = vecinosDeB.begin();
    return *it - 1;
}

std::pair<Graph, std::vector<Face>> extract_graph_and_faces_from_plantri(const std::vector<std::vector<int>>& A)
{
    int n = A.size();
    Graph G(n);
    std::vector<Face> F;
    for (int i = 0; i < n; ++i)
    {
        for (auto j : A[i])
        {
            --j;
            if (i < j)
                G.add_edge(i, j);

            Face f;
            f.push_back(i);
            f.push_back(j);
            int a = i;
            int b = j;
            int k = siguiente(A[b], a);
            while (k != i)
            {
                f.push_back(k);
                a = b;
                b = k;
                k = siguiente(A[b], a);
            }
            if (std::min_element(f.begin(), f.end()) == f.begin())
                F.emplace_back(f);
        }
    }
    
    return {G,F};
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <FILE>" << std::endl;
        return 1;
    }
    std::string file(argv[1]);
    
    auto T = read_file(file);

    std::stringstream ss;

    int total = 0;

    for (auto& A : T)
    {
        // 		std::cout << "Nueva grafica de " << G.size() << " vertices" <<
        // std::endl;
        auto GF = extract_graph_and_faces_from_plantri(A);

        CSPSolver S(GF.first, GF.second);

        total += S.PrintSolutions(ss);
    }
    std::cout << "# Total Embeddings" << std::endl;
    std::cout << total << std::endl;
    std::cout << ss.str() << std::endl;

    return 0;
}
