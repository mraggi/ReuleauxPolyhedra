#include "CSPSolver.hpp"

bool operator<(const Edge& A, const Edge& B)
{
    if (A.a != B.a)
        return A.a < B.a;
    return A.b < B.b;
}

bool do_faces_intersect(const Face& AA, const Face& BB)
{
    Face A = AA;
    Face B = BB;
    std::sort(A.begin(), A.end());
    std::sort(B.begin(), B.end());
    Face intersection;
    std::set_intersection(A.begin(), A.end(), B.begin(), B.end(), std::back_inserter(intersection));
    return intersection.size() == 2;
}

bool is_there_valid_assignment(Vertex x, const Edge& e, const Graph& C, const BipartiteGraph& B)
{
    Vertex b = e.b;
    for (auto w : B.neighborsX(b))
    {
        if (w == x)
            continue;
        if (C.are_neighbors(x, w))
        {
            return true;
        }
    }
    return false;
}

std::ptrdiff_t first_minus_one(const std::vector<int>& P)
{
    return std::find(P.begin(), P.end(), -1) - P.begin();
}
