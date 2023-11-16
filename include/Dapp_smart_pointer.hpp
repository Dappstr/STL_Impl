#pragma once

namespace dapp {
    template<typename T>
    class Smart_ptr {
    private:
        T* m_ptr = nullptr;

    public:
        Smart_ptr(T* ptr = nullptr) : m_ptr(ptr) {}

        //deleting copy constructor and copy assignment operator to replicate unique_ptr
        Smart_ptr(const Smart_ptr<T>&) = delete;
        Smart_ptr &operator=(const Smart_ptr<T>&) = delete;

        Smart_ptr(Smart_ptr<T>&& dst) {
            if (this != dst) {
                delete m_ptr;
                m_ptr = dst.m_ptr;
                dst.m_ptr = nullptr;
            }
            return *this;
        }

        Smart_ptr& operator=(Smart_ptr<T>&& dst) {
            if (this != dst) {
                delete m_ptr;
                m_ptr = dst.m_ptr;
                dst.m_ptr = nullptr;
            }
            return *this;
        }

        T& get_value() { return *m_ptr; }

        T& operator*() const { return *m_ptr; }

        T* get() { return m_ptr; }

        T* operator->() const { return m_ptr; }


        ~Smart_ptr() { delete m_ptr; }
    };
}