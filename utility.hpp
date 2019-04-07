#pragma once

#include <fstream>
#include <istream>
#include <sstream>

using index_t = std::ptrdiff_t;

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& V)
{
    for (auto&& v : V)
        os << v << ' ';
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

inline std::vector<std::string> read_txt_file(const std::string& filename,
                                              bool discard_comments = false)
{
    std::vector<std::string> result;
    std::ifstream file(filename);
    std::string buffer;
    while (std::getline(file, buffer))
    {
        if (discard_comments)
        {
            auto comment_start = std::find(buffer.begin(), buffer.end(), '#');
            buffer.erase(comment_start, buffer.end());
        }

        if (buffer.size() > 0)
            result.emplace_back(buffer);
    }
    return result;
}

inline std::vector<int> split_line_into_ints(const std::string& line)
{
    std::istringstream is(line);
    std::vector<int> result;
    std::copy(std::istream_iterator<int>(is),
              std::istream_iterator<int>(),
              std::back_inserter(result));

    return result;
}

inline void replace_by_bigger(double& a, double b)
{
    if (a < b)
        a = b;
}

inline void replace_by_smaller(double& a, double b)
{
    if (a > b)
        a = b;
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
