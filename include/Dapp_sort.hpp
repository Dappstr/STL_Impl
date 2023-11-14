namespace Dapp {
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