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
    while (end > begin)
    {
        const char reverse = *end;
        *end = *begin;
        --end;
        *begin = reverse;
        ++begin;
    }
}

template <typename T>
inline size_t convert_signed_base10(T val, char* str)
{
    if (val < 0)
    {
        *str = '-';
        ++str;
        return convert_signed_base10(-val, str)+1;
    }
    
    char* const begin = str;
    const char* entry;

    // Conversion. Number is reversed.
    while (val > 99)
    {
        const T tmp(val / 100);
        entry = &digit_lookup_table[(val - 100 * tmp) << 1];
        *str = *entry;
        ++str;
        *str = *(entry + 1);
        ++str;
        val = tmp;
    }

    // Convert remaining one or two digits.
    entry = &digit_lookup_table[val << 1];
    if (val > 9)
    {
        *str = *entry;
        ++str;
        ++entry;
    }
    *str = *entry;
    ++str;
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

    long long decimalMax = 1;
    for (int tmp = precision; tmp > 0; --tmp)
    {
        val *= 10;
        decimalMax *= 10;
    }
    val += 0.5;

    long long decimal = (long long)val;
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
        const char* entry;
        
        // Conversion. Number is reversed.
        while (decimal > 99)
        {
            const long long tmp(decimal / 100);
            entry = &digit_lookup_table[(decimal - 100 * tmp) << 1];
            str[pos2] = *entry;
            ++pos2;
            str[pos2] = *(entry + 1);
            ++pos2;
            decimal = tmp;
            precision -= 2;
        }

        // Convert remaining one or two digits.
        entry = &digit_lookup_table[decimal << 1];
        if (decimal > 9)
        {
            str[pos2] = *entry;
            ++pos2;
            ++entry;
            --precision;
        }
        str[pos2] = *entry;
        ++pos2;
        --precision;
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
        const char* entry;
        
        // Conversion. Number is reversed.
        while (integerPart > 99)
        {
            const long long tmp(integerPart / 100);
            entry = &digit_lookup_table[(integerPart - 100 * tmp) << 1];
            str[pos2] = *entry;
            ++pos2;
            str[pos2] = *(entry + 1);
            ++pos2;
            integerPart = tmp;
        }

        // Convert remaining one or two digits.
        entry = &digit_lookup_table[integerPart << 1];
        if (integerPart > 9)
        {
            str[pos2] = *entry;
            ++pos2;
            ++entry;
        }
        str[pos2] = *entry;
        ++pos2;
    }
    else
    {
        str[pos2] = '0';
        ++pos2;
    }
    
    strreverse(str+pos, str+pos2-1);

    return pos2;
}

template <typename T>
inline T retreive_float(const char* str, size_t size)
{
    if (size == 0) return 0;
    
    // sign
    int coef = 1;
    if (*str == '-')
    {
        coef = -1;
        --size;
        if (size == 0) return 0;
        ++str;
    }
    else if (*str == '+')
    {
        coef = 1;
        --size;
        if (size == 0) return 0;
        ++str;
    }
    
    // Integer part
    long long integerPart = 0;
    for (; size && *str && *str != '.' ; --size, ++str)
    {
        integerPart *= 10;
        integerPart += *str - '0';
    }
    T num = static_cast<double>(integerPart);

    // Decimal
    if (size && *str == '.')
    {
        --size;
        ++str;

        T reminder = 0;
        int decimalSize = 0;
        for (; size && *str ; --size, ++str)
        {
            reminder *= 10;
            reminder += *str - '0';
            ++decimalSize;
        }

        for (; decimalSize; --decimalSize)
            reminder /= 10;

        num += reminder;
    }
    return num * coef;
}

void testCodecStringtoIntOrDouble()
{
    std::cout << "================== testCodecStringtoIntOrDouble =================" << std::endl;
    
    high_resolution_clock::time_point start, end;
    auto time_span1 = 0ULL, time_span2 = 0ULL, time_span3 = 0ULL, time_span4 = 0ULL;
    
    constexpr int loop = 1'000'000;
    
    // reverse string
    // ==============
    
    std::string mystring("ABCDEFGHIJKLMNOPQRSTUVWXYUZ");
    for (int cpt=0; cpt < loop; ++cpt)
    {
        std::string ret = boost::lexical_cast<std::string>(cpt) + mystring;
        char* ret2 = strdup(ret.c_str());
        
        start = high_resolution_clock::now();
        std::reverse(std::begin(ret), std::end(ret));
        end = high_resolution_clock::now();
        time_span1 += duration_cast<nanoseconds>(end - start).count();
        
        start = high_resolution_clock::now();
        strreverse(ret2, ret2+strlen(ret2)-1);
        end = high_resolution_clock::now();
        time_span2 += duration_cast<nanoseconds>(end - start).count();
        
        // check algo
        assert(ret == ret2 || printf("returns differs (%s) is not equal (%s)", ret.c_str(), ret2) == 0);
        free(ret2);
    }
    std::cout << "std::reverse(string) took [" << time_span1/loop << "] ns" << std::endl;
    std::cout << "strreverse(string) took [" << time_span2/loop << "] ns" << std::endl;
    
    // int to string
    // =============
    
    time_span1 = 0ULL, time_span2 = 0ULL, time_span3 = 0ULL, time_span4 = 0ULL;
    for (int cpt=0; cpt < loop; ++cpt)
    {
        char buf[32] = {};
        
        start = high_resolution_clock::now();
        sprintf(buf, "%d", cpt);
        end = high_resolution_clock::now();
        time_span1 += duration_cast<nanoseconds>(end - start).count();
        
        start = high_resolution_clock::now();
        std::string res = boost::lexical_cast<std::string>(cpt);
        end = high_resolution_clock::now();
        time_span2 += duration_cast<nanoseconds>(end - start).count();
        
        memset(&buf, '\0', 32);
        start = high_resolution_clock::now();
        auto ret = convert_signed_base10<int>(cpt, buf);
        end = high_resolution_clock::now();
        time_span3 += duration_cast<nanoseconds>(end - start).count();
        
        // check algo
        assert(ret == res.length());
        assert(res == buf);
    }
    std::cout << "sprintf(int) took [" << time_span1/loop << "] ns" << std::endl;    
    std::cout << "boost::lexical_cast<std::string>(int) took [" << time_span2/loop << "] ns" << std::endl;
    std::cout << "convert_signed_base10<int> took [" << time_span3/loop << "] ns" << std::endl;
        
    // string to int
    // =============
    
    time_span1 = 0ULL, time_span2 = 0ULL, time_span3 = 0ULL, time_span4 = 0ULL;
    char *endptr = 0;
    for (int cpt=0; cpt < loop; ++cpt)
    {
        std::string buf2decode = boost::lexical_cast<std::string>(cpt);
        
        start = high_resolution_clock::now();
        long ret = boost::lexical_cast<int>(buf2decode);
        end = high_resolution_clock::now();
        time_span1 += duration_cast<nanoseconds>(end - start).count();
        
        start = high_resolution_clock::now();
        long ret2 = strtol(buf2decode.c_str(), &endptr, 10);
        end = high_resolution_clock::now();
        time_span2 += duration_cast<nanoseconds>(end - start).count();
        
        start = high_resolution_clock::now();
        ret2 = std::stoi(buf2decode);
        end = high_resolution_clock::now();
        time_span3 += duration_cast<nanoseconds>(end - start).count();
        
        start = high_resolution_clock::now();
        ret2 = retreive_signed_base10<int>(buf2decode.c_str(), buf2decode.length());
        end = high_resolution_clock::now();
        time_span4 += duration_cast<nanoseconds>(end - start).count();
        
        // check algo
        assert(ret == ret2 || printf("returns differs (%ld) is not equal (%ld)", ret, ret2) == 0);
    }
    std::cout << "boost::lexical_cast<int>(string) took [" << time_span1/loop << "] ns" << std::endl;
    std::cout << "strtol took [" << time_span2/loop << "] ns" << std::endl;
    std::cout << "std::stoi took [" << time_span3/loop << "] ns" << std::endl;
    std::cout << "retreive_signed_base10<int>(string) took [" << time_span4/loop << "] ns" << std::endl;
    
    // double to string
    // ================
    
    time_span1 = 0ULL, time_span2 = 0ULL, time_span3 = 0ULL, time_span4 = 0ULL;
    for (int cpt=0; cpt < loop; ++cpt)
    {
        char buf[32] = {};
        double dbl = cpt+0.125;
        
        start = high_resolution_clock::now();
        sprintf(buf, "%lf", dbl);
        end = high_resolution_clock::now();
        time_span1 += duration_cast<nanoseconds>(end - start).count();
        
        memset(&buf, '\0', 32);
        
        start = high_resolution_clock::now();
        char* begin = gcvt(dbl, 15, buf);
        end = high_resolution_clock::now();
        time_span2 += duration_cast<nanoseconds>(end - start).count();
        
        start = high_resolution_clock::now();
        std::string res = boost::lexical_cast<std::string>(dbl);
        end = high_resolution_clock::now();
        time_span3 += duration_cast<nanoseconds>(end - start).count();
        
        start = high_resolution_clock::now();
        size_t ret = convert_float<double>(buf, dbl, 3);
        end = high_resolution_clock::now();
        time_span4 += duration_cast<nanoseconds>(end - start).count();
        
        // check algo
        assert(ret == res.length() || printf("returns differs (%ld) is not equal (%ld)", ret, res.length()) == 0);
        assert(res == buf || printf("returns differs (%s) is not equal (%s)", res.c_str(), buf) == 0);
    }
    std::cout << "sprintf(double) took [" << time_span1/loop << "] ns" << std::endl;
    std::cout << "gcvt took [" << time_span2/loop << "] ns" << std::endl;
    std::cout << "boost::lexical_cast<std::string>(double) took [" << time_span3/loop << "] ns" << std::endl;
    std::cout << "convert_float<double>(string,double) took [" << time_span4/loop << "] ns" << std::endl;

    // string to double
    // ================
    
    time_span1 = 0ULL, time_span2 = 0ULL, time_span3 = 0ULL, time_span4 = 0ULL;
    for (int cpt=0; cpt < loop; ++cpt)
    {
        double dbl = cpt+0.125;
        std::string buf2decode = boost::lexical_cast<std::string>(dbl);
        
        start = high_resolution_clock::now();
        double ret = boost::lexical_cast<double>(buf2decode);
        end = high_resolution_clock::now();
        time_span1 += duration_cast<nanoseconds>(end - start).count();
        
        start = high_resolution_clock::now();
        ret = strtod(buf2decode.c_str(), &endptr);
        end = high_resolution_clock::now();
        time_span2 += duration_cast<nanoseconds>(end - start).count();
        
        start = high_resolution_clock::now();
        ret = std::stod(buf2decode);
        end = high_resolution_clock::now();
        time_span3 += duration_cast<nanoseconds>(end - start).count();
        
        start = high_resolution_clock::now();
        double ret2 = retreive_float<double>(buf2decode.c_str(), buf2decode.length());
        end = high_resolution_clock::now();
        time_span4 += duration_cast<nanoseconds>(end - start).count();
        
        // check algo
        assert(ret == ret2 || printf("returns differs (%lf) is not equal (%lf)", ret, ret2) == 0);
    }
    std::cout << "boost::lexical_cast<double>(string) took [" << time_span1/loop << "] ns" << std::endl;    
    std::cout << "strtod took [" << time_span2/loop << "] ns" << std::endl;
    std::cout << "std::stod took [" << time_span3/loop << "] ns" << std::endl;
    std::cout << "retreive_float<double>(string) took [" << time_span4/loop << "] ns" << std::endl;
}

