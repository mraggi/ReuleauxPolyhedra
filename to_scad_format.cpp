#include "Embedding.hpp"
#include "utils/TimeHelpers.hpp"
#include "utils/tqdm.hpp"
#include <cassert>
#include <cmath>
#include <iomanip>

std::pair<int, int> get_opposite_edge(const Graph& U, int a, int b)
{
    auto neighA = U.neighbors(a);
    auto neighB = U.neighbors(b);
    std::sort(neighA.begin(), neighA.end());
    std::sort(neighB.begin(), neighB.end());

    std::vector<int> result;
    std::set_intersection(neighA.begin(),
                          neighA.end(),
                          neighB.begin(),
                          neighB.end(),
                          std::back_inserter(result));
    assert(result.size() == 2);
    return {result[0], result[1]};
}

inline std::vector<Point3d> read_points(const std::vector<std::string>& data, int& curr_line)
{

    std::string line = data[curr_line++];
    line = with_char_removed(line, '(');
    line = with_char_removed(line, ',');
    line = with_char_removed(line, ')');

    auto X = split_line_into<double>(line);
    int n = X.size()/3;

    std::vector<Point3d> result(n);

    for (int i = 0, j = 0; i < n; ++i, j += 3)
    {
        result[i].x = X[j];
        result[i].y = X[j + 1];
        result[i].z = X[j + 2];
    }

    return result;
}

void center_points(std::vector<Point3d>& P)
{
    Point3d center;
    for (auto& p : P)
        center += p;
    center /= double(P.size());
    for (auto& p : P)
        p -= center;
}

int main(int argc, char* argv[])
{
    using namespace std::string_literals;
    std::vector<std::string> args(argv, argv + argc);

    if (argc < 3)
    {
        std::cerr << "Usage: " << args[0] << " <FILE> <OUTPUTFILE>" << std::endl;
        return 1;
    }

    auto txt_file = read_txt_file(args[1], true);
    std::string outfilename = args[2];

    int curr_line = 0;
    int i = -1;
    while (curr_line < txt_file.size())
    {
        ++i;
        auto G = read_graph(txt_file, curr_line);
        auto U = read_graph(txt_file, curr_line);
        auto P = read_points(txt_file, curr_line);
        
        center_points(P);
        
        std::string ofn = outfilename + std::to_string(i) + ".scad"s;
        
        std::ofstream outfile(ofn); 
        
        if (!outfile)
            std::cerr << "ERROR: Cannot open file " << ofn << " for writing." << std::endl;
        
        outfile << "$fn=240;\n"
                  << "use <lib.scad>;\n"
                  << "r=100;\n"
                  << "pts=r*[";
        for (auto p : P)
        {
            outfile << '[' << p.x << ", " << p.y << ", " << p.z << "], ";
        }
        outfile << "];\n\n";
        outfile << "ayo=[";
        for (auto a : G.vertices())
        {
            for (auto b : G.neighbors(a))
            {
                if (a > b)
                    continue;
                auto opposite = get_opposite_edge(U, a, b);
                if (a > opposite.first || a > opposite.second)
                    continue;
                outfile << "[" << a << ", " << b << ", " << opposite.first << ", "
                          << opposite.second << "], ";
            }
        }
        outfile << "];\n\n";

        outfile << "anchoconstante(pts,r,ayo);\n\n" << std::endl;
    }
    return 0;
}
