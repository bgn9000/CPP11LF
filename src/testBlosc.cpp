#include "common.h"

void testBlosc()
{
#ifdef USE_BLOSC
    std::cout << "================== testBlosc =================" << std::endl;

    ticks start, end;
    double latency = 0.0;
    constexpr int loop = 1000000;
    
    char buffer[512] = "";
    for (unsigned int cpt=0; cpt < sizeof(buffer)-1; ++cpt)
    {
        // text
        // buffer[cpt] = (cpt % 26) + 'A';
        // binary
        buffer[cpt] = (cpt % 255) + 1;
    }
    buffer[sizeof(buffer)-1]=0;
    //std::cout << "buffer=" << buffer << std::endl;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        static char buffer_dest[sizeof(buffer)] = "";
        memcpy((void*)buffer_dest, (void*)buffer, sizeof(buffer));
    }
    end = getticks();
    latency = static_cast<ticks>((ticks)(end-start))/loop;
//    latency = static_cast<ticks>(end-start);
    std::cout << "memcpy with latency [" << latency << "] (ticks)" << std::endl;

    std::cout << "Blosc Version [" << BLOSC_VERSION_STRING << "] date [" << BLOSC_VERSION_DATE << "]" << std::endl;
    blosc_init();
    
    // Compress with clevel=5 and shuffle active
    //blosc_set_nthreads(2);
    int csize = 0;
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        char buffer_dest[sizeof(buffer)] = "";
        csize = blosc_compress(5, 1, sizeof(char), sizeof(buffer), buffer, buffer_dest, sizeof(buffer));
    }
    end = getticks();
    latency = static_cast<ticks>((ticks)(end-start))/loop;
//    latency = static_cast<ticks>(end-start);
    std::cout << "blosc_compress with latency [" << latency << "] (ticks)" << std::endl;
    if (csize)
    {
        std::cout << "compression rate [" << (1.*sizeof(buffer)) / csize << "]%" << std::endl;
    }
    
    int dsize = 0;
    char buffer_dest[sizeof(buffer)] = "";
    csize = blosc_compress(5, 1, sizeof(char), sizeof(buffer), buffer, buffer_dest, sizeof(buffer));
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        char buffer_dest2[sizeof(buffer)] = "";
        csize = blosc_decompress(buffer_dest, buffer_dest2, sizeof(buffer));
    }
    end = getticks();
    latency = static_cast<ticks>((ticks)(end-start))/loop;
//    latency = static_cast<ticks>(end-start);
    std::cout << "blosc_decompress with latency [" << latency << "] (ticks)" << std::endl;
    
    // After using it, destroy the Blosc environment
    blosc_destroy();
#endif
}
