#pragma once

namespace dapp {
    template<typename Iter, typename T>
    void iota(Iter start, Iter end, T val) {
        for (; start != end; ++start) {
            *start = val;
            ++val;
        }
    }
}