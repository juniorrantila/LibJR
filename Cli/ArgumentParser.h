#pragma once
#include <JR/ErrorOr.h>
#include <JR/Function.h>
#include <JR/Types.h>
#include <JR/Vector.h>
#include <stdio.h>

namespace Cli {

class ArgumentParser {
    struct Argument {
        c_string name;
        c_string description;
        Function<void()> on_used;
    };

    struct FallibleArgument {
        c_string name;
        c_string description;
        Function<ErrorOr<void>(StringView)> on_used;
    };

    struct PositionalArgument {
        c_string name;
        Function<ErrorOr<void>(StringView)> on_used;
    };

public:
    static ErrorOr<ArgumentParser> create()
    {
        return ArgumentParser {
            TRY(Vector<Argument>::create()),
            TRY(Vector<FallibleArgument>::create()),
            TRY(Vector<PositionalArgument>::create())
        };
    }

    constexpr void destroy() const
    {
        if (is_valid()) {
            m_arguments.destroy();
            m_fallible_arguments.destroy();
            m_positional_arguments.destroy();
        }
    }

    constexpr bool is_valid() const
    {
        return m_arguments.is_valid() && m_fallible_arguments.is_valid() && m_positional_arguments.is_valid();
    }


    ErrorOr<void> add_argument(string_literal argument,
        string_literal description,
        Function<void()>&& on_used);

    ErrorOr<void> add_argument_with_parameter(string_literal argument,
        string_literal description,
        Function<ErrorOr<void>(StringView)>&& on_used);

    ErrorOr<void> add_required_positional(string_literal name,
        Function<ErrorOr<void>(StringView)>&& on_used);

    ErrorOr<void> run(View<StringView> args) const;

    void show_help(FILE* = stdout) const;

private:
    ArgumentParser(Vector<Argument> arguments,
        Vector<FallibleArgument> fallible_arguments,
        Vector<PositionalArgument> positional_arguments)
        : m_arguments(arguments)
        , m_fallible_arguments(fallible_arguments)
        , m_positional_arguments(positional_arguments)
    {
    }

    Vector<Argument> m_arguments;
    Vector<FallibleArgument> m_fallible_arguments;
    Vector<PositionalArgument> m_positional_arguments;
};

}
