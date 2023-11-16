#pragma once

namespace dapp {
    template <typename Iter, typename Pred, typename T>
    void replace_if(Iter start, Iter end, Pred pred, T new_value) {
        for(; start != end; ++start) {
            if(pred(*start)) {
                *start = new_value;
            }
        }
    }

}
