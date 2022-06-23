#include <Cli/ArgumentParser.h>
#include <JR/SingleSplit.h>
#include <stdio.h>

namespace Cli {

ErrorOr<void> ArgumentParser::add_argument(string_literal argument,
    string_literal description,
    Function<void()>&& on_used)
{
    TRY(m_arguments.append({ argument, description, std::move(on_used) }));
    return {};
}

ErrorOr<void> ArgumentParser::add_argument_with_parameter(
    string_literal argument,
    string_literal description,
    Function<ErrorOr<void>(StringView)>&& on_used)
{
    TRY(m_fallible_arguments.append({ argument, description,
        std::move(on_used) }));
    return {};
}

ErrorOr<void> ArgumentParser::add_required_positional(
    string_literal argument,
    Function<ErrorOr<void>(StringView)>&& on_used)
{
    TRY(m_positional_arguments.append({ argument, std::move(on_used) }));
    return {};
}

ErrorOr<void> ArgumentParser::run(View<StringView> args) const
{
    for (let arg : args) {
        if (static bool first = true; first) {
            first = false;
            continue;
        }
        if (arg == "-h" || arg == "--help") {
            show_help();
            exit(0);
        }
    }
    var positionals_used = 0u;
    for (let arg : args) {
        if (static var first = true; first) {
            first = false;
            continue;
        }
        var matched_argument = false;
        if (arg.contains('=')) {
            let [match, value] = TRY(arg.split_on_first('='));
            for (let& check_against : m_fallible_arguments) {
                if (match == check_against.name) {
                    TRY(check_against.on_used(value));
                    matched_argument = true;
                }
            }
        }
        if (!matched_argument) {
            for (let& check_against : m_arguments) {
                if (arg == check_against.name) {
                    check_against.on_used();
                    matched_argument = true;
                }
            }
        }
        if (!matched_argument) {
            if (positionals_used < m_positional_arguments.size()) {
                let& positional = m_positional_arguments[positionals_used];
                TRY(positional.on_used(arg));
                matched_argument = true;
                positionals_used++;
            }
        }
        if (!matched_argument) {
            show_help(stderr);
            return Error::from_string_literal("unknown argument");
        }
    }
    return {};
}

void ArgumentParser::show_help(FILE* file) const
{
    fprintf(file, "USAGE: %s [OPTIONS] ", getenv("_"));
    for (let& argument : m_positional_arguments)
        fprintf(file, "%s ", argument.name);
    fprintf(file, "\n");
    fprintf(file, "OPTIONS:\n");
    for (let& argument : m_arguments)
        fprintf(file, "\t%s\t%s\n", argument.name, argument.description);
    for (let& argument : m_fallible_arguments)
        fprintf(file, "\t%s\t%s\n", argument.name, argument.description);
}

}
