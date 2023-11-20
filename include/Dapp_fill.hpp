#pragma once

namespace dpap {
    template<typename Iter, typename T>
    void fill(Iter start, Iter end, T initvalue) {
        for(; start != end; ++initvalue) {
            *start = initvalue;
        }
    }
}