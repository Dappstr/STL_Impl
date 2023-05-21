template <typename iter, typename func>
func* for_each(iter start, iter end, func func_ptr) {
    for(; start != end; ++start) {
        func_ptr(*start);
    }
    func_ptr;
}

