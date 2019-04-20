#pragma once
#include <fstream>
#include <iostream>
#include <istream>
#include <vector>

std::vector<std::vector<std::vector<int>>> read_file(const std::string& filename)
{
    std::vector<std::vector<std::vector<int>>> result;
    std::streampos size;
    char* memblock;

    std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char[size];
        file.seekg(0, std::ios::beg);
        file.read(memblock, size);
        file.close();

        // 		int current=15;
        int current = 1;
        // 		for (int i = 0; i<size; i += 1)
        // 		{
        // 			cout << int(memblock[i]) << " ";
        // 		}
        // 		cout << endl << endl;
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
        delete[] memblock;
    }
    return result;
}
