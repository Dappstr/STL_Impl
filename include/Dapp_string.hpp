#pragma once 

#include <iostream>
#include <cstring> // This imports strlen and strcpy functions
#include <utility> // std::move
#include <cassert>
#include <stdlib.h>
#include <mutex>

class String
{
    private:
        size_t m_size = 0;
        char* m_buffer = nullptr;
        static const size_t npos = -1;
        inline static std::mutex mut;

    public:
        //TODO
        // Implement core string functionality (constructors, destructors)
       explicit String() {
           m_buffer = new char[1];
           m_buffer[0] = '\0';
           m_size = 1;
       }
        
       //Explicit constructor to only allow either empty construction, or construction of strings
       //This disallows users from passing something like an integer or double as a constructor parameter
       String(const char* str)
           :m_size(strlen(str)) { 
           m_buffer = new char[m_size + 1];
           strcpy(m_buffer, str);
       }

       String(std::initializer_list<char> init_list)
           :m_size (init_list.size()) {
               m_buffer = new char[m_size];
               std::copy(init_list.begin(), init_list.end(), m_buffer); 
               m_buffer[m_size] = '\0';
           }

       //Copy constructor
       String(const String& src)
           :m_size(src.m_size) {
           m_buffer = new char[m_size];
           strcpy(m_buffer, src.m_buffer);
       }

       //Move constructor
       String(String&& src)
           :m_size(std::move(src.m_size)) {
           m_buffer = std::move(src.m_buffer); 
           
            //We set the source string to have no value.
            //This could differ from the STL implementation of string where after moving, the object is in a valid but undefined state, however,
            //I feel as though we should be able to define the state clearly, which is that it is empty
           src.m_buffer = nullptr;
           src.m_size = 0;
       }

        inline size_t size() { return m_size; }
        inline bool empty() { return m_size > 0 ? false : true; }

        //Will zero every element in the string, but retain the size
        //Assuming that the m_size member is greater than 0 in order to not underflow when setting the null terminator
        void clear() {
            assert(m_size > 0 && "Error! empty() requires size greater than 0"); 
            delete[] m_buffer;

            m_buffer = new char[1];
            m_size = 1;
            m_buffer[m_size-1] = '\0';
        }
       
        inline const char* begin() { return &m_buffer[0]; } // Will point to beginning of buffer
        inline const char* end() { return &m_buffer[m_size]; } // Will point to end of buffer (null terminator)

        //Will point to the character (address at) indx. This way if we wanted to modify the index using `at` instead of the index operator, we can.
        inline const char* at(int indx) noexcept(noexcept(indx < m_size)) { return &m_buffer[indx]; }
        
        void append(const char* s) {
            std::lock_guard<std::mutex> lock(mut);
            
            size_t new_size = m_size + strlen(s) + 1;
            char* new_buffer = new char[new_size+1];
            
            strcpy(new_buffer, m_buffer);
            strcat(new_buffer, s);
            
            new_buffer[new_size] = '\0';
            
            delete[] this->m_buffer;
            m_buffer = new char[new_size];
            strcpy(this->m_buffer, new_buffer);
            
            delete[]new_buffer;
            m_size = new_size;
        }
        
        void append(const String& src) {
            std::lock_guard<std::mutex> lock(mut);

            size_t new_size = m_size + strlen(src.m_buffer);
            char* new_buffer= new char[new_size+1];
            
            strcpy(new_buffer, m_buffer);
            strcat(new_buffer, src.m_buffer);
            
            new_buffer[new_size] = '\0';

            delete[] this->m_buffer;
            this->m_buffer = new char[m_size+1];
            strcpy(m_buffer, new_buffer);
            
            delete[] new_buffer;
            m_size = new_size;
        }
        
        void insert(size_t pos, const char* s) {
            std::lock_guard<std::mutex> lock(mut);
            
            size_t new_size = m_size + strlen(s);
            char* new_buffer = new char[new_size+1];
            
            strncpy(new_buffer, m_buffer, pos);
            new_buffer[pos] = '\0';

            strcat(new_buffer, s);
            strcat(new_buffer, m_buffer + pos);
        
            delete[] m_buffer;
            m_buffer = new char[new_size+1];
            strcpy(m_buffer, new_buffer);
            
            m_buffer[new_size] = '\0';
            m_size = new_size;
        }

        void insert(size_t pos, const String& s) { 
            std::lock_guard<std::mutex> lock(mut);

            size_t new_size = m_size + s.m_size;
            char* new_buffer = new char[new_size+1];

            strncpy(new_buffer, m_buffer, pos);
            new_buffer[pos] = '\0';

            strcat(m_buffer, s.m_buffer);
            strcat(new_buffer, m_buffer + pos);

            delete[] m_buffer;
            m_buffer = new char[new_size+1];
            
            strcpy(m_buffer, new_buffer);
            m_buffer[new_size] = '\0';
            m_size = new_size;
        }
        
        void erase(size_t pos, size_t len) {
            assert(pos + len <= m_size && "Length must be less than or equal to the string length");
            
            size_t new_size = m_size - len;
            char* new_buffer = new char[new_size+1];
            
            strncpy(new_buffer, m_buffer, pos); // Copy characters up to (not including) `pos`
            strcpy(new_buffer + pos, m_buffer + pos  + len); //Then copy the characters starting at pos, then up to length

            delete[] m_buffer;
            m_size = new_size;
            strcpy(m_buffer, new_buffer);
            m_buffer[m_size] = '\0';
        }

        void erase() {
            clear();
        }

	  size_t find(const String& s, size_t pos = 0) const {
            if (m_size < s.m_size) {
                return npos;
            }

            if (pos < m_size) {
                char first_char = s.m_buffer[0];

                for (size_t i = pos; i < m_size; ++i) {
                    if (m_buffer[i] == first_char) {
                        String str_cmp; // String used for comparison
                        strncpy(str_cmp.m_buffer, this->m_buffer + i, s.m_size);
                        str_cmp.m_buffer[s.m_size] = '\0';

                        if (strcmp(str_cmp.m_buffer, s.m_buffer) == 0) {
                            return i;
                        }
                    }
                }
            }
            return npos;
        }

        size_t find(const char* s, size_t pos = 0) const {
            size_t s_len = strlen(s);

            if (m_size < s_len) {
                return npos;
            }

            if (pos < m_size) {
                char first_char = s[0];
                for (size_t i = pos; i < m_size; ++i) {
                    if (m_buffer[i] == first_char) {
                        char str_cmp[s_len + 1]; // Char array used for comparison
                        strncpy(str_cmp, this->m_buffer + i, s_len);
                        str_cmp[s_len] = '\0';

                        if (strcmp(str_cmp, s) == 0) {
                            return i;
                        }
                    }
                }
            }
            return npos;
        }

        String substr(size_t pos = 0, size_t len = npos) const {
            assert(pos <= this->m_size && " substr pos cannot be greater than size");
            
            char* sub_str = new char [len+1];
            sub_str = (char*)memcpy(sub_str, this->m_buffer+pos, len);
            sub_str[len] = '\0';

            String ret_str(sub_str);
            delete[] sub_str;
            return ret_str;
        }
        

        //Operators
        String& operator=(const String& rhs) {
            std::lock_guard<std::mutex> lock(mut);

            if(this == &rhs) { return *this; }
            delete[] m_buffer;
            
            m_size = rhs.m_size;
            m_buffer = new char[m_size+1];
            
            memcpy(m_buffer, rhs.m_buffer, rhs.m_size);
            m_buffer[m_size] = '\0'; 
            
            return *this;
        }
        
        String& operator=(String&& rhs) {
            std::lock_guard<std::mutex> lock(mut); 

            if(this == &rhs) { return *this; }
            delete[] m_buffer;

            m_size = rhs.m_size;
            strcpy(m_buffer, rhs.m_buffer);
            
            rhs.m_size = 0;
            rhs.m_buffer = nullptr;
            
            return *this;
        }

        String operator+(const char* rhs) {
            size_t new_size = m_size + strlen(rhs) + 1;
            char* new_buffer = new char[new_size];
            
            strcpy(new_buffer, this->m_buffer);
            strcat(new_buffer, rhs);
            
            String new_str(new_buffer);
            return new_str;
        }

        String operator+(const String& rhs) {
            size_t size = m_size + rhs.m_size + 1;
            char* new_buffer = new char[size];
            
            strcpy(new_buffer, m_buffer);
            strcat(new_buffer, rhs.m_buffer);
            
            String new_string(new_buffer);
            
            delete[] new_buffer;
            return new_string;
        }

        char& operator[] (size_t indx) noexcept(noexcept(indx < this->m_size)) {
            assert(indx < this->m_size && "Index must be within range\n");
            return m_buffer[indx];
        }

        friend std::ostream& operator << (std::ostream& out, const String src) {
            std::lock_guard<std::mutex> lock(mut);
            out << src.m_buffer;
            return out;
        }
       
        void* operator new(size_t size) {
            std::lock_guard<std::mutex> lock(m: mut);

            void* buffer = calloc(size, 1);

            if(!buffer) { throw std::bad_alloc(); }
            return buffer;
        }

        void operator delete(void* ptr) noexcept {
            mut.lock();
            free(ptr);
            mut.unlock();
        }

        ~String()
        {
           m_size = 0;
           delete[] m_buffer;
        }

};
