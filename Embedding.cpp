#include "Embedding.hpp"
#include "BuildBipartite.hpp"
#include "DifferentialEvolution.hpp"
#include "TimeHelpers.hpp"
#include "tqdm.hpp"
#include <cmath>
#include <iomanip>

using Individual = std::vector<Point3d>;
using Population = std::vector<Individual>;

struct DiffEvoParams
{
    int population_size{40};
    int num_epochs{400000};
    double force{0.5};
    double change_prob{0.5};
};

struct UnitDistanceGraph
{
    using vectors = std::vector<Point3d>;

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
        //         double sum_dist_sq = 0.0;
        int n = A.size();
        for (int i = 1; i < n; ++i)
        {
            for (int j = 0; j < i; ++j)
            {
                Point3d P = I[i];
                Point3d Q = I[j];
                double d = distancesq(P, Q);
                if (A[i][j])
                {
                    double r = d - 1.;
                    total += r*r;
                }
                else
                {
                    if (d < 0.05 || d > 0.95)
                        total += 10.;
                    //                     sum_dist_sq += d;
                }
            }
        }

        //         if (total < 1e-14)
        //             return -sum_dist_sq;

        return total;
    }

    void PhysicsMutator(Individual& p, int num_epochs) const
    {
        int N = p.size();

        vectors a(N);
        vectors v(N);

        for (int i = 0; i < num_epochs; ++i)
        {
            calculate_forces(p, a);
            simulate_time(p, v, a);
        }
    }

    void calculate_forces(const vectors& p, vectors& a) const
    {
        int n = A.size();

        constexpr double K = 1.; // spring constant

        for (int i = 1; i < n; ++i)
        {
            for (int j = 0; j < i; ++j)
            {
                Point3d P = p[i];
                Point3d Q = p[j];
                Point3d D = Q - P;
                double d = distance(P, Q);
                D /= d;
                if (A[i][j])
                {
                    double x = d - 1.;
                    Point3d force = K*x*D;
                    a[i] += force;
                    a[j] -= force;
                }
                else
                {
                    double x = d - 0.5;
                    Point3d force = K*x*D/10.;
                    a[i] += force;
                    a[j] -= force;
                }
            }
        }
    }

    void simulate_time(vectors& p, vectors& v, vectors& a) const
    {
        constexpr double time_step = 1e-3;
        int n = p.size();
        for (int i = 0; i < n; ++i)
        {
            v[i] += a[i]*time_step;
            v[i] *= 0.9999;
            p[i] += v[i]*time_step;
        }
        for (auto& aa : a)
            aa = Point3d(0., 0., 0.);
    }

    Matrix A;
};

void clamp(Point3d& p, double r)
{
    if (p.x < -r)
        p.x = -r;
    else if (p.x > r)
        p.x = r;

    if (p.y < -r)
        p.y = -r;
    else if (p.y > r)
        p.y = r;

    if (p.z < -r)
        p.z = -r;
    else if (p.z > r)
        p.z = r;
}

struct Normalize
{
    Normalize(const UnitDistanceGraph& u, Random& r) : U(u), R(r) {}
    void operator()(Individual& I)
    {
        //         if (R.random_real(0.,1.) < 0.01)
        //         {
        //             U.PhysicsMutator(I,1000);
        //         }

        for (auto& p : I)
        {
            clamp(p, 4.);
        }
    }

    const UnitDistanceGraph& U;
    Random& R;
};

std::vector<Point3d> FindUnitDistEmbedding(const Graph& G, const DiffEvoParams& P = DiffEvoParams())
{
    int n = G.num_vertices();
    UnitDistanceGraph U(G);

    //     std::vector<Point3d> result;
    //     double best_cost = 9999999.;

    for (int attempt = 0; attempt < 20; ++attempt)
    {
        auto population = Population(P.population_size);
        std::generate(population.begin(), population.end(), [n]() {
            return generate_random_points(n, 4.);
        });

        DifferentialEvolver D(population, U);

        Random R;

        auto pbar = tq::trange(P.num_epochs);

        int how_many_to_add = P.population_size;
        int add_every_nth = P.num_epochs/(2*how_many_to_add);

        Normalize normalizer(U, R);

        for (int epoch : pbar)
        {
            double prob = R.random_real(P.change_prob/2, P.change_prob*1.5);
            double force = R.random_real(P.force/2, P.force*1.5);

            D.step(prob, force, normalizer);

            pbar << "Best: " << D.best_cost << " at epoch " << epoch
                 << " with a population of: " << D.population_size();

            if (D.best_cost < 1.e-14)
                return D.best;

            if (epoch < P.num_epochs/2 && (epoch + 1)%add_every_nth == 0)
            {
                auto W = generate_random_points(n, 5.);
                if (R.random_real(0., 1.) < 0.5)
                    U.PhysicsMutator(W, 1000);
                D.insert_individual(W);
            }

            //             if (D.best_cost < best_cost)
            //             {
            //                 best_cost = D.best_cost;
            //                 result = D.best;
            //             }
        }
    }

    //     if (best_cost > 0)
    return std::vector<Point3d>();

    //     return result;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> args(argv, argv + argc);

    if (argc < 2)
    {
        std::cerr << "Usage: " << args[0] << " <FILE>" << std::endl;
        return 1;
    }

    auto ALL = ParseFile(args[1]);

    int start = 0;
    int end = ALL.size();

    if (argc == 4)
    {
        start = std::stoi(args[2]);
        end = std::stoi(args[3]);
    }

    std::cout << std::setprecision(15);

    for (int i = start; i < end; ++i)
    {
        GraphAndUnitDistanceGraph& GFU = ALL[i];
        std::cout << "# Original Graph " << i << " in [" << start << ", " << end << ")" << std::endl;
        std::cout << GFU.G << std::endl;

        std::cout << "# Unit Distance Graph " << i << std::endl;
        std::cout << GFU.U << std::endl;

        auto embedding = FindUnitDistEmbedding(GFU.U);

        if (embedding.empty())
        {
            std::cout << "# WARNING!!: failed to find good embedding for graph " << i << "!!!"
                      << std::endl;
        }
        else
        {
            UnitDistanceGraph U(GFU.U);
            double cost = U(embedding);
            std::cout << "# Embedding " << i << " (with cost " << cost << ")\n";
            std::cout << embedding << "\n\n\n";
        }

        std::cerr << "\nDone: " << i << " in range [" << start << ", " << end << ")" << std::endl;
    }

    return 0;
}
