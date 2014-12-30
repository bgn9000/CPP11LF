#include "common.h"

template<class T>
inline T atomicConditionalSet(T& currentValue, T newValue, T cmp)
{
    // Set currentValue to newValue if currentValue == cmp. Returns the previous value of currentValue
    asm("lock; cmpxchg %2, %1"
        : "+a"(cmp), "+m"(currentValue)
        : "r"(newValue)
        : "cc");
    return cmp;
}

template<class T>
inline T atomicSet(T& currentValue, T newValue)
{
    asm("lock; xchg %0, %1"
        : "+r"(newValue), "+m"(currentValue));
    return newValue;
}

template<class T>
inline T atomicGet(const T& currentValue)
{
    return atomicConditionalSet(const_cast<T&>(currentValue), 0, 0);
}

template <class T>
inline T atomicAdd(T& currentValue, T value)
{
    T prevValue = value;
    asm("lock; xaddl %0, %1"
        : "+r"(prevValue), "+m"(currentValue)
        :
        : "cc");
    return prevValue + value;
}

template <class T>
inline T atomicInc(T& currentValue)
{
    return atomicAdd(currentValue, 1);
}

template <class T>
inline T atomicDec(T& currentValue)
{
    return atomicAdd(currentValue, -1);
}

void testAtomic()
{
    std::cout << "================== testAtomic =================" << std::endl;
    
    std::atomic<int> atomicInt;
    if (atomicInt.is_lock_free())
    {
        std::cout << "Lock free atomic" << std::endl;
    }
    else
    {
        std::cout << "!!!!! Not lock free atomic !!!!" << std::endl;
    }

    ticks start, end;
    double latency = 0.0;
    constexpr int loop = 1000000;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        atomicInt.store(cpt);
        assert(cpt == atomicInt.load());
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "std::atomic<int> store/load with latency [" << latency << "] (ticks)" << std::endl;
    
    int myAtomic = 0;
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        atomicSet(myAtomic, cpt);
        assert(cpt == atomicGet(myAtomic));
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "asm atomicSet/atomicGet with latency [" << latency << "] (ticks)" << std::endl;

    atomicInt.store(1);
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        ++atomicInt;
        assert(1 == --atomicInt);
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "std::atomic<int> ++/-- with latency [" << latency << "] (ticks)" << std::endl;
    
    myAtomic = 1;
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        atomicInc(myAtomic);
        assert(1 == atomicDec(myAtomic));
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "asm atomicInc/atomicDec with latency [" << latency << "] (ticks)" << std::endl;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        atomicInt += 10;
        atomicInt -= 10;
        assert(1 == atomicInt.load());
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "std::atomic<int> +=/-= with latency [" << latency << "] (ticks)" << std::endl;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        atomicAdd(myAtomic, 10);
        atomicAdd(myAtomic, -10);
        assert(1 == atomicGet(myAtomic));
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "asm atomicAdd(+)/atomicAdd(-) with latency [" << latency << "] (ticks)" << std::endl;
}

