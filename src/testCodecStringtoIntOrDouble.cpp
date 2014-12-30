#include "common.h"

#include <boost/lexical_cast.hpp>

void testCodecStringtoIntOrDouble()
{
    std::cout << "================== testCodecStringtoIntOrDouble =================" << std::endl;
    
    high_resolution_clock::time_point start, end;
    nanoseconds time_span;
    
    constexpr int loop = 1000000;
    
    // int to string
    char buf[32] = {};
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        sprintf(buf, "%d", cpt);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "sprintf(int) took [" << time_span.count()/loop << "] ns" << std::endl;
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        boost::lexical_cast<std::string>(cpt);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "boost::lexical_cast<std::string>(int) took [" << time_span.count()/loop << "] ns" << std::endl;
    
    
    
    
    // string to int
    std::string buf2decode("12345678");
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        boost::lexical_cast<int>(buf2decode);
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
    
    
    
    // double to string
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        double dbl = cpt+0.125;
        sprintf(buf, "%lf", dbl);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "sprintf(double) took [" << time_span.count()/loop << "] ns" << std::endl;
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < loop; ++cpt)
    {
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
        boost::lexical_cast<std::string>(dbl);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "boost::lexical_cast<std::string>(double) took [" << time_span.count()/loop << "] ns" << std::endl;
 
    
    
    
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
