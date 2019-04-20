
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
