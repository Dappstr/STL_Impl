#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <mutex>

template <typename T, size_t N = 0>
class Vector
{
    private:
        size_t m_size = 0;
        size_t m_cap = N;
        T* m_buffer = nullptr;
        std::mutex mtx {};

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
    
        //Append a value to the buffer
        void append(const T& val) {
            mtx.lock();
            
            //We check if the current size is at the current capacity
            //If it is, we need to allocate a new larger buffer for the new element
            if (m_size == m_cap) {
                size_t new_size = m_size + 1;

                //Create the new buffer
                T* new_buff = new T[new_size];

                //Copy over contents from old buffer (m_buffer) to the new temporary one
                memcpy(new_buff, m_buffer, m_size);
               
                //Assign the new element to its value
                new_buff[new_size - 1] = val;

                //Delete the current_buffer
                delete[] m_buffer;
                
                //Increase the size and transfer ownership to the new buffer
                m_size = new_size;
                m_cap = new_size;
                m_buffer = std::move(new_buff);
            }
            else {
                m_buffer[m_size] = val;
                ++m_size;
            }
            mtx.unlock();
        }
    
        // Return the value at the given index
        T at(size_t indx) {
            assert(indx <= m_size);
            return *(&m_buffer[indx]);
        }

        auto address_of(int indx) {
            assert(indx <= m_size); 
            return std::addressof(m_buffer[indx]);
        }

        //Point towards the beginning of the buffer assuming that the buffer has items.
        auto* begin() const noexcept(noexcept(this->m_size > 0)) {
            assert(m_buffer != NULL);
            return &m_buffer[0];
        }
    
        // Return the capacity of the buffer
        const size_t capacity() const {return m_cap; } // Will return maximum current capacity
    
        // Will clear the contents of the buffer and return a buffer with the previous contents
        Vector<T, N> clear() {
            Vector<T, this->m_size> ret_vec;
            memcpy(ret_vec.m_buffer, this->m_buffer, this->m_size);
            
            ret_vec.m_size = this->m_size;
            delete[] this->m_buffer;

            this->m_buffer = 0;
            
            return ret_vec;
        }
    
        // Returns whether the buffer is empty or not
        const bool empty() { return m_size > 0; }

        // Returns a pointer to the end of the buffer
        auto* end() noexcept(noexcept(this->m_size > 0)) {
            assert(m_size >= 0);
            return &m_buffer[m_size-1];
        }
       
        // Returns the last element and then reduces the size while preserving the capacity
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

        // Will shrink the array down to size assuming n is greater than 0 as well as being lower than the current size while preserving the capacity.
        void shrink_to_fit(int n){
            assert(n > 0 && "Shrink value must be greater than 0");
            assert(n < this->m_size && "Shrink value must be less than current size");
            
            T new_buff = new T[n];
            memcpy(new_buff, this->m_buffer, n);
            
            delete[] this->m_buffer;
            
            this->m_buffer = new_buff;
            this->m_size = n;
        }
 
        // Returns size of the buffer
        const size_t size() const { return m_size; } // Will return current size
    
        // Operators
        friend std::ostream& operator << (std::ostream& out, Vector<T, N>& vec ) {
            for(int i = 0; i < vec.m_size; ++i) {
                out << vec.m_buffer[i] << ' ';
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
