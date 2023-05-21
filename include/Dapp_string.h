#pragma once 
#include <cstring> // This imports strlen and strcpy functions
#include <iostream>

class String
{
    private:
        int m_size = 0;
        char* m_buffer = nullptr;
    
    public:
        //TODO
        // Implement core string functionality (constructors, destructors)
       String() {
           m_buffer = new char[1];
           m_buffer[0] = '\0';
       }

       explicit String(const char* str)
       :m_size(std::strlen(str)) { // Use std::strlen to clarify the scope
           m_buffer = new char[m_size + 1]; // Allocate space for the null character
           std::strcpy(m_buffer, str); // Use std::strcpy to clarify the scope
       }

       ~String()
       {
           m_size = 0;
           delete[] m_buffer;
       }

        // Implement operators
        // Implement helper functions
};
