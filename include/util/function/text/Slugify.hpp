#pragma once

#include <cctype>
#include <stdexcept>
#include <string>

namespace PDJE_UTIL::function {

struct SlugifyArgs {
    std::string input;
    bool        lowercase = true;
    char        separator = '-';
};

inline std::string
slugify(const SlugifyArgs &args)
{
    if (std::isalnum(static_cast<unsigned char>(args.separator))) {
        throw std::invalid_argument(
            "SlugifyArgs.separator must be a non-alphanumeric delimiter.");
    }

    std::string output;
    output.reserve(args.input.size());

    bool previous_was_separator = true;
    for (unsigned char ch : args.input) {
        if (std::isalnum(ch)) {
            output.push_back(args.lowercase
                                 ? static_cast<char>(std::tolower(ch))
                                 : static_cast<char>(ch));
            previous_was_separator = false;
        } else if (!previous_was_separator && !output.empty()) {
            output.push_back(args.separator);
            previous_was_separator = true;
        }
    }

    while (!output.empty() && output.back() == args.separator) {
        output.pop_back();
    }

    return output;
}

} // namespace PDJE_UTIL::function
