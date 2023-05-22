#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdlib.h>

template <typename T, size_t N = 0>
class Vector
{
    private:
        size_t m_size = 0;
        size_t m_cap = N;
        T* m_buffer = nullptr;

    public:

        // Constructors
        Vector() {
            m_buffer = new T[N];
            m_size = N;
            m_cap = N;
        }

        Vector (size_t n) {
            m_buffer = new T[n];
            m_size = n;
            m_cap = n;
        }
        
        Vector(std::initializer_list<T> lst) {
            m_size = lst.size();
            m_cap = lst.size();
            m_buffer = new T[m_cap];
            std::copy(lst.begin(), lst.end(), this->begin());
        }
         
        // Utility functions
        const size_t size() const { return m_size; } // Will return current size
        const size_t capacity() const {return m_cap; } // Will return maximum current capacity
        const bool empty() { return m_size > 0; }

        auto* begin() const noexcept(noexcept(this->m_size > 0)) {
            assert(m_buffer != NULL);
            return &m_buffer[0];
        }
        
        auto* end() noexcept(noexcept(this->m_size > 0)) {
            assert(m_size >= 0);
            return &m_buffer[m_size-1];
        }
       
        void append(const T& val) {
            if (m_size == m_cap) {
                T new_buff = new T[m_size + 1];
                memcpy(new_buff, m_buffer, m_size);
                new_buff[m_size+1] = val;
                delete[] m_buffer;
                ++m_size;
                m_buffer = new_buff;
            }
            else {
                m_buffer[m_size-1] = val;
            }
        }


        auto pop() {
            assert(m_size > 0 && "Out of index error!\n");
            T popped = m_buffer[m_size-1];            
            --m_size;
            
            if(m_size > 0) {
                T* new_buffer = new T[m_size];
                memcpy(new_buffer, m_buffer, m_size * sizeof(T));
                delete[] m_buffer;
                m_buffer = new_buffer;
            }
            return popped;
        }

        // Operators
        friend std::ostream& operator << (std::ostream& out, Vector<T, N>& vec ) {
            for(int i = 0; i < vec.m_size; ++i) {
                out << vec.m_buffer[i];
            }
            return out;
        }
        T operator[](size_t indx) {
            return m_buffer[indx];
        }

        ~Vector() {
            delete[] m_buffer;
            m_size = 0;
            m_cap = 0;
            m_buffer = nullptr;
        }
};
