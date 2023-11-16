#include <type_traits>
#include <iterator>

template <typename Iter, typename T,
        typename = std::enable_if_t<
                    std::is_arithmetic<T>::value &&
                    std::is_arithmetic<typename std::iterator_traits<Iter>::value_type>::value>>
auto accumulate(Iter begin, Iter end, T start)
{
    T sum = start;
    for (; begin != end; ++begin) { // Corrected loop condition
        sum += *begin;
    }
    return sum;
}