#include "Embedding.hpp"
#include "TimeHelpers.hpp"
#include "tqdm.hpp"
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

int main(int argc, char* argv[])
{
    std::vector<std::string> args(argv, argv + argc);

    if (argc < 2)
    {
        std::cerr << "Usage: " << args[0] << " <FILE>" << std::endl;
        return 1;
    }

    auto txt_file = read_txt_file(args[1], true);

    int i = 0;
    while (i < txt_file.size())
    {
        auto G = read_graph(txt_file, i);
        auto U = read_graph(txt_file, i);
        auto P = read_points(txt_file, i);

        std::cout << "$fn=240;\n"
                  << "use <lib.scad>;\n"
                  << "r=100;\n"
                  << "pts=r*[";
        for (auto p : P)
        {
            std::cout << '[' << p.x << ", " << p.y << ", " << p.z << "], ";
        }
        std::cout << "\b\b];\n\n";
        std::cout << "ayo=[";
        for (auto a : G.vertices())
        {
            for (auto b : G.neighbors(a))
            {
                if (a > b)
                    continue;
                auto opposite = get_opposite_edge(U, a, b);
                std::cout << "[" << a << ", " << b << ", " << opposite.first << ", "
                          << opposite.second << "], ";
            }
        }
        std::cout << "\b\b];\n\n";

        std::cout << "anchoconstante(pts,r,ayo);\n\n" << std::endl;
    }
    return 0;
}
