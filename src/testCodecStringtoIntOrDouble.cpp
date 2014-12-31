#include "common.h"

#include <boost/lexical_cast.hpp>

static const char* digit_lookup_table =
{
    "00102030405060708090" \
    "01112131415161718191" \
    "02122232425262728292" \
    "03132333435363738393" \
    "04142434445464748494" \
    "05152535455565758595" \
    "06162636465666768696" \
    "07172737475767778797" \
    "08182838485868788898" \
    "09192939495969798999"
};

inline void strreverse(char* begin, char* end)
{
    char reverse;
    while (end > begin)
    {
        reverse = *end;
        *end-- = *begin;
        *begin++ = reverse;
    }
}

template <typename T>
inline size_t convert_signed_base10(T val, char* str)
{
    if (val < 0)
    {
        *str++ = '-';
        return convert_signed_base10(-val, str)+1;
    }
    
    char* const begin = str;
    const char* entry;

    // Conversion. Number is reversed.
    while (val > 99)
    {
        const T tmp(val / 100);
        entry = &digit_lookup_table[(val - 100 * tmp) << 1];
        *str++ = *entry;
        *str++ = *(entry + 1);
        val = tmp;
    }

    // Convert remaining one or two digits.
    entry = &digit_lookup_table[val << 1];
    if (val > 9)
    {
        *str++ = *entry++;
    }

    *str++ = *entry;
    *str=0;

    strreverse(begin, str-1);

    return str-begin;
}

template <typename T>
inline T retreive_signed_base10(const char* str, size_t size)
{
    // sign
    int coef = 1;
    if (*str == '-')
    {
        coef = -1;
        --size;
        ++str;
    }
    else if (*str == '+')
    {
        coef = 1;
        --size;
        ++str;
    }

    T val = 0;
    for (; size; --size, ++str)
    {
        val *= 10;
        val += *str - '0';
    }
    return coef * val;
}

template <typename T>
inline size_t convert_float(char* str, T val, int precision)
{
    int pos = 0;
    if (val < 0)
    {
        val = -val;
        str[pos] = '-';
        ++pos;
    }

    long long integerPart = (long long)val;
    val -= integerPart;

    int decimalMax = 1;
    for (int tmp = precision; tmp > 0; --tmp)
    {
        val *= 10;
        decimalMax *= 10;
    }
    val += 0.5;

    int decimal = (int)val;
    // overflow?
    if (decimal >= decimalMax)
    {
        // increment the integer part and decrement the decimal ones
        ++integerPart;
        decimal -= decimalMax;
    }

    // Writing decimal part in reverse order
    int pos2 = pos;
    if (decimal != 0)
    {
        for (int tmp = decimal; tmp != 0 && precision != 0; tmp /= 10)
        {
            str[pos2] = '0' + (char)(tmp % 10);
            ++pos2;
            --precision;
        }
    }
    // Add some missing '0'
    for (int tmp = precision; tmp != 0; --tmp)
    {
        str[pos2] = '0';
        ++pos2;
    }

    // Writing the dot
    str[pos2] = '.';
    ++pos2;

    // Writing integer part in reverse order
    if (integerPart != 0)
    {
        for (long long tmp = integerPart; tmp != 0; tmp /= 10)
        {
            str[pos2] = '0' + (char)(tmp % 10);
            ++pos2;
        }
    }
    else
    {
        str[pos2] = '0';
        ++pos2;
    }

    size_t size = pos2;

    // Reverse back what was written
    while (pos2 > pos)
    {
        char c = str[--pos2];
        str[pos2] = str[pos];
        str[pos++] = c;
    }

    return size;
}

void testCodecStringtoIntOrDouble()
{
    std::cout << "================== testCodecStringtoIntOrDouble =================" << std::endl;
    
    high_resolution_clock::time_point start, end;
    nanoseconds time_span;
    
    constexpr int loop = 1000000;
    
    // int to string
    // =============
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        char buf[32] = {};
        sprintf(buf, "%d", cpt);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "sprintf(int) took [" << time_span.count()/loop << "] ns" << std::endl;
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        std::string ret = boost::lexical_cast<std::string>(cpt);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "boost::lexical_cast<std::string>(int) took [" << time_span.count()/loop << "] ns" << std::endl;
    
    // check my algo
    for (int cpt=0; cpt < loop; ++cpt)
    {
        char buf[32] = {};
        std::string res = boost::lexical_cast<std::string>(cpt);
        size_t ret = convert_signed_base10<int>(cpt, buf);
        assert(ret == res.length());
        assert(res == buf);
    }
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        char buf[32] = {};
        int ret = convert_signed_base10<int>(cpt, buf);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "convert_signed_base10<int> took [" << time_span.count()/loop << "] ns" << std::endl;
    
    // string to int
    // =============
    
    std::string buf2decode("12345678");
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        int ret = boost::lexical_cast<int>(buf2decode);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "boost::lexical_cast<int>(string) took [" << time_span.count()/loop << "] ns" << std::endl;
    
    char *endptr = 0;
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        long ret = strtol(buf2decode.c_str(), &endptr, 10);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "strtol took [" << time_span.count()/loop << "] ns" << std::endl;
    
    // check my algo
    for (int cpt=0; cpt < loop; ++cpt)
    {
        std::string buf2decode = boost::lexical_cast<std::string>(cpt);
        int ret = boost::lexical_cast<int>(buf2decode);
        int ret2 = retreive_signed_base10<int>(buf2decode.c_str(), buf2decode.length());
        assert(ret == ret2);
    }
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        int ret = retreive_signed_base10<int>(buf2decode.c_str(), buf2decode.length());
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "retreive_signed_base10<int>(string) took [" << time_span.count()/loop << "] ns" << std::endl;
    
    // double to string
    // ================
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        char buf[32] = {};
        double dbl = cpt+0.125;
        sprintf(buf, "%lf", dbl);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "sprintf(double) took [" << time_span.count()/loop << "] ns" << std::endl;
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        char buf[32] = {};
        double dbl = cpt+0.125;
        char* begin = gcvt(dbl, 15, buf);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "gcvt took [" << time_span.count()/loop << "] ns" << std::endl;
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        double dbl = cpt+0.125;
        std::string ret = boost::lexical_cast<std::string>(dbl);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "boost::lexical_cast<std::string>(double) took [" << time_span.count()/loop << "] ns" << std::endl;
 
    // check my algo
    for (int cpt=0; cpt < loop; ++cpt)
    {
        double dbl = cpt+0.125;
        std::string res = boost::lexical_cast<std::string>(dbl);
        char buf[32] = {};
        size_t ret = convert_float<double>(buf, dbl, 3);
//std::cout << "buf=[" << buf << "] res=[" << res << "]" << std::endl;
//std::cout << "ret=[" << ret << "] resLength=[" << res.length() << "]" << std::endl;
        assert(ret == res.length());
        assert(res == buf);
    }
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        char buf[32] = {};
        double dbl = cpt+0.125;
        size_t ret = convert_float<double>(buf, dbl, 6);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "convert_float<double>(string,double) took [" << time_span.count()/loop << "] ns" << std::endl;

    // string to double
    buf2decode = "123.45678";
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        boost::lexical_cast<double>(buf2decode);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "boost::lexical_cast<double>(string) took [" << time_span.count()/loop << "] ns" << std::endl;
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        double ret = strtod(buf2decode.c_str(), &endptr);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "strtod took [" << time_span.count()/loop << "] ns" << std::endl;
    
    
}