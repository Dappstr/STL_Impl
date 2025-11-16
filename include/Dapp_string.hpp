#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring> // This imports strlen and strcpy functions
#include <iostream>
#include <mutex>
#include <new>
#include <stdlib.h>
#include <utility> // std::move

namespace dapp {
class String {
  private:
    // size_t m_size = 0;
    const static size_t SSO_CAP{16}; // 15 + a null terminator
    // char *m_buffer = nullptr;

    union {
        struct {
            char *m_buffer;
            size_t m_size{}, m_capacity;
        } Non_sso;

        struct {
            char m_ssoBuffer[SSO_CAP];
            size_t m_size{};
        } Sso;
    };

    bool m_isSSO = true;

    static const size_t npos = -1;

    inline char *data() noexcept { return m_isSSO ? Sso.m_ssoBuffer : Non_sso.m_buffer; }

    inline const char *data() const noexcept { return m_isSSO ? Sso.m_ssoBuffer : Non_sso.m_buffer; }

    inline size_t size() const noexcept { return m_isSSO ? Sso.m_size : Non_sso.m_size; }

    inline size_t capacity() const noexcept { return m_isSSO ? SSO_CAP : Non_sso.m_capacity; }

    inline void setSize(size_t new_size) noexcept {
        if (m_isSSO)
            Sso.m_size = new_size;
        else
            Non_sso.m_size = new_size;
    }

  public:
    inline static std::mutex mut;

    String() noexcept {
        m_isSSO = true;
        Sso.m_ssoBuffer[0] = '\0';
        Sso.m_size = 0;
    }

    String(const char *str) {
        const size_t len = strlen(str);

        if (len <= SSO_CAP) {
            m_isSSO = true;
            Sso.m_size = len;
            strcpy(Sso.m_ssoBuffer, str);
        } else {
            m_isSSO = false;
            Non_sso.m_size = len;
            Non_sso.m_capacity = len;
            try {
                Non_sso.m_buffer = new char[len + 1];
            } catch (const std::bad_alloc &e) {
                std::cerr << "Error allocating memory for string object: " << e.what();
                exit(EXIT_FAILURE);
            }
            strcpy(Non_sso.m_buffer, str);
        }
    }

    String(std::initializer_list<char> init_list) {
        const size_t list_size = init_list.size();

        if (list_size <= SSO_CAP) {
            m_isSSO = true;
            Sso.m_size = list_size;
            size_t index = 0;
            for (const auto ch : init_list) {
                Sso.m_ssoBuffer[index] = ch;
                ++index;
            }
            Sso.m_ssoBuffer[Sso.m_size] = '\0';
        } else {
            Non_sso.m_size = list_size;
            Non_sso.m_capacity = list_size;
            try {
                Non_sso.m_buffer = new char[list_size + 1];
            } catch (const std::bad_alloc &e) {
                std::cerr << "Error allocating memory for string object: " << e.what();
                exit(EXIT_FAILURE);
            }
            size_t indx = 0;
            for (const auto ch : init_list) {
                Non_sso.m_buffer[indx] = ch;
                ++indx;
            }
            Non_sso.m_buffer[Non_sso.m_size] = '\0';
        }
    }

    String(std::initializer_list<const char *> init_list) {
        size_t total_str_size = 0;
        // Calculate total size
        for (const auto &str : init_list) {
            total_str_size += strlen(str);
        }
        // Null terminator
        total_str_size += 1;

        if (total_str_size <= SSO_CAP) {
            char *buffer_ptr = Sso.m_ssoBuffer;
            for (const auto &str : init_list) {
                strcpy(buffer_ptr, str);
                buffer_ptr += strlen(str);
            }
            Sso.m_size = total_str_size;
        } else {
            Non_sso.m_size = total_str_size;
            try {
                Non_sso.m_buffer = new char[total_str_size];
            } catch (const std::bad_alloc &e) {
                std::cerr << "Error allocating memory for string object.";
                exit(EXIT_FAILURE);
            }
            char *buffer_ptr = Non_sso.m_buffer;
            for (const auto &str : init_list) {
                strcpy(buffer_ptr, str);
                buffer_ptr += strlen(str);
            }
        }
    }

    explicit String(const size_t sz, const char c) {
        if (sz <= SSO_CAP) {
            Sso.m_size = sz;
            for (size_t i = 0; i < sz; ++i) {
                Sso.m_ssoBuffer[i] = c;
            }
            Sso.m_ssoBuffer[sz] = '\0';
        } else {
            try {
                Non_sso.m_size = sz;
                Non_sso.m_capacity = sz;
                Non_sso.m_buffer = new char[sz + 1];
            } catch (const std::bad_alloc &e) {
                std::cerr << "Error allocating memory for string object.";
                exit(EXIT_FAILURE);
            }
            for (size_t i = 0; i < sz; ++i) {
                Non_sso.m_buffer[i] = c;
            }
            Non_sso.m_buffer[sz] = '\0';
        }
    }

    // Copy constructor
    String(const String &src) {
        if (src.m_isSSO) {
            Sso.m_size = src.Sso.m_size;
            strcpy(Sso.m_ssoBuffer, src.Sso.m_ssoBuffer);
        } else {
            Non_sso.m_size = src.Non_sso.m_size;
            Non_sso.m_capacity = src.Non_sso.m_capacity;
            try {
                Non_sso.m_buffer = new char[src.Non_sso.m_size + 1];
            } catch (const std::bad_alloc &e) {
                std::cerr << "Error allocating memory for string object.";
                exit(EXIT_FAILURE);
            }
            strcpy(Non_sso.m_buffer, src.Non_sso.m_buffer);
        }
    }

    // Move constructor
    String(String &&src) noexcept {
        if (src.m_isSSO) {
            Sso.m_size = src.Sso.m_size;
            strcpy(Sso.m_ssoBuffer, src.Sso.m_ssoBuffer);
            src.Sso.m_ssoBuffer[0] = '\0';
            src.Sso.m_size = 0;
        } else {
            Non_sso.m_size = src.Non_sso.m_size;
            Non_sso.m_capacity = src.Non_sso.m_capacity;
            Non_sso.m_buffer = std::move(src.Non_sso.m_buffer);
            src.Non_sso.m_size = 0;
            src.Non_sso.m_capacity = 0;
        }
    }

    [[nodiscard]] inline const size_t size() noexcept { return m_isSSO ? Sso.m_size : Non_sso.m_size; }

    [[nodiscard]] inline const bool empty() noexcept { return m_isSSO ? Sso.m_size < 1 : Non_sso.m_size < 1; }

    void clear() {
        if (m_isSSO) {
            Sso.m_ssoBuffer[0] = '\0';
            Sso.m_size = 0;
        } else {
            delete[] Non_sso.m_buffer;
            Non_sso.m_buffer = new char[1];
            Non_sso.m_buffer[0] = '\0';
            Non_sso.m_size = 0;
            Non_sso.m_capacity = 0;
        }
    }

    inline char *begin() noexcept {
        return m_isSSO ? Sso.m_ssoBuffer : Non_sso.m_buffer;
    } // Will point to beginning of buffer
    inline char *end() noexcept {
        return m_isSSO ? Sso.m_ssoBuffer + Sso.m_size : Non_sso.m_buffer + Non_sso.m_size;
    } // Will point to end of buffer (null terminator)

    // Will point to the character (address at) indx. This way if we wanted to modify the index using `at` instead of
    // the index operator, we can.
    inline const char &at(int indx) noexcept(noexcept(this->m_isSSO ? indx <= 15 : indx <= Non_sso.m_capacity)) {
        assert(indx < (m_isSSO ? Sso.m_size : Non_sso.m_size));
        if (m_isSSO) {
            return Sso.m_ssoBuffer[indx];
        } else {
            return Non_sso.m_buffer[indx];
        }
    }

    void append(const char *s) {
        std::lock_guard<std::mutex> lock(mut);

        size_t new_add_len = strlen(s);
        size_t new_size = size() + new_add_len;

        if (m_isSSO && new_size <= SSO_CAP) {
            memcpy(Sso.m_ssoBuffer + Sso.m_size, s, new_size + 1);
            Sso.m_size += new_add_len;
        } else {
            if (m_isSSO) {
                char tmp[SSO_CAP + 1];
                memcpy(tmp, Sso.m_ssoBuffer, Sso.m_size + 1);
                m_isSSO = false;
                Non_sso.m_size = Sso.m_size;
                Non_sso.m_capacity = std::max(new_size, Non_sso.m_size * 2);
                Non_sso.m_buffer = new char[Non_sso.m_capacity + 1];
                memcpy(Non_sso.m_buffer, tmp, Non_sso.m_size + 1);
            }
            if (new_size > Non_sso.m_capacity) {
                Non_sso.m_capacity = std::max(new_size, Non_sso.m_capacity * 2);
                char *new_buffer = new char[Non_sso.m_capacity + 1];
                memcpy(new_buffer, Non_sso.m_buffer, Non_sso.m_size);
                delete[] Non_sso.m_buffer;
                Non_sso.m_buffer = new_buffer;
            }
            memcpy(Non_sso.m_buffer + Non_sso.m_size, s, new_add_len + 1);
            Non_sso.m_size = new_size;
        }
    }

    void append(const String &src) {
        std::lock_guard<std::mutex> lock(mut);

        const char *src_buffer = src.m_isSSO ? src.Sso.m_ssoBuffer : src.Non_sso.m_buffer;
        const size_t src_size = src.m_isSSO ? src.Sso.m_size : src.Non_sso.m_size;
        const size_t old_size = m_isSSO ? Sso.m_size : Non_sso.m_size;
        const size_t new_size = old_size + src_size;

        if (m_isSSO && new_size <= SSO_CAP) {
            memcpy(Sso.m_ssoBuffer + old_size, src_buffer, src_size + 1);
            Sso.m_size = new_size;
            return;
        } else {
            append(src.data());
        }
    }

    void insert(size_t pos, const char *s) {
        std::lock_guard<std::mutex> lock(mut);
        assert(pos <= size() && "Index for insertion is out of range!");

        const size_t add_length = strlen(s);
        const size_t old_size = size();
        const size_t new_size = add_length + old_size;

        if (m_isSSO && new_size <= SSO_CAP) {
            memmove(Sso.m_ssoBuffer + pos + add_length, Sso.m_ssoBuffer + pos,
                    old_size - pos + 1); // Make room for inserted string
            memcpy(Sso.m_ssoBuffer + pos, s, add_length);
            Sso.m_size = new_size;
            return;
        }

        // Promotion to heap
        if (m_isSSO) {
            char temp[SSO_CAP + 1];
            memcpy(temp, Sso.m_ssoBuffer, Sso.m_size + 1);
            m_isSSO = false;
            Non_sso.m_size = Sso.m_size;
            Non_sso.m_capacity = std::max(new_size, Sso.m_size * 2);
            Non_sso.m_buffer = new char[Non_sso.m_capacity + 1];
            memcpy(Non_sso.m_buffer, temp, Sso.m_size + 1);
        }

        if (new_size > Non_sso.m_capacity) {
            Non_sso.m_capacity = std::max(new_size, Non_sso.m_capacity * 2);
            char *new_buffer = new char[Non_sso.m_capacity + 1];
            memcpy(new_buffer, Non_sso.m_buffer, pos);
            memcpy(new_buffer + pos + add_length, Non_sso.m_buffer + pos, old_size - pos + 1);
            delete[] Non_sso.m_buffer;
            Non_sso.m_buffer = new_buffer;
        } else {
            memmove(Non_sso.m_buffer + pos + add_length, Non_sso.m_buffer + pos, old_size - pos + 1);
        }
        memcpy(Non_sso.m_buffer + pos, s, add_length);
        Non_sso.m_size = new_size;
    }

    void insert(size_t pos, const String &s) {
        std::lock_guard<std::mutex> lock(mut);

        insert(pos, s.data());
    }

    void erase(size_t pos, size_t len) {
        std::lock_guard<std::mutex> lock(mut);
        assert(pos <= size() && "Erasure position is out of range.");

        if (pos + len > size()) {
            len = size() - pos;
        }
        char *buffer = data();
        const size_t old_size = size();
        memmove(buffer + pos, buffer + pos + len, old_size - pos - len + 1);
        setSize(old_size - len);
    }

    void erase() { clear(); }

    size_t find(const String &s, size_t pos = 0) const {
        if (s.size() == 0 || size() == 0) {
            return npos;
        }

        for (size_t i = pos; i + s.size() <= size(); ++i) {
            if (memcmp(data() + i, s.data(), s.size()) == 0) {
                return i;
            }
        }
        return npos;
    }

    size_t find(const char *s, size_t pos = 0) const {
        if (strlen(s) == 0 || size() == 0 || strlen(s) > size()) {
            return npos;
        }

        for (size_t i = pos; i + strlen(s) <= size(); ++i) {
            if (memcmp(data() + i, s, strlen(s)) == 0) {
                return i;
            }
        }
        return npos;
    }

    String substr(size_t pos = 0, size_t len = npos) const {
        assert(pos <= size() && " substr pos cannot be greater than size");

        const size_t correct_len = std::min(len, size() - pos);
        String result;

        if (correct_len <= SSO_CAP) {
            memcpy(result.Sso.m_ssoBuffer, data() + pos, correct_len);
            result.Sso.m_ssoBuffer[correct_len] = '\0';
            result.Sso.m_size = correct_len;
            result.m_isSSO = true;
        } else {
            result.m_isSSO = false;
            result.Non_sso.m_size = correct_len;
            result.Non_sso.m_capacity = correct_len;
            try {
                result.Non_sso.m_buffer = new char[correct_len + 1];
            } catch (const std::bad_alloc &e) {
                std::cerr << "Failed to allocate memory for substr string.";
                exit(EXIT_FAILURE);
            }
            memcpy(result.Non_sso.m_buffer, data() + pos, correct_len);
            result.Non_sso.m_buffer[correct_len] = '\0';
        }
        return result;
    }

    // Operators
    String &operator=(const String &rhs) {
        std::lock_guard<std::mutex> lock(mut);

        if (!m_isSSO) {
            delete[] Non_sso.m_buffer;
        }
        m_isSSO = rhs.m_isSSO;

        if (m_isSSO) {
            memcpy(Sso.m_ssoBuffer, rhs.Sso.m_ssoBuffer, rhs.Sso.m_size + 1);
            Sso.m_size = rhs.Sso.m_size;
        } else {
            Non_sso.m_size = rhs.Non_sso.m_size;
            Non_sso.m_capacity = rhs.Non_sso.m_capacity;
            Non_sso.m_buffer = new char[Non_sso.m_capacity + 1];
            memcpy(Non_sso.m_buffer, rhs.Non_sso.m_buffer, Non_sso.m_size + 1);
        }
        return *this;
    }

    String &operator=(String &&rhs) noexcept {
        std::lock_guard<std::mutex> lock(mut);

        if (!m_isSSO) {
            delete[] Non_sso.m_buffer;
        }
        m_isSSO = rhs.m_isSSO;

        if (rhs.m_isSSO) {
            memcpy(Sso.m_ssoBuffer, rhs.Sso.m_ssoBuffer, rhs.Sso.m_size + 1);
            Sso.m_size = rhs.Sso.m_size;
        } else {
            Non_sso.m_size = rhs.Non_sso.m_size;
            Non_sso.m_capacity = rhs.Non_sso.m_capacity;
            Non_sso.m_buffer = rhs.Non_sso.m_buffer;

            rhs.Non_sso.m_buffer = nullptr;
            rhs.Non_sso.m_capacity = 0;
            rhs.Non_sso.m_size = 0;
        }
        return *this;
    }

    String operator+(const char *rhs) {
        String new_str(*this);
        new_str.append(rhs);
        return new_str;
    }

    String operator+(const String &rhs) {
        String new_str(*this);
        new_str.append(rhs);
        return new_str;
    }

    char &operator[](size_t indx) & noexcept(noexcept(indx < (m_isSSO ? Sso.m_size : Non_sso.m_capacity))) {
        assert(indx < (m_isSSO ? Sso.m_size : Non_sso.m_size));
        return m_isSSO ? Sso.m_ssoBuffer[indx] : Non_sso.m_buffer[indx];
    }

    char operator[](size_t indx) && noexcept(noexcept(indx < (m_isSSO ? Sso.m_size : Non_sso.m_capacity))) {
        assert(indx < (m_isSSO ? Sso.m_size : Non_sso.m_size));
        return m_isSSO ? Sso.m_ssoBuffer[indx] : Non_sso.m_buffer[indx];
    }

    const bool operator<(const String &rhs) noexcept { return strcmp(data(), rhs.data()) < 0; }

    const bool operator<(const char *rhs) noexcept { return strcmp(data(), rhs) < 0; }

    bool operator==(const String &str) noexcept { return strcmp(data(), str.data()) == 0; }

    bool operator==(const char *str) noexcept { return strcmp(data(), str) == 0; }

    friend std::ostream &operator<<(std::ostream &out, const String &src) noexcept { return out << src.data(); }

    friend std::istream &operator>>(std::istream &in, String &dst) {
        constexpr size_t BUFFER_SIZE = 1024;
        char temp[BUFFER_SIZE];

        in >> temp;
        dst = String(temp);
        return in;
    }

    /*
        Example:
        std::fstream file("example.txt")
        String line;
        while(getline(file, line)) {
            ...
        }
    */
    friend std::istream &getline(std::istream &in, String &str) {
        constexpr size_t BUFFER_SIZE = 1024;
        char temp[BUFFER_SIZE];

        if (in.getline(temp, BUFFER_SIZE)) {
            str = String(temp);
        }

        return in;
    }

    operator const char *() const & { return m_isSSO ? Sso.m_ssoBuffer : Non_sso.m_buffer; }

    void *operator new(size_t size) {
        void *buffer = calloc(1, size);
        if (!buffer) {
            throw std::bad_alloc();
        }
        return buffer;
    }

    void *operator new(size_t size, const std::nothrow_t &) noexcept { return calloc(1, size); }

    void *operator new[](size_t size) {
        void *buffer = calloc(1, size);
        if (!buffer) {
            throw std::bad_alloc();
        };
        return buffer;
    }

    void *operator new[](size_t size, const std::nothrow_t &) noexcept { return calloc(1, size); }

    void operator delete(void *ptr) noexcept { free(ptr); }

    void operator delete[](void *ptr) noexcept { free(ptr); }

    ~String() {
        if (!m_isSSO && Non_sso.m_buffer) {
            delete[] Non_sso.m_buffer;
            Non_sso.m_buffer = nullptr;
        }
    }
};

String operator"" _S(const char *p, size_t) { return String{p}; }
} // namespace dapp
