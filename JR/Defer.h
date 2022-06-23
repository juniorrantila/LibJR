#pragma once

template<typename F>
class Defer {
public:
    constexpr Defer(F lambda)
        : m_lambda(lambda)
    {
    }

    constexpr ~Defer()
    {
        m_lambda();
    }

private:
    F m_lambda;
};
