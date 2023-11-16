template <typename Iter, typename T>
void remove(Iter start, Iter end, T value) {
    while(start != end) {
        if(*start == value) {
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