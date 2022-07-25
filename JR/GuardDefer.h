#pragma once

template<typename F>
class GuardDefer {
public:
    constexpr GuardDefer(F lambda)
        : m_lambda(lambda)
    {
    }

    constexpr ~GuardDefer()
    {
        if (m_guard)
            m_lambda();
    }

    constexpr void disarm()
    {
        m_guard = false;
    }

private:
    F m_lambda;
    bool m_guard { true };
};
