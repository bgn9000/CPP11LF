#include <iostream>
#include <string>
#include <cstring>
#include <cassert>

void foo(std::string& str) {}
void foo2(std::string&& str) {}
void bar(const std::string& str) {}

void whichValueRef(std::string&) { std::cout << "lvalue ref" << std::endl; }
void whichValueRef(const std::string&) { std::cout << "const lvalue ref" << std::endl; }
void whichValueRef(std::string&&) { std::cout << "rvalue ref" << std::endl; }

// move semantics : move constructor and assignement
// steal internals of a temporary but moving data from one object to another
// should reset internals of origin at the end of the move
class StringWithMove
{
    char* buffer;
    size_t len;
public:
    StringWithMove() = delete;
    StringWithMove(const char* _buffer, size_t _len)  : buffer{new char[_len+1]}, len{_len}
    {
        std::cout << "StringWithMove constructor" << std::endl;
        strncpy(buffer, _buffer, len+1);
    }
    ~StringWithMove() { if (buffer != nullptr) delete [] buffer; }

    StringWithMove(const StringWithMove& s) : buffer{new char[s.len+1]}, len{s.len}
    {
        std::cout << "StringWithMove copy constructor" << std::endl;
        strncpy(buffer, s.buffer, len+1);
    }
    StringWithMove(StringWithMove&& s) : buffer{std::move(s.buffer)}, len{std::move(s.len)}
    {
        std::cout << "StringWithMove move constructor" << std::endl;
        s.buffer = nullptr;
        s.len = 0;
    }

    StringWithMove substr(size_t _pos, size_t _len)
    {
        assert(_len > 0 && len > _pos +_len);
        char* _buffer = new char[_len];
        strncpy(_buffer, buffer+_pos, _len);
        return StringWithMove(_buffer, _len);
    }
    StringWithMove substr2(size_t _pos, size_t _len)
    {
        assert(_len > 0 && len > _pos +_len);
        char* _buffer = new char[_len];
        strncpy(_buffer, buffer+_pos, _len);
        return std::move(StringWithMove(_buffer, _len));
    }

    StringWithMove& operator=(const StringWithMove& s)
    {
        if (this != &s)
        {
            std::cout << "StringWithMove copy assignement" << std::endl;
            buffer = new char[s.len+1];
            len = s.len;
            strncpy(buffer, s.buffer, len+1);
        }
        return *this;
    }
    StringWithMove& operator=(StringWithMove&& s)
    {
        if (this != &s)
        {
            std::cout << "StringWithMove move assignement" << std::endl;
            buffer = s.buffer;
            len = s.len;
            s.buffer = nullptr;
            s.len = 0;
        }
        return *this;
    }
};

template <class T> void swap(T& t1, T& t2)
{
    T temp(t1);
    t1 = t2;
    t2 = temp;
}

template <class T> void swapWithMove(T& t1, T& t2)
{
    T temp(std::move(t1));
    t1 = std::move(t2);
    t2 = std::move(temp);
}

int main()
{
    std::string hello{"hello world!"};
    // foo(hello.substr(0,3)); //=> expect only lvalue (need rvalue ref)
    foo2(hello.substr(0,3));
    bar(hello.substr(0,3));
    std::string& str1 = hello;
    std::string&& str2 = hello.substr(0,3);
    // std::string&& str3 = hello; //=> don't bind lvalue with rvalue ref

    // rvalue become a lvalue once assign to a variable
    foo(str2);
    // foo2(str2); //=> not anymore an rvalue
    bar(str2);

    whichValueRef(hello);
    whichValueRef(str1);
    const std::string& str3 = hello;
    whichValueRef(str3);
    whichValueRef(hello.substr(0,3)); // rvalue ref
    whichValueRef(str2);

    // turn anything to a rvalue with std::move and then bound to a rvalue ref
    std::cout << "std::move..." << std::endl;
    whichValueRef(std::move(hello));
    whichValueRef(std::move(str1));
    whichValueRef(std::move(str2));
    // not really anything & + && => & (collapsing rule)
    whichValueRef(std::move(str3)); // stay a rvalue

    // move semantics
    // StringWithMove str4; //=> default constructor deleted
    StringWithMove str5("hello world!", 12);
    StringWithMove str6(str5);
    StringWithMove str7(str5.substr(0,3)); // rvo : construct temporary and use as is
    StringWithMove str8(str5.substr2(0,3)); // construct temporary, pass it as rvalue and move

    // get rid off useless copies
    std::cout << "swap..." << std::endl;
    swap(str7, str8);
    swapWithMove(str7, str8);
    return 0;
}
