#pragma once

#include <random>

/* Copyright (c) 2018 Arvid Gerstmann. */
/* This code is licensed under MIT license. */
class pcg
{
public:
    using result_type = uint32_t;
    static constexpr result_type(min)() { return 0; }
    static constexpr result_type(max)() { return UINT32_MAX; }
    friend bool operator==(pcg const&, pcg const&);
    friend bool operator!=(pcg const&, pcg const&);

    explicit pcg(uint64_t seed0, uint64_t seed1 = 0x8fda36748fea9bULL)
    {
        m_state = 0;
        m_inc = (seed0 << 1) | 1;
        (void)operator()();
        m_state += seed1;
        (void)operator()();
    }

    result_type operator()()
    {
        uint64_t oldstate = m_state;
        m_state = oldstate*6364136223846793005ULL + m_inc;
        uint32_t xorshifted = uint32_t(((oldstate >> 18u)^oldstate) >> 27u);
        int rot = oldstate >> 59u;
        return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    }

    void discard(unsigned long long n)
    {
        for (unsigned long long i = 0; i < n; ++i)
            operator()();
    }

private:
    uint64_t m_state;
    uint64_t m_inc;
};

bool operator==(pcg const& lhs, pcg const& rhs)
{
    return lhs.m_state == rhs.m_state && lhs.m_inc == rhs.m_inc;
}
bool operator!=(pcg const& lhs, pcg const& rhs)
{
    return lhs.m_state != rhs.m_state || lhs.m_inc != rhs.m_inc;
}