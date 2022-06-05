//
// Created by danil on 02.05.2022.
//

#ifndef L_1_EXCEPTIONS_H
#define L_1_EXCEPTIONS_H
#include <exception>
#include <string>

class LargeSizeException:public std::exception
{
private:
    std::string m_error;
public:
    explicit LargeSizeException(std::string e)
    {
        m_error = e;
    }
    const char* what() const noexcept override
    {
        return m_error.c_str();
    }
};

class NoMemoryAvailableException:public std::exception
{
private:
    std::string m_error;
public:
    explicit NoMemoryAvailableException(std::string e)
    {
        m_error = e;
    }
    const char* what() const noexcept override
    {
        return m_error.c_str();
    }
};


#endif //L_1_EXCEPTIONS_H
