#include "CSPSolver.hpp"

bool operator<(const Edge& A, const Edge& B)
{
    if (A.u != B.u)
        return A.u < B.u;
    return A.v < B.v;
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

index_t first_minus_one(const std::vector<index_t>& P)
{
    return std::find(P.begin(), P.end(), -1L) - P.begin();
}
