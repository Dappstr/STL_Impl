//A heap based implementation of std::array
#pragma once

#include <initializer_list>
#include <iostream>
#include <initializer_list>
#include <type_traits>
#include <cassert>

namespace dapp {
    template<typename T, size_t N>
    class Array {
    private:
        T *m_buffer = nullptr;
        size_t m_size = N;

    public:
        Array(std::initializer_list<T> lst)
                : m_size(lst.size()) {
            m_buffer = new T[m_size];

            for (size_t i = 0; i < m_size; ++i) {
                m_buffer[i] = *(lst.begin() + i);
            }
        }

        Array(const Array& src)
                : m_size(src.m_size) {
            m_buffer = new T[m_size];

            for (int i = 0; i < m_size; ++i) {
                m_buffer[i] = src.m_buffer[i];
            }
        }

        //Utility functions
        T at(size_t indx) noexcept(noexcept(indx > -1)) {
            assert(indx < m_size);

            return m_buffer[indx];
        }

        T front() { return m_buffer; }

        T back() { return m_buffer + m_size; }

        size_t size() { return m_size; }

        bool empty() { return m_size < 1; }

        void fill(T value) {
            for (size_t i = 0; i < m_size; ++i)
                m_buffer[i] = value;
        }

        void clear() {
            for (size_t i = 0; i < m_size; ++i) {
                m_buffer[i] = 0;
            }
        }

        //Operator overloads1
        friend std::ostream &operator<<(std::ostream &out, const Array<T, N> &arr) {
            for (int i = 0; i < arr.m_size; ++i) {
                out << arr.m_buffer[i] << ' ';
            }
            out << '\n';
            return out;
        }

        T &operator[](size_t indx) noexcept(noexcept(indx > -1 && indx < m_size)) {
            assert(indx >= 0 && indx < m_size);
            return m_buffer[indx];
        }

        Array &operator=(const Array& src) {
            if (this == &src)
                return *this;

            assert(this->m_size == src.m_size && "Array sizes must be of equal length");

            delete[]  m_buffer;

            m_size = src.m_size;
            m_buffer = new T[m_size];
            memcpy(this->m_buffer, src.m_buffer, m_size);

            return *this;
        }

        ~Array() {
            m_size = 0;
            delete[] m_buffer;
        }
    };
}