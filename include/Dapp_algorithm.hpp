//
// Created by laneb on 12/30/2024.
//

#ifndef DAPP_ALGORITHM_HPP
#define DAPP_ALGORITHM_HPP

namespace dapp {
    template <typename InputIt, typename OutputIt>
    OutputIt copy(InputIt first, InputIt last, OutputIt d_first) {
        while (first != last) {
            *d_first++ = *first++;
        }
        return d_first;
    }

    template <typename Ctr, typename T>
    size_t erase(Ctr& c, const T& value) {
        size_t removed_count = 0;

        auto it = c.begin();
        while (it != c.end()) {
            if (*it == value) {
                it = c.erase(it);
                ++removed_count;
            } else {
                ++it;
            }
        }

        return removed_count;
    }

    template <typename Ctr, typename Pred>
    size_t erase_if(Ctr& c, Pred pred) {
        size_t removed_count = 0;

        auto it = c.begin();
        while (it != c.end()) {
            if (pred(*it)) {
                it = c.erase(it);
                ++removed_count;
            } else {
                ++it;
            }
        }

        return removed_count;
    }

    template<typename Iter, typename T>
    void fill(Iter start, Iter end, const T& initvalue) {
        for(; start != end; ++initvalue) {
            *start = initvalue;
        }
    }

    template<typename Iter, typename Size, typename T>
    Iter fill_n(Iter start, const Size count, const T& value) {
        for(Size i = 0; i < count; ++i, ++start) {
            *start = value;
        }
        return start;
    }

    template<typename iter, typename func>
    func for_each(iter start, iter end, func fn) {
        for (; start != end; ++start) {
            fn(*start);
        }
        return fn;
    }

    template<typename iter, typename Size, typename func>
    func for_each_n(iter start, Size n, func fn) {
        for (Size i = 0; i < n; ++i, ++start) {
            fn(*start);
        }
        return fn;
    }

    template<typename Iter, typename T>
    Iter remove(Iter start, Iter end, T value) {
        Iter it = start;
        for(Iter s = start; s != end; ++s, ++it) {
            if(*it != value) {
                *it = *s;
                ++it;
            }
        }
        return it;
    }
    template<typename Iter, typename Pred>
    Iter remove_if(Iter start, Iter end, Pred p) {
        Iter it = start;
        for(Iter s = start; s != end; ++s, ++it) {
            if(!p(*it)) {
                *it = *s;
                ++it;
            }
        }
        return it;
    }

    template <typename Iter, typename T>
    void replace(Iter start, Iter end, const T old_value, const T new_value) {
        for(; start != end; ++start) {
            if(*start == old_value) {
                *start = new_value;
            }
        }
    }

    template <typename Iter, typename Pred, typename T>
    void replace_if(Iter start, Iter end, Pred pred, T new_value) {
        for(; start != end; ++start) {
            if(pred(*start)) {
                *start = new_value;
            }
        }
    }

    template<typename Iter>
    void sort(Iter start, Iter end) {
        auto swap = [](Iter a, Iter b) {
            auto temp = *a;
            *a = *b;
            *b = temp;
        };

        for (Iter i = start; i != end; ++i) {
            for (Iter j = i; j != start && *j < *(j - 1); --j) {
                swap(j, j - 1);
            }
        }
    }
}

#endif //DAPP_ALGORITHM_HPP
