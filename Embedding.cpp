#include "Embedding.hpp"
#include "BuildBipartite.hpp"
#include "DifferentialEvolution.hpp"
#include "TimeHelpers.hpp"
#include "tqdm.hpp"
#include <iomanip>
#include <cmath>

using Individual = std::vector<Point3d>;
using Population = std::vector<Individual>;

struct DiffEvoParams
{
    int population_size {40}; 
    int num_epochs {10000000};
    double force {0.5};
    double change_prob {0.3};
};

struct UnitDistanceGraph
{
    UnitDistanceGraph(const Graph& G) : A(G.num_vertices(), Row(G.num_vertices(), 0)) 
    {
        for (auto u : G.vertices())
        {
            for (auto v : G.neighbors(u))
            {
                A[u][v] = 1;
            }
        }
    }
    
    // loss function
    double operator()(const Individual& I) const
    {
        double total = 0.0;
        int n = A.size();
        for (int i = 1; i < n; ++i)
        {
            for (int j = 0; j < i; ++j)
            {
                Point3d P = I[i];
                Point3d Q = I[j];
                double d = distancesq(P,Q);
                if (A[i][j])
                {
                    double r = d - 1.;
                    total += r*r;
                } 
                else
                {
                    if (d < 0.1 || d > 0.95)
                        total += 10.;
                }
            }
        }
        return total;
    }
    
    Matrix A;
};

std::vector<Point3d> FindUnitDistEmbedding(const Graph& G, const DiffEvoParams& P = DiffEvoParams())
{
    int n = G.num_vertices();
    
    auto population = Population(P.population_size);
    std::generate(population.begin(), population.end(), [n](){ return generate_random_points(n, 10.); });
    std::cout << population[0] << std::endl;
    
    UnitDistanceGraph U(G);
    
    DifferentialEvolver D(population, U);
    
    Random R;
    
    auto pbar = tq::trange(P.num_epochs);
    
    for (int epoch : pbar)
    {
        double prob = R.random_real(P.change_prob/2, P.change_prob*1.5);
        double force = R.random_real(P.force/2, P.force*1.5);
        D.step(prob, force);
        pbar << "Best: " << D.best_cost;
        if (D.best_cost < 0.0000000000001)
            break;
    }
    
    return {};
}



int main(int argc, char* argv[])
{
    std::vector<std::string> args(argv, argv + argc);

    if (argc != 2)
    {
        std::cout << "Usage: " << args[0] << " <FILE>" << std::endl;
        return 1;
    }

    for (GraphAndUnitDistanceGraph& GFU : ParseFile(args[1]))
    {
        std::cout << "# Graph" << std::endl;
        std::cout << GFU.U << std::endl;
 
        auto embedding = FindUnitDistEmbedding(GFU.U);
        
        std::cout << "# Embedding" << std::endl;
        std::cout << embedding << std::endl;
//         break;
    }

    return 0;
}
