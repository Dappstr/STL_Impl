/*
Manual implementation of a stack without the use of a container such as std::vector as a buffer
WIP
*/

#include <iostream>

template <typename T>
concept acceptableType =
                        std::is_move_constructible_v<std::decay_t<T>> &&
                        std::is_copy_constructible_v<std::decay_t<T>> && 
                        std::is_copy_assignable_v<std::decay_t<T>> &&
                        std::is_move_assignable_v<std::decay_t<T>>;

template <acceptableType T, size_t N = 0>
class Stack
{
    private:
        //std::vector<T> m_stack{};
        T* m_buffer {};
        size_t m_size{N};

    protected:
    void insertElement(const T& val) {
            ++m_size;
            T* tempBuff = new T[m_size];

            memcpy(tempBuff, m_buffer, m_size-1);
            tempBuff[m_size-1] = val;

            delete[] m_buffer;
            m_buffer = new T[m_size];

            //memcpy(m_buffer, tempBuff, m_size); Not working for some reason
            for(size_t i = 0; i < m_size; ++i) {
                m_buffer[i] = std::move(tempBuff[i]);
            }
            delete[] tempBuff;
        }

        void insertElement(T&& val) {
            ++m_size;
            T* tempBuff = new T[m_size];

            memcpy(tempBuff, m_buffer, m_size-1);
            tempBuff[m_size-1] = std::move(val);

            delete[] m_buffer;
            m_buffer = new T[m_size];

            //memcpy(m_buffer, tempBuff, m_size); Not working for some reason
            for(size_t i = 0; i < m_size; ++i) {
                m_buffer[i] = std::move(tempBuff[i]);
            }
            delete[] tempBuff;
        }

    public:
        //Default allocation for a stack of type T
        Stack() = default;

        Stack(T val) {
            assert(N > 0);
            m_buffer = new T[m_size];
            m_buffer[m_size-1] = val;
        }

        size_t size() { return m_size; }


        //default for now, implementing later    
        Stack(const Stack& src) = default;

        /*
        Finish later
        Stack(const Stack& src) noexcept(noexcept(m_size )){
        }*/

         //Default for now, implementing later    
        Stack(Stack&& src) = default;

        //Operators
        //Default for now, implementing later
        Stack& operator=(const Stack& src) = default;

        /*
        Stack& operator=(const Stack& src) {
            std::copy(src.m_stack.begin(), src.m_stack.end(), std::back_inserter(m_stack));
        }*/

        //Default for now, implementing later
        Stack& operator=(Stack&& src) = default;

        friend std::ostream& operator<<(std::ostream& out, C<T, N>& c) {
            for(size_t i = 0; i < c.m_size; ++i) {
                out << '[';
                if(i == c.m_size - 1) { out << c.m_buffer[i] << ']'; }
                else { out << c.m_buffer[i] << "], "; }
            }
            return out;
        }

        ~Stack() {
            delete[] m_buffer;
            m_size = 0;
        }
};
