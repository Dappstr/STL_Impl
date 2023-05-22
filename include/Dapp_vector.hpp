#include <iostream>

template <typename T, size_t N = 0>
class Vector
{
    private:
        size_t m_size = N;
        size_t m_cap = N;
        T* m_buffer = nullptr;

    public:
        Vector() {
            m_buffer = new T[sizeof(T)];
            m_size = N;
        }

        Vector (size_t n){ m_buffer = new T[n]; }

        const size_t size() const { return m_size; } // Will return current size
        const size_t cap() const {return m_cap; } // Will return maximum current capacity

        ~Vector() {
            delete[] m_buffer;
            m_size = 0;
            m_cap = 0;
            m_buffer = nullptr;
        }
};
