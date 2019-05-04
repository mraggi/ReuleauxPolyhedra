#include "CSPSolver.hpp"
#include "utils/Graph.hpp"
#include "utils/ReadFile.hpp"
#include "utils/utility.hpp"
#include <iostream>
#include <sstream>

int siguiente(const std::vector<int>& vecinosDeB, int a)
{
    auto it = find(vecinosDeB.begin(), vecinosDeB.end(), a + 1);
    ++it;
    if (it == vecinosDeB.end())
        it = vecinosDeB.begin();
    return *it - 1;
}

std::pair<Graph, std::vector<Face>> extract_graph_and_faces_from_plantri(
  const std::vector<std::vector<int>>& A)
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

    return {G, F};
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <FILE>" << std::endl;
        return 1;
    }
    std::string file(argv[1]);

    auto T = read_plantri_file(file);

    std::stringstream ss;

    int total = 0;
    std::cout << "# Total number of 3-connected planar graphs: " << T.size() << std::endl;
    for (auto& A : T)
    {
        // 		std::cout << "Nueva grafica de " << G.size() << " vertices" <<
        // std::endl;
        auto GF = extract_graph_and_faces_from_plantri(A);

        CSPSolver S(GF.first, GF.second);

        total += S.PrintSolutions(ss);
    }

    std::cout << "# Total Embeddings\n";
    std::cout << total << '\n';
    std::cout << ss.str() << '\n';

    return 0;
}
