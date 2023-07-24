#include <cassert>
#include <iostream>
#include <initializer_list>
#include <type_traits>

template <typename T, size_t N>
class Array
{
    private:
        T* m_buffer = nullptr;
        size_t m_size = N;

    public:
        Array(std::initializer_list<T> lst)
          :m_size(lst.size()) {
             m_buffer = new T[m_size];

             for(size_t i = 0; i < m_size; ++i) {
                m_buffer[i] = lst.at(i);
             }

             //Case for if type is char, must null terminate.
             if(std::is_same_v<T, char>) {
                m_buffer[m_size] = '\0';
             }
           }

        //Utility functions
        T at(size_t indx) noexcept(noexcept(indx > -1)) {
            assert(indx < m_size);
            
            return m_buffer[indx];
        }

        //Utility functions
        //TODO:
        //    Include utility functions
        //    Add operator overloads

        //Operator overloads
       
        ~Array() {
            m_size = 0;
            delete[] m_buffer;
        }
};
