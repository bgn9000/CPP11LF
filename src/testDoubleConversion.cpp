#include "common.h"

#include <boost/lexical_cast.hpp>

void testDoubleConversion()
{
#ifdef USE_DOUBLECONV
    std::cout << "================== testDoubleConversion =================" << std::endl;
    
    high_resolution_clock::time_point start, end;
    nanoseconds time_span;
    
    constexpr int loop = 1'000'000;
    
    // double to string
    // ================
    
    using ::double_conversion::DoubleToStringConverter;
    using ::double_conversion::StringBuilder;
    DoubleToStringConverter conv(DoubleToStringConverter::NO_FLAGS, "Infinity", "NaN", 'E', 0, 0, 0, 0);
    
    // check this
    for (int cpt=0; cpt < loop; ++cpt)
    {
        double dbl = cpt+0.125;
        std::string res = boost::lexical_cast<std::string>(dbl);
        char buf[32] = {};
        
        StringBuilder builder(buf, sizeof(buf));
        conv.ToFixed(dbl, 3, &builder);
        const size_t ret = builder.position();
        builder.Finalize();        
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
        StringBuilder builder(buf, sizeof(buf));
        conv.ToFixed(dbl, 3, &builder);
        const size_t length = builder.position();
        builder.Finalize();
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "DoubleToStringConverter took [" << time_span.count()/loop << "] ns" << std::endl;
    
    // string to double
    // ================
    
    std::string buf2decode("123.45678");
    using ::double_conversion::StringToDoubleConverter;
    StringToDoubleConverter conv2(StringToDoubleConverter::ALLOW_TRAILING_JUNK | StringToDoubleConverter::ALLOW_LEADING_SPACES,
                                 0.0,
                                 // return this for junk input string
                                 std::numeric_limits<double>::quiet_NaN(),
                                 nullptr, nullptr);
    
    // check this
    for (int cpt=0; cpt < loop; ++cpt)
    {
        double dbl = cpt+0.125;
        std::string buf2decode2 = boost::lexical_cast<std::string>(dbl);
        double ret = boost::lexical_cast<double>(buf2decode2);
        int length;
        auto ret2 = conv2.StringToDouble(buf2decode2.c_str(), buf2decode2.length(), &length); // processed
        assert(ret == ret2);
    }
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        int length;
        auto ret = conv2.StringToDouble(buf2decode.c_str(), buf2decode.length(), &length); // processed
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "StringToDoubleConverter took [" << time_span.count()/loop << "] ns" << std::endl;
#endif
}

