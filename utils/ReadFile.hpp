#pragma once
#include <fstream>
#include <iostream>
#include <istream>
#include <vector>

inline std::vector<std::vector<std::vector<int>>> read_plantri_file(const std::string& filename)
{
    std::vector<std::vector<std::vector<int>>> result;
    std::streampos size;

    std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        std::vector<char> memblock(size);
        file.seekg(0, std::ios::beg);
        file.read(&memblock[0], size);
        file.close();

        int current = 1;
        while (current < size)
        {
            int n = memblock[current];
            current += 2;
            std::vector<std::vector<int>> G(n);
            for (int i = 0; i < n; ++i)
            {
                while (memblock[current] != 0)
                {
                    G[i].push_back(memblock[current]);
                    current += 2;
                }
                current += 2;
            }
            ++current;
            result.push_back(G);
        }
    }
    return result;
}
