#include <cassert>
#include <iostream>
#include <memory>

template <typename T, size_t N = 0>
class Vector
{
    private:
        size_t m_size = N;
        size_t m_cap = N;
        T* m_buffer = nullptr;

    public:

        // Constructors
        Vector() {
            m_buffer = new T[sizeof(T)];
            m_size = N;
        }

        Vector (size_t n){ m_buffer = new T[n]; }

        // Utility functions
        const size_t size() const { return m_size; } // Will return current size
        const size_t capacity() const {return m_cap; } // Will return maximum current capacity
        const bool empty() { return m_size > 0; }

        auto* begin() {
            assert(std::addressof(m_buffer != NULL));
            return &m_buffer[0];
        }
        
        auto* end() {
            assert(m_size >= 0);
            return &m_buffer[m_size-1];
        }
        
        // Operators
        friend std::ostream& operator << (std::ostream& out, Vector<T, N>& vec ) {
            for(int i = 0; i < vec.m_size; ++i) {
                out << vec.m_buffer[i];
            }
            return out;
        }

        ~Vector() {
            delete[] m_buffer;
            m_size = 0;
            m_cap = 0;
            m_buffer = nullptr;
        }
};
