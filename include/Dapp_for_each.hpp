#pragma once

namespace dapp {
    template<typename iter, typename func>
    func for_each(iter start, iter end, func fn) {
        for (; start != end; ++start) {
            fn(*start);
        }
        return fn;
    }
}