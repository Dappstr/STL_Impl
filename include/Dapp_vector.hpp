#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <mutex>

template <typename T>
class Vector
{
private:
    size_t m_size = 0;
    size_t m_cap = 0;
    T* m_buffer = nullptr;
    std::mutex mtx {};

public:

    // Constructors
    Vector() {

        try {
            m_buffer = new T;
        }
        catch(const std::bad_alloc& e) {
            std::cerr << "Error allocating memory in " << __func__ << ": " << e.what();
            exit(EXIT_FAILURE);
        }

        //Set the initial size to be 0 and the capacity will be an optional template argument
        m_size = 0;
        m_cap = 0;
    }

    Vector(const Vector& v) {
        m_size = v.m_size;
        m_cap = v.m_cap;

        try {
            m_buffer = new T[m_cap];
            std::copy(v.m_buffer, v.m_buffer + m_size, m_buffer);
        }
        catch(const std::bad_alloc& e) {
            std::cerr << "Error allocating memory in " << __func__ << ": " << e.what();
            exit(EXIT_FAILURE);
        }
    }

    Vector(Vector&& v) noexcept {
        m_cap = std::move(v.m_cap);
        m_size = std::move(v.m_size);

        for(int i = 0; i < m_size; ++i) {
            this->m_buffer[i] = std::move(v.m_buffer[i]);
        }
    }

    Vector(const size_t n) {
        try {
            m_buffer = new T[n];
            m_size = 0;
            m_cap = n;
        }
        catch(const std::bad_alloc& e) {
            std::cerr << "Error allocating memory in " << __func__ << ": " << e.what();
            exit(EXIT_FAILURE);
        }
    }

    Vector(const size_t n, const T& val) {
        try {
            m_buffer = new T[n];
            m_cap = m_size = n;

            for(size_t i = 0; i < n; ++i) {
                m_buffer[i] = val;
            }
        }
        catch(const std::bad_alloc& e) {
            std::cerr << "Error allocating memory in " << __func__ << ": " << e.what();
            exit(EXIT_FAILURE);
        }

    }

    Vector(std::initializer_list<T> lst) {
        m_size = lst.size();
        m_cap = lst.size();

        try {
            m_buffer = new T[m_cap];
        }
        catch(const std::bad_alloc& e) {
            std::cerr << "Error allocating memory in " << __func__ << ": " << e.what();
            exit(EXIT_FAILURE);
        }
        std::copy(lst.begin(), lst.end(), this->begin());
    }

    // Utility functions

    //Resize the current buffer
    void resize(const size_t n) {
        assert(n >= 0 && "Cannot resize vector to size less than 0");

        if(n == 0) {
            delete[] m_buffer;
            m_cap = 0;
            m_size = 0;
        }
        else if (n != m_cap) {
            T* new_buffer;
            try {
                new_buffer = new T[n];
            }
            catch(const std::bad_alloc& e) {
                std::cerr << "Error allocating memory in " << __func__ << ": " << e.what();
                exit(EXIT_FAILURE);
            }

            size_t elements_to_copy = (n > m_size) ? n : m_size;

            for(size_t i = 0; i < elements_to_copy; ++i) {
                new_buffer[i] = m_buffer[i];
            }

            for(size_t i = m_size; i < n; ++i) {
                new_buffer[i] = T();
            }

            delete[] m_buffer;
            m_buffer = new_buffer;
            m_size = (n > m_size) ? m_size : n;
            m_cap = n;
        }
    }

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
            memcpy(new_buff, m_buffer, m_size * sizeof(T));

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

    void fill(const T& val) {
        //memset(m_buffer, val, sizeof(val) * this->m_cap);
        for(int i = 0; i < m_cap; ++i) {
            this->m_buffer[i] = val;
        }
        m_size = m_cap;
    }

    // Return the value at the given index
    T at(size_t indx) noexcept {
        assert(indx <= m_size);
        return *(&m_buffer[indx]);
    }

    decltype(auto) address_of(const int indx) noexcept {
        assert(indx <= m_size);
        return std::addressof(m_buffer[indx]);
    }

    //Point towards the beginning of the buffer assuming that the buffer has items.
    decltype(auto) begin() const noexcept(noexcept(this->m_size > 0)) {
        assert(m_buffer != NULL);
        return m_buffer;
    }

    // Returns a pointer to the end of the buffer
    decltype(auto) end() noexcept(noexcept(this->m_size > 0)) {
        assert(m_size >= 0);
        return m_buffer + m_size;
    }

    // Return the capacity of the buffer
    inline const size_t capacity() noexcept {return m_cap; } // Will return maximum current capacity

    // Will clear the contents of the buffer and return a buffer with the previous contents
    Vector<T> clear() {
        Vector<T> ret_vec(*this);

        memset(this->m_buffer, m_cap, 0);

        std::fill_n(m_buffer, m_size, T());
        return ret_vec;
    }

    // Returns whether the buffer is empty or not
    inline const bool empty() noexcept { return m_size > 0; }

    // Returns the last element and then reduces the size while preserving the capacity
    decltype(auto) pop() {
        assert(m_size > 0 && "Out of index error!\n");
        T popped = m_buffer[m_size-1];
        --m_size;

        if(m_size > 0) {
            T* new_buffer;
            try {
                new_buffer = new T[m_size];
                memcpy(new_buffer, m_buffer, m_size * sizeof(T));
                delete[] m_buffer;
                m_buffer = new_buffer;
            }
            catch(const std::bad_alloc& e) {
                std::cerr << "Could not allocate memory in " << __func__ << ": " << e.what();
                exit(EXIT_FAILURE);
            }
        }
        return popped;
    }

    // Will shrink the array down to size assuming n is greater than 0 as well as being lower than the current size while preserving the capacity.
    void shrink_to_fit(int n) {
        assert(n > 0 && "Shrink value must be greater than 0");
        assert(n < this->m_size && "Shrink value must be less than current size");

        T* new_buff;
        try {
            new_buff = new T[n];

            memcpy(new_buff, this->m_buffer, n);

            delete[] this->m_buffer;

            this->m_buffer = new_buff;
            this->m_size = n;
        }
        catch(const std::bad_alloc& e) {
            std::cerr << "Could not allocate memory in " << __func__ << ": " << e.what();
            exit(EXIT_FAILURE);
        }
    }

    // Returns size of the buffer
    inline const size_t size() & { return m_size; } // Will return current size
    inline const size_t size() && { return m_size; } // Will return current size

    // Operators
    Vector<T>& operator=(const Vector<T>& rhs) noexcept(noexcept(rhs.m_cap > -1)) {
        this->m_size = rhs.m_size;
        this->m_cap = rhs.m_cap;

        delete[] m_buffer;
        this->m_buffer = new T[this->m_cap];

        memcpy(this->m_buffer, rhs.m_buffer, this->m_size);

        return *this;
    }

    Vector<T>& operator=(Vector<T>&& rhs) noexcept(noexcept(rhs.m_cap > -1)) {
        this->m_cap = std::move(rhs.m_cap);
        this->m_size = std::move(rhs.m_size);


        delete[] this->m_buffer;

        try {
            this->m_buffer = new T[this->m_cap];
        }
        catch(const std::bad_alloc& e) {
            std::cerr << "Could not allocate memory in " << __func__ << ": " << e.what();
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < m_size; ++i) {
            this->m_buffer[i] = std::move(rhs.m_buffer[i]);
        }

        return *this;
    }

    Vector<T>& operator=(std::initializer_list<T> lst) {
        m_size = lst.size();
        m_cap = lst.size();
        try {
            m_buffer = new T[m_cap];
            std::copy(lst.begin(), lst.end(), this->begin());
        }
        catch(const std::bad_alloc& e) {
            std::cerr << "Could not allocate memory in " << __func__ << ": " << e.what();
            exit(EXIT_FAILURE);
        }
        return *this;
    }

    Vector<T>& operator++() {
        for(size_t i = 0; i < this->m_size; ++i) {
            m_buffer[i] += 1;
        }
        return *this;
    }

    Vector<T>& operator--() {
        for(size_t i = 0; i < this->m_size; ++i) {
            m_buffer[i] -= 1;
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const Vector<T>& vec ) {
        for(int i = 0; i < vec.m_cap; ++i) {
            out << vec.m_buffer[i] << ' ';
        }
        return out;
    }

    T& operator[](const size_t indx) & {
        return m_buffer[indx];
    }

    const T& operator[](const size_t indx) const& {
        return m_buffer[indx];
    }

    Vector<T> operator+(const Vector<T>& rhs) {
        size_t max_size = this->m_size > rhs.m_size ? this->m_size : rhs.m_size;

        Vector<T> new_vec(max_size);

        for(size_t i = 0; i < max_size; ++i) {
            new_vec[i] = this->m_buffer[i];
        }

        for(size_t i = 0; i < rhs.m_size; ++i) {
            if(i < this->m_size) {
                new_vec.m_buffer[i] += rhs.m_buffer[i];
            }
            else {
                new_vec.m_buffer[i] = rhs.m_buffer[i];
            }
        }

        new_vec.m_size = max_size;
        return new_vec;
    }

    void* operator new(size_t sz) {
        void* buffer = calloc(sz, 1);
        if(!buffer) { throw std::bad_alloc(); }
        return buffer;
    }
    void* operator new(size_t sz, const std::nothrow_t&) noexcept {
        void* buffer = calloc(sz, 1);
        return buffer;
    }

    void* operator new[](size_t sz) {
        void* buffer = calloc(sz, 1);
        if(!buffer) { throw std::bad_alloc(); }
        return buffer;
    }
    void* operator new[](size_t sz, const std::nothrow_t&) noexcept {
        void* buffer = calloc(sz, 1);
        return buffer;
    }

    void operator delete(void* ptr) noexcept {
        free(ptr);
    }
    void operator delete[](void* ptr) noexcept {
        free(ptr);
    }

    ~Vector() {
        if(m_cap < 1)
            delete m_buffer;
        else {
            delete[] m_buffer;
        }
        m_buffer = nullptr;

        m_size = 0;
        m_cap = 0;
    }
};
