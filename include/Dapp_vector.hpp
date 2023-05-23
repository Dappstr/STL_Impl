#include <cassert>
#include <cstdlib>
#include <cstring>
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
        auto* begin() const noexcept(noexcept(this->m_size > 0)) {
            assert(m_buffer != NULL);
            return &m_buffer[0];
        }
        const size_t capacity() const {return m_cap; } // Will return maximum current capacity
    
        Vector<T, N> clear() {
            Vector<T, m_size> ret_vec;
            memcpy(ret_vec.m_buffer, this->m_buffer, this->m_size);
            ret_vec.m_size = this->m_size;
            delete[] this->m_buffer;
            this->m_buffer = 0;
            return ret_vec;
        }
    
        const bool empty() { return m_size > 0; }

        auto* end() noexcept(noexcept(this->m_size > 0)) {
            assert(m_size >= 0);
            return &m_buffer[m_size-1];
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

        void shrink_to_fit(int n){
            assert(n > 0 && "Shrink value must be greater than 0");
            T new_buff = new T[n];
            memcpy(new_buff, this->m_buffer, n);
            delete[] this->m_buffer;
            this->m_buffer = new_buff;
            this->m_size = n;
        }
 
        const size_t size() const { return m_size; } // Will return current size
    
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

        Vector<T, N> operator+ (const Vector<T,N>& rhs) {
            Vector<T, N> new_vec;
            for(size_t i = 0; i < N; ++i) {
                new_vec.m_buffer[i] = this->m_buffer[i] + rhs.m_buffer[i];
            }

            return new_vec;
        }

        ~Vector() {
            delete[] m_buffer;
            m_size = 0;
            m_cap = 0;
            m_buffer = nullptr;
        }
};
