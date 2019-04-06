#pragma once

template <class T>
struct Identity
{
    void operator()(T& t) {}
};
