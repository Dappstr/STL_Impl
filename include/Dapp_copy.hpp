#pragma once

#include <concepts>
#include <type_traits>

namespace dapp {
#if __cplusplus == 202002L

    template <std::input_or_output_iterator InputIt, std::output_iterator<typename std::iterator_traits<InputIt>::value_type> OutputIt>
    requires std::copyable<typename std::iterator_traits<InputIt>::value_type>
    OutputIt copy(InputIt first, InputIt last, OutputIt d_first) {
        while (first != last) {
            *d_first++ = *first++;
        }
        return d_first;
    }

    #elif __cplusplus <= 201703L

    template <typename InputIt, typename OutputIt>
    std::enable_if_t<std::is_copy_assignable<typename std::iterator_traits<InputIt>::value_type>::value, OutputIt>
    copy(InputIt first, InputIt last, OutputIt d_first) {
        while (first != last) {
            *d_first++ = *first++;
        }
        return d_first;
    }
    #endif
}