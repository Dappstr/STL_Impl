#pragma once

#include <iostream>

namespace Dapp {
    template<typename T>
    class Smart_array_ptr {
        private:
            T* m_arr = nullptr;

        public:
            Smart_array_ptr(T* arr = nullptr)
                :m_arr(arr)
            {}

            Smart_array_ptr(const Smart_array_ptr<T>&) = delete;
            Smart_array_ptr<T>& operator =(const Smart_array_ptr<T>&) = delete;

            Smart_array_ptr(Smart_array_ptr<T>&& src) noexcept {
                if(this != &src) {
                    delete[] m_arr;
                    m_arr = src.m_arr;
                    src.m_arr = nullptr;
                }
            }

            Smart_array_ptr<T>& operator=(Smart_array_ptr<T>&& src) noexcept {
                if(this != &src) {
                    delete[] m_arr;
                    m_arr = src.m_arr;
                    src.m_arr = nullptr;
                }
                return *this;
            }

            inline T* get() const noexcept {
                return m_arr;
            }

            inline T& get_value() const noexcept {
                return *m_arr;
            }

            inline T& operator[](const size_t indx) const noexcept {
                return m_arr[indx];
            }

            inline T* operator->() const noexcept {
                return m_arr;
            }

           inline T& operator*() const noexcept {
                return *m_arr;
            }

            ~Smart_array_ptr() {
                if(m_arr) {
                    delete[] m_arr;
                }
            }
    };
}