#pragma once

using index_t = std::ptrdiff_t;

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& rhs)
{
    os << "{";
    if (!rhs.empty())
    {
        auto it = rhs.begin();
        os << *it;
        ++it;
        for (; it != rhs.end(); ++it)
            os << "," << *it;
    }
    os << "}";
    return os;
}

inline void remove_from_vector(std::vector<Vertex>& V, Vertex x)
{
    auto it = find(V.begin(), V.end(), x);
    *it = V.back();
    V.pop_back();
}

template <class T>
bool belongs_to(const T& x, const std::vector<T>& X)
{
    return std::find(X.begin(), X.end(), x) != X.end();
}

// template <class T>
// std::ostream& operator<<(std::ostream& os, const vector<vector<T>>& A)
// {
// 	os << "{";
// 	for (const auto& x : A)
// 	{
// 		os << x << ",";
// 	}
// 	os << "}" << endl;
// 	return os;
// }
