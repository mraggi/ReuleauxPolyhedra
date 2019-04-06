#include "DifferentialEvolution.hpp"
#include "Graph.hpp"
#include "utility.hpp"
#include "BuildBipartite.hpp"

struct GraphAndDual
{
    GraphAndDual(Graph g, std::vector<Face> f, Graph d) : 
        G(std::move(g))
        , F(std::move(f))
        , D(std::move(d))
    {}
    
    Graph G;
    std::vector<Face> F;
    Graph D;
};

Graph read_graph(const std::vector<std::string>& data, int& curr_line, int n = -1, int m = -1)
{
    if (n == -1 || m == -1)
    {
        auto nm = split_line_into_ints(data[curr_line]);
        ++curr_line;
        n = nm[0];
        m = nm[1];
    }
    Graph G(n);
    for (int i = 0; i < m; ++i)
    {
        auto E = split_line_into_ints(data[curr_line]);
        G.add_edge(E[0],E[1]);
        ++curr_line;
    }
    
    return G;
}

std::vector<Face> read_faces(const std::vector<std::string>& data, int& curr_line, int n)
{
    std::vector<Face> F;
    for (int i = 0; i < n; ++i)
    {
        F.emplace_back(split_line_into_ints(data[curr_line]));
        ++curr_line;
    }
    
    return F;
}



std::vector<GraphAndDual> ParseFile(const std::string& filename)
{
    std::vector<GraphAndDual> result;
    
    std::vector<std::string> data = read_txt_file(filename, true);
    
    int total = split_line_into_ints(data[0])[0];
    
    int curr_line = 1;
    
    for (int t = 0; t < total; ++t)
    {
        auto G = read_graph(data, curr_line);
        auto F = read_faces(data, curr_line,G.num_vertices());
        auto D = read_graph(data, curr_line,G.num_vertices(),G.num_edges());
        result.emplace_back(G,F,D);
    }
    
    return result;
}

int main(int argc, char *argv[])
{
    std::vector<std::string> args(argv, argv+argc);
    
    if (argc != 2)
    {
        std::cout << "Usage: " << args[0] << " <FILE>" << std::endl;
        return 1;
    }
    
    
    
    for (GraphAndDual& GFD : ParseFile(args[1]))
    {
        std::cout << "Vertices, edges: " << GFD.G.num_vertices() << ", " << GFD.G.num_edges() << std::endl;
        std::cout << GFD.D << std::endl;
//         DifferentialEvolver D(Population InitialPopulation, Cost f);
    }
    
    
    return 0;
}
