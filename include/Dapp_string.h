#pragma once 

#include <cstring> // This imports strlen and strcpy functions
#include <iostream>
#include <utility> // std::move
#include <cassert>

class String
{
    private:
        int m_size = 0;
        char* m_buffer = nullptr;
    
    public:
        //TODO
        // Implement core string functionality (constructors, destructors)
       explicit String() {
           m_buffer = new char[1];
           m_buffer[0] = '\0';
       }
        
       //Explicit constructor to only allow either empty construction, or construction of strings
       //This disallows users from passing something like an integer or double as a constructor parameter
       explicit String(const char* str)
           :m_size(strlen(str)) { 
           m_buffer = new char[m_size + 1];
           strcpy(m_buffer, str);
       }

       //Copy constructor
       String(const String& src)
           :m_size(src.m_size) {
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
        inline bool isEmpty() { return m_size > 0 ? false : true; }
        inline const char* begin() { return &m_buffer[0]; }
        inline const char* end() { return &m_buffer[m_size-1]; }

        //Will zero every element in the string, but retain the size
        //Assuming that the m_size member is greater than 0 in order to not underflow when setting the null terminator
        void empty() {
            assert(m_size > 0 && "Error! empty() requires size greater than 0"); 
            memset(m_buffer, 0, m_size);
            m_buffer[m_size-1] = '\0';
        }
        
        //Operators


        ~String()
        {
           m_size = 0;
           delete[] m_buffer;
        }

};
