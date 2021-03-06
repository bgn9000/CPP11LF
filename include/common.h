#pragma once

#include <iostream>
#include <algorithm>
#include <ctime>
#include <cassert>
#include <cstring>
#include <cstdio>

#include <chrono>
#include <thread>
#include <atomic>

#ifdef USE_BLOSC
#   include <blosc.h>
#endif

#ifdef USE_DOUBLECONV
#   include <double-conversion.h>
#endif

using namespace std::chrono;

typedef unsigned long long ticks;
static __inline__ ticks getticks(void)
{
#if defined (__amd64__) || defined (__x86_64__)
    // Read the high res tick counter into 32 bit int variables "eax" and
    // "edx", and then combine them into 64 bit int "now"
    unsigned int eax, edx;
    asm volatile ("rdtsc" : "=a" (eax), "=d" (edx) : : "memory");
    return (((unsigned long long) eax) | (((unsigned long long) edx) << 32));
#else
    // Read the high-res tick counter directly into memory variable "now".
    // The A constraint signifies a 64-bit int.
    ticks now;
    asm volatile ("rdtsc" : "=A" (now) : : "memory");
    return now;
#endif
}

extern double FREQUENCY_CPU;

// tests
extern void testThread();
extern void testAlloc();
extern void testChrono();
extern void testAtomic();
extern void testOnePublisherOneListener();
extern void testCodecStringtoIntOrDouble();
extern void testBlosc();
extern void testDoubleConversion();

extern bool doTestThread;
extern bool doTestThread10ms;
extern bool doTestAlloc;
extern bool doTestChrono;
extern bool doTestAtomic;
extern bool doTestOnePublisherOneListenerThread;
extern bool doTestOnePublisherOneListenerThreadBasic;
extern bool doTestOnePublisherOneListenerThreadParallel;
extern bool doTestCodecStringtoIntOrDouble;
extern bool doTestBlosc;
extern bool doTestDoubleConversion;


