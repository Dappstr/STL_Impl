template <typename Iter, typename Pred>
void remove_if(Iter start, Iter end, Pred pred) {
    while(start != end) {
        if(pred(*start)) {
            decltype(auto) next = start;
            while(next + 1 != end) {
                *next = *(next + 1);
                ++next;
            }
            *next = 0;
            --end;
        }
        else {
            ++start;
        }
    }
}