#pragma once

namespace dapp {
    template <typename Iter, typename T>
    void replace(Iter start, Iter end, const T old_value, const T new_value) {
        for(; start != end; ++start) {
            if(*start == old_value) {
                *start = new_value;
            }
        }
    }
}
