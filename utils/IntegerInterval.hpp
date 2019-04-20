#pragma once
#include <algorithm>
#include <boost/iterator/counting_iterator.hpp>
#include <cassert>
#include <vector>

// An integer interval is a closed-open interval of integers. For example,
// IntegerInterval(3,8) = {3,4,5,6,7}.
template <class IntType = int>
class IntegerInterval
{
public:
    static_assert(std::is_integral<IntType>::value, "Template parameter IntType must be integral");
    using value_type = IntType;
    using difference_type = std::ptrdiff_t;
    using size_type = difference_type;
    using iterator = boost::counting_iterator<IntType>;
    using const_iterator = iterator;

public:
    explicit IntegerInterval() = default;

    explicit IntegerInterval(IntType n) : last_(n)
    {
        if (last_ < 0)
            last_ = 0;
    }

    explicit IntegerInterval(IntType from, IntType to) : first_(from), last_(to)
    {
        if (last_ < first_) // empty interval
            last_ = first_;
    }

    size_type size() const { return last_ - first_; }
    IntType operator[](size_type i) const { return first_ + i; }

    iterator begin() const { return iterator(first_); }
    iterator end() const { return iterator(last_); }

private:
    IntType first_{0};
    IntType last_{0};
}; // end class IntegerInterval

using integer_interval = IntegerInterval<int>;
using big_integer_interval = IntegerInterval<std::int64_t>;

// Think of NN as the set of natural numbers.
template <class IntType>
auto NN(IntType n)
{
    return IntegerInterval<IntType>{n};
}

template <class IntTypeFrom, class IntTypeTo>
auto II(IntTypeFrom from, IntTypeTo to)
{
    using intt = std::common_type_t<IntTypeFrom, IntTypeTo>;
    return IntegerInterval<intt>{from, to};
}

template <class Container, class Index = std::ptrdiff_t>
auto indices(const Container& C)
{
    return IntegerInterval<Index>(C.size());
}
