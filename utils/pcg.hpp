#pragma once

#include <random>

/* Copyright (c) 2018 Arvid Gerstmann. */
/* This code is licensed under MIT license. */
/* This code was lightly modified from the original */

class pcg
{
public:
    using result_type = uint32_t;
    static constexpr result_type(min)() { return 0; }
    static constexpr result_type(max)() { return UINT32_MAX; }

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
        uint32_t xorshifted = (((oldstate >> 18u)^oldstate) >> 27u);
        int rot = oldstate >> 59u;
        return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    }

    void discard(uint64_t n)
    {
        for (uint64_t i = 0; i < n; ++i)
            operator()();
    }

private:
    uint64_t m_state;
    uint64_t m_inc;
};
