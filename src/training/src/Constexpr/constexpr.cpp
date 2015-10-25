#include <iostream>

template <typename T, size_t SIZE = 1024>
class StupidString
{
    T data[SIZE];
};

constexpr size_t constexprlen(const char* _data)
{
    return *_data == 0 ? 0 : 1+constexprlen(_data+1);
}

class constexprString
{
    const char* data;
    size_t len;

public:
    constexpr constexprString(const char* _data) 
        : data(_data), len(constexprlen(_data))
    {}
    constexpr size_t size() const { return len; }
};

template <class T>
class test_staticassert
{
    T data;
public:
    test_staticassert()
    {
        static_assert(sizeof(data) < 3, "only small data size inf 3 bytes");
    }
};

int main()
{
    // first : define constexpr what is
    constexpr int charsize = 128 * sizeof(char);
    const int charsize2 = 128 * sizeof(char);
    StupidString<char> str1;
    std::cout << "sizeof(str1) = " << sizeof(str1) << std::endl;
    StupidString<char, 512> str2;
    std::cout << "sizeof(str2) = " << sizeof(str2) << std::endl;
    StupidString<char, charsize> str3;
    std::cout << "sizeof(str3) = " << sizeof(str3) << std::endl;
    StupidString<char, charsize2> str4;
    std::cout << "sizeof(str4) = " << sizeof(str4) << std::endl;
    //const int sizes[] = { 256, 2046 }; //=> need constexpr : initialize compilation stage
    constexpr int sizes[] = { 256, 2046 };
    StupidString<char, sizes[1]> str5;
    std::cout << "sizeof(str5) = " << sizeof(str5) << std::endl;
    //constexpr std::string hello = "hello world!"; //=> string is not constexpr
    constexpr constexprString hello = "hello world!";
    StupidString<char, hello.size()> str6;
    std::cout << "sizeof(str6) = " << sizeof(str6) << std::endl;
    // constexpr allows metaprogramming without templates

    // static assertions (test at compilation time and writes a string as an error if test is false)
    static_assert(hello.size() == 12, "wrong size");
    test_staticassert<char> test_assert1;
    test_staticassert<short> test_assert2;
    //test_staticassert<int> test_assert3;

    // Enum (scoped and strongly typed enums)
    constexpr char zero = '0';
    constexpr char one = '1';
    constexpr char two = '2';
    enum OldDigits
    {
        oldzero = zero,
        oldone = one,
        oldtwo = two,
    };
    char test_enum1 = oldzero;
    enum class Digits : char
    {
        zero = zero,
        one = one,
        two = two,
    };
    // char test_enum2 = zero; //=> zero in scope of Digits
    // char test_enum3 = Digits::zero; //=> cannot cast rvalue Digits into char
    Digits test_enum4 = Digits::two;
    std::cout << "test_enum4=" << static_cast<char>(test_enum4) 
        << "(" <<  static_cast<int>(test_enum4) << ")" << std::endl;
    return 0;
}
